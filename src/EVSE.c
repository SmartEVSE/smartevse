/*
;    Project:       Smart EVSE (3Phase), with current measurement via Current Transmitters over RS485
;    Date:          16 October 2013
;
;    Changes:
;    1.00  Initial release
;    1.01  Added Normal EVSE mode + Cable Lock support
;
;    (C) 2013  Michael Stegen / Stegen Electronics
;
; Permission is hereby granted, free of charge, to any person obtaining a copy
; of this software and associated documentation files (the "Software"), to deal
; in the Software without restriction, including without limitation the rights
; to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
; copies of the Software, and to permit persons to whom the Software is
; furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in
; all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
; IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
; FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
; AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
; LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
; OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
; THE SOFTWARE.
*/


#include "p18f25k22.h"
#include <stdio.h>
#include <usart.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void SetCurrent(unsigned int);
unsigned int CalcCurrent();



// Configuration settings
#pragma	config FCMEN = OFF,	IESO = OFF, PRICLKEN = ON
#pragma config PLLCFG = OFF, FOSC = HSMP // High Speed Medium power (4-16Mhz), PLL Off
#pragma	config BORV = 285, BOREN = OFF, PWRTEN = ON
#pragma	config WDTPS = 2048, WDTEN = OFF     // WDT timeout
#pragma config CCP2MX = PORTB3, PBADEN = OFF, CCP3MX = PORTC6  // PortB digital IO
#pragma config HFOFST = OFF, T3CMX = PORTB5, P2BMX = PORTC0, MCLRE = INTMCLR

#pragma config XINST = ON, DEBUG = OFF, LVP = OFF, STVREN = ON
#pragma	config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF, CPD = OFF, CPB = OFF
#pragma	config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma	config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma	config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma	config EBTRB = OFF

#define ICAL 3.00						// Irms Calibration value (for Current transformers) 
#define MAX_MAINS 13					// max Current the Mains connection can supply
#define MAX_CURRENT 13					// max charging Current for the EV
#define MIN_CURRENT 6					// minimum Current the EV will accept
#define MODE 0							// Normal EVSE mode
#define LOCK 0							// No Cable lock

#define GOODFCS16 0x0f47				// crc16 frame check value

#define STATE_A 1						// Vehicle not connected
#define STATE_B 2						// Vehicle connected / not ready to accept energy
#define STATE_C 3						// Vehicle connected / ready to accept energy / ventilation not required
#define STATE_D 4						// Vehicle connected / ready to accept energy / ventilation required

#define PILOT_12V 1
#define PILOT_9V 2
#define PILOT_6V 3
#define PILOT_DIODE 4 
#define PILOT_NOK 0

#define NO_ERROR 0
#define LESS_6A 1
#define CT_NOCOMM 2
#define OVERCURRENT 4 
#define TEMP_HIGH 5

#define SOLENOID_LOCK		{PORTAbits.RA4 = 1;PORTAbits.RA5 = 0;}
#define SOLENOID_UNLOCK		{PORTAbits.RA4 = 0;PORTAbits.RA5 = 1;}
#define SOLENOID_OFF		{PORTAbits.RA4 = 1;PORTAbits.RA5 = 1;}

#define CONTACTOR_OFF PORTBbits.RB4 = 0;				// Contactor OFF
#define CONTACTOR_ON  PORTBbits.RB4 = 1;				// Contactor ON


#pragma romdata eedata=0xf00000
rom unsigned int EE_MaxMains = MAX_MAINS;
rom unsigned int EE_MaxCurrent = MAX_CURRENT;
rom unsigned int EE_MinCurrent = MIN_CURRENT;
rom double EE_ICal = ICAL;
rom char EE_Mode = MODE;
rom char EE_Lock = LOCK;
#pragma romdata



// Global data
#pragma udata udata
char	U1buffer[50];		// Uart1 Receive buffer /RS485
char	U2buffer[50];		// Uart2 buffer /Serial CLI

#pragma udata
// The following data will be updated by eeprom data at powerup:
unsigned int MaxMains;				// Max Mains Amps (hard limit, limited by the MAINS connection)
unsigned int MaxCurrent;			// Max Charge current
unsigned int MinCurrent;			// Minimal current the EV is happy with
double ICal;						// CT calibration value
char Mode;							// EVSE mode
char Lock;							// Cable lock enable/disable
// total 12 bytes

double Irms[4]={0,0,0,0};			// Momentary current per Phase (Amps *10) (23= 2.3A)
									// Max 4 phases supported

unsigned int crc16;
unsigned char State = STATE_A;
unsigned char Error = NO_ERROR;
unsigned char NextState;

unsigned int MaxCapacity;			// Cable limit (Amps)(limited by the wire in the charge cable, set automatically)
unsigned int Ilimit;				// Max current (Amps *10), depending on MaxCapacity and MaxMains
unsigned int Imeasured=0;			// Max of all CT inputs (Amps *10)
unsigned int Iset;					// PWM output is set to this current level (Amps *10)

unsigned char RX1byte;
unsigned char idx=0,idx2=0,ISRFLAG=0,ISR2FLAG=0;
unsigned char menu=0;
unsigned int locktimer=0,unlocktimer=0;	// solenoid timers
unsigned long Timer=0;				// mS counter
unsigned char LCDTimer=0;
unsigned char TempEVSE=0;			// Temperature EVSE in deg C (0-125)


void high_isr(void);

//sets the interrupt vector to point to the high_isr function
#pragma code int_service = 0x08
void int_service(void)
{
  _asm goto high_isr _endasm
}
#pragma code

#pragma	interrupt high_isr
void high_isr(void)
{
	// Determine what caused the interrupt
	while(PIR1bits.RC1IF)				// Uart1 receive interrupt? RS485
	{
		RX1byte = RCREG1;			// copy received byte	
									// check for start/end of data packet byte, and max number of bytes in buffer 
		if (idx == 50) idx--;
		if (RX1byte == 0x7E)		// max 50 bytes in buffer
		{
			if (idx > 7) 			// end of packet detected?
			{
				ISRFLAG = idx;		// flag complete packet for main routine
			} 
			idx = 0;				// reset index
		}
		else if (RX1byte == 0x7D)	// escape character found?
		{
			ISRFLAG = 1;			// yes, mark next byte
		}
		else						// normal characters
		{
			if (ISRFLAG == 1 )		// was previous byte a escape character?
			{
				ISRFLAG = 0;
				RX1byte = 0x20 ^ RX1byte; 
			}
			U1buffer[idx++] = RX1byte;	
		}
	//	PIR1bits.RC1IF = 0;			// Reset interrupt flag
	}


	while(PIR3bits.RC2IF)						// Uart2 receive interrupt?
	{
												// Check for BREAK character, then Reset
   		if(RCSTA2bits.FERR && (State == STATE_A) && RCONbits.POR)
		{										// Make sure any data during a POR is ignored
			RX1byte = RCREG2;					// copy received byte
			if (!RX1byte) Reset();				// Only reset if not charging...
		} else 	RX1byte = RCREG2;			

        RCONbits.POR= 1;						// flag that future resets are not POR resets
	
		TXREG2 = RX1byte;						// echo to UART2 port, don't check for overflow here.
		if (idx2 == 50) idx2--;
		if ((RX1byte== 0x08) && (idx2>0))
		{
			idx2--;	// backspace
		}
		else 
		{
			if (RX1byte==0x0d || RX1byte==0x0a)	// CR or LF?
			{
				RX1byte=0;
				ISR2FLAG=idx2+1;				// ENTER, process data
			}
			U2buffer[idx2++] = RX1byte;			// store byte
		}
	}

	while(PIR5bits.TMR4IF)				// Timer 4 interrupt, called 1000 times/sec
	{
		if (Lock)						// Cable lock enabled?
		{
			if (Error || (State != STATE_C))
			{
				if (unlocktimer<300)			// 300ms pulse		
				{
					SOLENOID_UNLOCK;
				}
				else SOLENOID_OFF;
				if (unlocktimer++ >400)
				{
					if (PORTCbits.RC1 == 0)		// still locked...
					{
						if (unlocktimer >5000) unlocktimer =0; 		//try to unlock again in 5 seconds
					}
					else unlocktimer =400;		
				}
				locktimer=0;
			}	
			else 
			{
				if (locktimer<300)				// 300ms pulse
				{
					 SOLENOID_LOCK;
				} 
				else SOLENOID_OFF;
				if (locktimer++ >400)
				{
					if (PORTCbits.RC1 == 1)		// still unlocked...
					{
						if (locktimer >5000) locktimer =0; 			//try to lock again in 5 seconds
					}
					else locktimer =400;		
				}
				unlocktimer=0;
			}
		}	

		Timer++;						// mSec counter (overflows in 1193 hours)
		PIR5bits.TMR4IF=0;				// clear interrupt flag
	}
	
}



// calculates 16-bit CRC of given data
// used for Frame Check Sequence on data frame
// Poly used is x^16+x^12+x^5+x
unsigned int calc_crc16(char* start, char len)
{
	unsigned int crc=0xffff,c;
	int i;
	while(len--)
	{
  		c=*start;
  		for(i=0;i<8;i++)
		{
   			if((crc ^ c) & 1) crc=(crc>>1)^0x8408;
   			else crc>>=1;
   			c>>=1;
  		}
  		start++;
 	}
	crc = (unsigned int)(crc ^ 0xFFFF);
 	return(crc);
}


// helper functions
unsigned char btohexa_high(unsigned char b)
{
    b >>= 4;
    return (b>0x9) ? b+'a'-10:b+'0';
}

unsigned char btohexa_low(unsigned char b)
{
    b &= 0x0F;
    return (b>9) ? b+'a'-10:b+'0';
}


void read_settings(void)
{
	char x;
	char *pBytes;
								  	// Read settings from EEprom
    pBytes = (char*)&MaxMains;
	EECON1 = 0; 					// select EEprom
	EEADR = 0;
  	for (x=0; x<12;x++)				// 12 bytes
    {
    	EECON1bits.RD = 1;
    	*pBytes++ = EEDATA;
    	EEADR++;
    }
}

void write_settings(void)
{
	char x;	
	char savint;
	char *pBytes;

	pBytes = (char*)&MaxMains;

    savint = INTCON; 			// Save interrupts state
    INTCONbits.GIE=0; 			// Disable interrupts

	
	for (x=0;x<12;x++)				// 12 bytes
    {
	    EEADR = x;
	    EECON1 = 0; 				//ensure CFGS=0 and EEPGD=0
	    EECON1bits.WREN = 1; 		//enable write to EEPROM
    	
		EEDATA = *pBytes++;			// set data
	    EECON2 = 0x55; 				// required sequence #1
	    EECON2 = 0xAA; 				// #2
	    EECON1bits.WR = 1; 			// #3 = actual write
	    while (EECON1bits.WR);
	    EECON1bits.WREN = 0; 		// disable write to EEPROM
    }

    INTCON = savint; 			// Restore interrupts
	printf((const far rom char *)"\r\ndata saved\r\n");

}


void _user_putc(unsigned char byte) // user defined printf support on uart2
{
	// output one byte on UART2
	while(!PIR3bits.TX2IF)			// set when register is empty 
		continue;
	TXREG2 = byte;

}

void hex_print(unsigned char byte)
{
	_user_putc(btohexa_high(byte));
	_user_putc(btohexa_low(byte));
}


unsigned char ReadPilot(void)				// Read Pilot Signal 
{
	ADCON0bits.GO=1;						// initiate ADC conversion on the selected channel
	while(ADCON0bits.GO);
	if (ADRES > 976) return PILOT_12V;						// Pilot at 12V
	if ((ADRES > 868) && (ADRES < 922))	return PILOT_9V;	// Pilot at 9V
	if ((ADRES > 761) && (ADRES < 805))	return PILOT_6V;	// Pilot at 6V
	if ((ADRES > 50) && (ADRES < 150))	return PILOT_DIODE;	// Diode Check OK (-12V)
	return PILOT_NOK;										// Pilot NOT ok
}

void ProximityPin(void)
{
	ADCON0 = 0b00000101;					// ADC input AN1 (Proximity Pin)
	ADCON2 = 0b10100101;					// Right justify, Tacq = 8 uS, FOSC/16
	ADCON0bits.GO=1;						// initiate ADC conversion on the selected channel
	while(ADCON0bits.GO);

	MaxCapacity=13;										// Max cable current = 13A
	if ((ADRES > 394) && (ADRES < 434))	MaxCapacity=20; // Max cable current = 20A
	if ((ADRES > 175) && (ADRES < 193)) MaxCapacity=32;	// Max cable current = 32A
	if ((ADRES > 88) && (ADRES < 98)) MaxCapacity=63;	// Max cable current = 63A

	ADCON0 = 0b00000001;					// ADC input AN0 (Pilot)
	ADCON2 = 0b10000101;					// Right justify, Tacq = 0 uS, FOSC/16
}


void SetCurrent(unsigned int current)		// current in Amps x10	(165= 16.5A)
{
	unsigned int DutyCycle;
	if ((current >= 60) && (current <= 510)) DutyCycle = current/0.6;
											// calculate DutyCycle from current
	else if ((current > 510) && (current <=800)) DutyCycle = (current / 2.5)+640;
	CCPR1L = DutyCycle >> 2;				// Msb of DutyCycle
											// 2 Lsb are part of CCP1CON, use Timer 2	
	CCP1CON =(((DutyCycle & 0x03) << 4) | 0x0C); 
											// PWM Pilot signal enabled
}


unsigned int CalcCurrent()					// calculates PWM current (Amps x10)
{
	Iset+= Ilimit - Imeasured;				// last PWM setting + difference between setpoint and measured value
    if(Iset> (MaxCurrent*10)) Iset=(MaxCurrent*10);		// limit to Max Current (set by user)
    if(Iset> (MaxCapacity*10)) Iset=(MaxCapacity*10);	// limit to Max cable Capacity
    if(Iset< 60) Iset= 60;				// minimal 6A charge
	return Iset;

}


// Serial Command line interface
// Display Menu, and process input.
//------------------------------------------------
// Smart EVSE
// -- Main menu --
// MODE  - Set to Smart mode, or Normal EVSE mode
// MAINS - Set max MAINS Current (25-100)
// MAX   - Set MAX Charge Current for the EV (16-80)
// MIN   - Set MIN Charge Current the EV will accept
// CAL   - Calibrate CT1
// LOCK  - Cable lock Enable/disable
// L1: 1.2A L2: 5.3A L3: 0.4A (MAX:26A MIN:10A)
//
void RS232cli(void)
{
	unsigned int n;
	double Inew,Iold;

	printf((const far rom char *)"\r\n");
	if (menu==0)		// menu = Main Menu
	{
		if (strcmppgm2ram(U2buffer,(const far rom char *)"MAINS") == 0) menu=1;		// Switch to Set Max Mains Capacity
		if (strcmppgm2ram(U2buffer,(const far rom char *)"MAX") == 0) menu=2;		// Switch to Set Max Current
		if (strcmppgm2ram(U2buffer,(const far rom char *)"MIN") == 0) menu=3;		// Switch to Set Min Current
		if (strcmppgm2ram(U2buffer,(const far rom char *)"CAL") == 0) menu=4;		// Switch to Calibrate CT1
		if (strcmppgm2ram(U2buffer,(const far rom char *)"MODE") == 0) menu=5;		// Switch to Normal or Smart mode
		if (strcmppgm2ram(U2buffer,(const far rom char *)"LOCK") == 0) menu=6;		// Switch to Enable/Disable Cable Lock
	}
	else if (U2buffer[0]==0) menu=0;
	else							// menu = 1,2,3,4 read entered value from cli
	{		
		if (menu==1 || menu==2 || menu==3)
		{
			n=(unsigned int)atob(U2buffer);
			if ((menu==1) && (n>24) && (n<101))
			{
				MaxMains=n;						// Set new MaxMains
				Ilimit=MaxMains*10;				// Update Ilimit
				write_settings();				// Write to eeprom
			}
			else if ((menu==2) && (n>15) && (n<81))
			{
				MaxCurrent=n;					// Set new MaxCurrent
				write_settings();				// Write to eeprom
			}
			else if ((menu==3) && (n>5) && (n<17))
			{
				MinCurrent=n;					// Set new MinCurrent
				write_settings();				// Write to eeprom
			}
			else printf((const far rom char *)"\r\nError! please check limits\r\n");
		}
		else if (menu==4)
		{
			Inew =atof(U2buffer);
			if ((Inew<10) || (Inew >80)) printf((const far rom char *)"\r\nError! please calibrate with atleast 10A\r\n");
			else
			{
				Iold=Irms[0]/ICal; 
				ICal=(Inew*10)/Iold;			// Calculate new Calibration value
				write_settings();				// Write to eeprom
			}
		}
		else if (menu==5)
		{
			if (strcmppgm2ram(U2buffer,(const far rom char *)"SMART") == 0)
			{
				Mode = 1;
				write_settings();				// Write to eeprom
			}
			else if (strcmppgm2ram(U2buffer,(const far rom char *)"NORMAL") == 0)
			{
				Mode = 0;
				write_settings();				// Write to eeprom
			}
			
		}
		else if (menu==6)
		{
			if (strcmppgm2ram(U2buffer,(const far rom char *)"ENABLE") == 0)
			{
				Lock = 1;
				write_settings();				// Write to eeprom
			}
			else if (strcmppgm2ram(U2buffer,(const far rom char *)"DISABLE") == 0)
			{
				Lock = 0;
				write_settings();				// Write to eeprom
			}
		}

		menu=0;
	}


	if (menu==0)
	{
		printf((const far rom char *)"\r\n---------------------- SMART EVSE  ----------------------\r\n");
		printf((const far rom char *)" v1.01  for detailed instructions, see www.smartevse.org\r\n");
		printf((const far rom char *)" Internal Temperature: %2u C\r\n",TempEVSE);
		printf((const far rom char *)"---------------------------------------------------------\r\n");
		printf((const far rom char *)"MODE  - Set to Smart mode, or Normal EVSE mode    (");
		if (Mode) printf((const far rom char *)"SMART)\r\n");
		else printf((const far rom char *)"NORMAL)\r\n");
		printf((const far rom char *)"MAINS - Set Max MAINS Current                     (%3u A)\r\n",MaxMains);
		printf((const far rom char *)"MAX   - Set MAX Charge Current for the EV         ( %2u A)\r\n",MaxCurrent);
		printf((const far rom char *)"MIN   - Set MIN Charge Current the EV will accept ( %2u A)\r\n",MinCurrent);
		printf((const far rom char *)"LOCK  - Cable lock Enable/Disable                 (");
		if (Lock) printf((const far rom char *)"ENABLE)\r\n");
		else printf((const far rom char *)"DISABLE)\r\n");
		printf((const far rom char *)"CAL   - Calibrate CT1  (CT1:%3u.%01uA CT2:%3u.%01uA CT3:%3u.%01uA)\r\n>",(unsigned int)Irms[0]/10, (unsigned int)Irms[0]%10, (unsigned int)Irms[1]/10, (unsigned int)Irms[1]%10, (unsigned int)Irms[2]/10, (unsigned int)Irms[2]%10);

	}
	else if (menu==1) 
	{
		printf((const far rom char *)"WARNING - DO NOT SET CURRENT HIGHER THAN YOUR CIRCUIT BREAKER\r\n"); 
        printf((const far rom char *)"OR GREATER THAN THE RATED VALUE OF THE EVSE\r\n");
		printf((const far rom char *)"MAINS Current set to: %u A\r\nEnter new max MAINS Current (25-100): ",MaxMains);
	}
	else if (menu==2) 
	{
		printf((const far rom char *)"WARNING - DO NOT SET CURRENT HIGHER THAN YOUR CIRCUIT BREAKER\r\n"); 
        printf((const far rom char *)"OR GREATER THAN THE RATED VALUE OF THE EVSE\r\n");
		printf((const far rom char *)"MAX Current set to: %u A\r\nEnter new MAX Charge Current (16-80): ",MaxCurrent);
	}

	else if (menu==3)
	{
		printf((const far rom char *)"MIN Charge Current set to: %u A\r\nEnter new MIN Charge Current (6-16): ",MinCurrent);
	}
	else if (menu==4)
	{
		printf((const far rom char *)"CT1 reads: %3u.%01u A\r\nEnter new Measured Current for CT1: ",(unsigned int)Irms[0]/10, (unsigned int)Irms[0]%10);
	}
	else if (menu==5)
	{
		printf((const far rom char *)"EVSE set to : ");
		if (Mode) printf((const far rom char *)"Smart mode\r\n");
		else printf((const far rom char *)"Normal mode\r\n");
		printf((const far rom char *)"Enter new EVSE Mode (SMART/NORMAL): ");
	}
	else if (menu==6)
	{
		printf((const far rom char *)"Cable lock : ");
		if (Lock) printf((const far rom char *)"Enabled\r\n");
		else printf((const far rom char *)"Disabled\r\n");
		printf((const far rom char *)"Enter new Cable lock mode (ENABLE/DISABLE): ");
	}


	ISR2FLAG=0;				// clear flag
	idx2=0;					// reset buffer pointer

}


void delay(unsigned int d)
{
	unsigned long x;
	x=Timer;							// read Timer value (increased every ms)
	while (Timer < (x+d)) { }
}

void LCD_write(unsigned char c)
{
	unsigned char x,y;
	x= PORTB & 0xf0;
	PORTB=x | (0x0f & (c >>4));
	PORTCbits.RC5 = 1;					// LCD Enable 1
	PORTCbits.RC5 = 0;					// LCD Enable 0
	delay(1);
	PORTB=x | (0x0f & c);
	PORTCbits.RC5 = 1;					// LCD Enable 1
	PORTCbits.RC5 = 0;					// LCD Enable 0
	delay(1);
}	


void LCD_print(const far rom char *data )	// write string of data to LCD
{
	PORTCbits.RC3 = 1;					// LCD RS 
	do
  	{
    	LCD_write(*data);
  	} while (*++data);
	PORTCbits.RC3 = 0;					// LCD RS
}


void init_lcd(void)						// initialize the LCD
{
	unsigned char x;
	delay(50);							// wait 50ms

	PORTCbits.RC0 = 0;					// LCD backlight off

	PORTCbits.RC5 = 0;					// LCD Enable 0
	PORTCbits.RC4 = 0;					// LCD R/W
	PORTCbits.RC3 = 0;					// LCD RS

	x=PORTB & 0xf0;
	PORTB=x |(0x0f & 0x3);
	PORTCbits.RC5 = 1;					// LCD Enable 1
	PORTCbits.RC5 = 0;					// LCD Enable 0
	delay(5);							// wait 5ms

	PORTCbits.RC5 = 1;					// LCD Enable 1
	PORTCbits.RC5 = 0;					// LCD Enable 0
	delay(1);							// wait 1ms

	PORTCbits.RC5 = 1;					// LCD Enable 1
	PORTCbits.RC5 = 0;					// LCD Enable 0
	delay(1);							// wait 1ms

	PORTB=x |(0x0f & 0x2);
	PORTCbits.RC5 = 1;					// LCD Enable 1
	PORTCbits.RC5 = 0;					// LCD Enable 0
	delay(1);							// wait 1ms

	LCD_write(0x28);
	LCD_write(0x08);					// display off
	LCD_write(0x01);					// display clear
	delay(5);
	LCD_write(0x06);					// entry mode set
//	LCD_write(0x0D);					// display on/ blinking on
	LCD_write(0x0C);					// display on/ blinking off
}


void Temp(void)								// Measure Temperature EVSE (0-125 C)
{
	unsigned char temp;

	ADCON0 = 0b00001001;					// ADC input AN2 (Temperature Sensor)
	ADCON2 = 0b00111101;					// Left justify, Tacq = 20 uS, FOSC/16
	ADCON0bits.GO=1;						// initiate ADC conversion on the selected channel
	while(ADCON0bits.GO);

	temp = ADRESH;

	ADCON0 = 0b00000001;					// ADC input AN0 (Pilot)
	ADCON2 = 0b10000101;					// Right justify, Tacq = 0 uS, FOSC/16

	if (temp <21) TempEVSE=0;				// set to 0 deg C
	else TempEVSE = temp-21;				// set Temp (1-125 deg C)
}



// called once a second
void LCD(void)
{
		unsigned char x;
		if (LCDTimer==10) LCDTimer=0;

		if (Error)
		{
			PORTCbits.RC0 = 1;					// LCD backlight on

			if (Error==LESS_6A)
			{
				LCD_write(0x80);				// address 00H / first row
				LCD_print((const far rom char *)"ERROR NO");
				LCD_write(0xC0);				// address 40H / second row
				LCD_print((const far rom char *)"CURRENT!");
			}
			else if (Error==CT_NOCOMM)
			{
				LCD_write(0x80);				// address 00H / first row
				LCD_print((const far rom char *)"ERROR NO");
				LCD_write(0xC0);				// address 40H / second row
				LCD_print((const far rom char *)"SER.COMM");
			}
			else if (Error==OVERCURRENT)
			{
				LCD_write(0x80);				// address 00H / first row
				LCD_print((const far rom char *)"ERROR   ");
				LCD_write(0xC0);				// address 40H / second row
				LCD_print((const far rom char *)"OVERCURR");
			}
			else if (Error==TEMP_HIGH)
			{
				LCD_write(0x80);				// address 00H / first row
				LCD_print((const far rom char *)"ERROR   ");
				LCD_write(0xC0);				// address 40H / second row
				LCD_print((const far rom char *)"HIGHTEMP");
			}
			return;
		}

		if ((LCDTimer++>4) && Mode)			
		{
			LCD_write(0x80);				// address 00H / first row
			LCD_print((const far rom char *)"L1 L2 L3");
			LCD_write(0xC0);				// address 40H / second row
			PORTCbits.RC3 = 1;					// LCD RS 
			for (x=0; x<3 ;x++)
			{
				LCD_write( (unsigned char)(Irms[x]/100)+0x30 );
				LCD_write( (unsigned char)((unsigned int)(Irms[x]/10)%10)+0x30 );
				LCD_write(' ');
			}
			PORTCbits.RC3 = 0;					// LCD RS 

		}
		else if ((State == STATE_A) || (State == STATE_B))
		{
			PORTCbits.RC0 = 0;					// LCD backlight off

			LCD_write(0x80);				// address 00H / first row
			LCD_print((const far rom char *)"READY TO");
			LCD_write(0xC0);				// address 40H / second row
			LCD_print((const far rom char *)"CHARGE  ");
		}
		else if (State == STATE_C)
		{
			PORTCbits.RC0 = 1;					// LCD backlight on

			LCD_write(0x80);				// address 00H / first row
			LCD_print((const far rom char *)"CHARGING");
			LCD_write(0xC0);				// address 40H / second row
			PORTCbits.RC3 = 1;					// LCD RS 
			LCD_write( (unsigned char)(Iset/100)+0x30 );
			LCD_write( (unsigned char)((Iset/10)%10)+0x30 );
			LCD_write('A');
			LCD_write('(');
			LCD_write( (unsigned char)(Ilimit/100)+0x30 );
			LCD_write( (unsigned char)((Ilimit/10)%10)+0x30 );
			LCD_write('A');
			LCD_write(')');
			PORTCbits.RC3 = 0;	
		}

}


void init(void)
{
	OSCCON = 0b01101100;	// setup external oscillator
	OSCCON2 = 0b00000100;	// primary Oscillator On.

	RCON = 0b10011111;		// Set Interrupt priority 

	PMD0 = 0b00000000;		// Perhiperal Module Enable/Disable
	PMD1 = 0b00000000;		// All enabled
	PMD2 = 0b00000000;

	PORTA = 0; 				// Init PORTA
	ANSELA = 0b00000111;	// RA0, RA1, RA2 are analog inputs (pin 2,3,4)
	TRISA = 0b00000111;		// Set RA<2:0> as inputs
//	ANSELA = 0b00000011;	// RA0, RA1 are analog inputs (pin 2,3)
//	TRISA = 0b00000011;		// Set RA<1:0> as inputs

	PORTB = 0;
	ANSELB = 0;				// All digital IO
	TRISB = 0b10000000;		// RB7 input (RX2), all other output
	WPUB = 0x80;			// weak pullup on RB7
	INTCON2bits.RBPU = 0;	// Enable weak pullups on PORTB

	PORTC = 0;
	ANSELC = 0;				// All digital IO
	TRISC = 0b10000010;		// RC1 and RC7 input (RX1), all other output

	SPBRGH1 = 13;			// Initialize UART 1 (RS485)
	SPBRG1 = 4;				// Baudrate 1200 
	BAUDCON1 = 0b00001000;	// 16 bit Baudrate register is used
	TXSTA1 = 0b00100100;	// Enable TX, 8 bit, Asynchronous mode
	RCSTA1 = 0b10010000;	// Enable serial port TX and RX, 8 bit. 

	SPBRGH2 = 0;			// Initialize UART 2
	SPBRG2 = 34;			// Baudrate 115k2 (114285)
//	SPBRG2 = 207;			// Baudrate 19k2
	BAUDCON2 = 0b00001000;	// 16 bit Baudrate register is used
	TXSTA2 = 0b00100100;	// Enable TX, 8 bit, Asynchronous mode
	RCSTA2 = 0b10010000;	// Enable serial port TX and RX, 8 bit. 


	ADCON0 = 0b00000001;	// ADC On input AN0 (Pilot)
	ADCON1 = 0;
	ADCON2 = 0b10000101;	// Right justify, Tacq = 0 uS, FOSC/16

	T0CON = 0b10000111;		// Timer 0 @ 16Mhz => 32 uS Timer0, 16 bit counter, 1:256 prescaler

	PR2 = 249;				// Timer 2 frequency value -> 1Khz @ 16 Mhz
	T2CON = 0b00000110;		// Timer 2 ON, prescaler 1:16
	CCP1CON = 0;			// PWM off

	PR4 = 249;				// Timer 4 frequency value -> 1Khz @ 16 Mhz
	T4CON = 0b00000110;		// Timer 4 ON, prescaler 1:16


	
	stdout = _H_USER;    	// Uart2 for printf

	PIE1bits.RC1IE = 1;		// enable receive Interrupt for UART1
	PIE3bits.RC2IE = 1;		// enable receive Interrupt for UART2
	PIE5bits.TMR4IE = 1;	// enable Timer4 Interrupt


	INTCONbits.GIEH = 1;	// global High Priority interrupts enabled
	INTCONbits.GIEL = 0;	// global Low Priority interrupts disabled

	Iset=MaxCurrent*10;		// initially set to MaxCurrent
	Ilimit=MaxMains*10;		// initially set to MaxMains

	printf((const far rom char *)"\r\nSmart EVSE powerup.\r\n");
}

void main(void)
{
	char *pBytes;
	char x,n;
	unsigned char pilot,count,timeout=5;
	unsigned int timer=0; 
	char DiodeCheck=0;


	init();									// initialize ports, ADC, UARTs etc

	init_lcd();

	read_settings();						// from EEprom
	
	while(1)								// MAIN loop
	{

		if (ISR2FLAG) RS232cli();			// RS232 command line interface

		
		if (State == STATE_A)				// ############### EVSE State A #################
		{
			CCP1CON = 0;					// PWM off
			PORTCbits.RC2 = 1;				// Control pilot static +12V
			CONTACTOR_OFF;					// Contactor OFF
			Timer =0;						// reset charge Timer


			if ((Imeasured <= ((MaxMains-MinCurrent)*10)) && ((Error ==LESS_6A) || (Error==OVERCURRENT)) )
			{	
				Error=NO_ERROR;
			}
		
			pilot = ReadPilot();
			if (pilot == PILOT_9V)			// State B ?
			{
				if (NextState == STATE_B)
				{
					count++;							// repeat 5 times
					if (count == 5)
					{
						if (Imeasured > ((MaxMains-MinCurrent)*10))
						{
							if (Error == NO_ERROR) printf((const far rom char *)"Not enough current available!\r\n");
							Error = LESS_6A;
						}	 
						if (Error == NO_ERROR)
						{
							DiodeCheck=0;
							State = STATE_B;				// switch to State B
							printf((const far rom char *)"STATE A->B\r\n");
						}
					}	
				}
				else 
				{
					NextState=STATE_B;
					count=0;
				}		
				
			}		
		}	

		if (State == STATE_B)				// ############### EVSE State B #################
		{
											// measure voltage at ~5% and ~90% of PWM cycle
			if ((TMR2 > 7) && (TMR2 < 24))	// PWM cycle 3% - 9% (should be high)
			{
				pilot = ReadPilot();
				if (pilot == PILOT_12V)			// Disconnected?
				{
					if (NextState == STATE_A)
					{
						count++;							// repeat 5 times
						if (count == 5)	
						{
							State = STATE_A;				// switch to STATE_A
							printf((const far rom char *)"STATE B->A\r\n");
						}
					}
					else 
					{
						NextState=STATE_A;
						count=0;
					}
				}
				else if (pilot == PILOT_6V)
				{
					if ((NextState == STATE_C) && (DiodeCheck==1))
					{
						count++;							// repeat 5 times
						if (count == 5) 
						{
							ProximityPin();					// Sample Proximity Pin
  
							printf((const far rom char *)"Cable limit: %uA  Max: %uA \r\n",MaxCapacity, MaxCurrent);

							CONTACTOR_ON;					// Contactor ON
							DiodeCheck=0;
							State = STATE_C;				// switch to STATE_C
							LCDTimer=0;
							LCD();							// immediately update LCD
							printf((const far rom char *)"STATE B->C\r\n");
						}
					}
					else 
					{
						NextState=STATE_C;
						count=0;
					}
				}
				else	// PILOT_9V
				{
					if (NextState == STATE_B)				// Did the EV switch from State_C to State_B?
					{										// then there was probably not enough current available
															// or the charging was finished.
						if ((Imeasured > ((MaxMains-MinCurrent)*10)) && (timer <= 50))// Less then minimum current available?	
						{									// skip first 10 seconds, so that we have a good reading of actual current.
							Ilimit = 60;					// Set to 6A, to prevent the EV from starting to charge
															// this will give a "Charge error" on a Tesla Roadster
							timer =60;						// reset timeout to 60 seconds
						}
						else if (timer == 0)				// We had enough current for atleat 60 secs
						{
							Ilimit = MaxMains*10;			// set the charge limit to MaxMains
						}
					}
					else NextState=0;						// no State to switch to
				}
			}
			if (TMR2 > 224) 				// PWM cycle > 90% (sould be low if current < 65A)
			{
				pilot = ReadPilot();
				if (pilot == PILOT_DIODE) DiodeCheck=1;
				else DiodeCheck=0;
			}
		}							

		if (State == STATE_C)				// ############### EVSE State C #################
		{
											// measure voltage at ~5% of PWM cycle
			if ((TMR2 > 7) && (TMR2 < 24))	// cycle 3% - 9% (sould be high)
			{
				pilot = ReadPilot();
				if ((pilot == PILOT_12V) || (pilot == PILOT_NOK))	// Disconnected or Error?
				{
					if (NextState == STATE_A)
					{
						count++;							// repeat 5 times
						if (count == 5)	
						{
							State = STATE_A;	// switch back to STATE_A
							printf((const far rom char *)"STATE C->A\r\n");
						}
					}
					else 
					{
						NextState=STATE_A;
						count=0;
					}
				}
				else if (pilot == PILOT_9V)
				{
					if (NextState == STATE_B) 
					{
						count++;							// repeat 5 times
						if (count == 5) 
						{
							CONTACTOR_OFF;					// Contactor OFF
							DiodeCheck=0;
							State = STATE_B;				// switch back to STATE_B
							timer=60;						// stay atleast 60 seconds in STATE B
							printf((const far rom char *)"STATE C->B\r\n");
						}
					}
					else 
					{
						NextState=STATE_B;
						count=0;
					}
				}
				else	// PILOT_6V
				{
					NextState=0;							// no State to switch to					
				
					// Calculate maximum charge current, based on:
					// - Charging cable capacity (MaxCapacity)
					// - Maximal current per phase (MaxMains)
					// - momentary measured current (Imeasured)
	
					if (Imeasured > (MaxMains*20))				// Overcurrent?
					{
						Error = OVERCURRENT;
						State = STATE_A;						// ERROR, switch back to STATE_A
						printf((const far rom char *)"Overcurrent!\r\n");
					}
					if (TempEVSE >=60)							// Temperature too High?
					{
						Error = TEMP_HIGH;
						State = STATE_A;						// ERROR, switch back to STATE_A
						printf((const far rom char *)"Temperature too High!\r\n");
					}

				}
			}
		}												// end of State C code

	

		if (RCSTA1bits.OERR)			// Uart1 Overrun Error?
		{
			RCSTA1bits.CREN = 0;
			RCSTA1bits.CREN = 1;		// Restart Uart
		}
		if (RCSTA2bits.OERR)			// Uart2 Overrun Error?
		{
			RCSTA2bits.CREN = 0;
			RCSTA2bits.CREN = 1;		// Restart Uart
		}

		x= TMR0L;
		if (TMR0H >= 0x3d )				// 1 second timer
		{
			TMR0H=0;
			TMR0L=0;
			if ((timer>0) && (Ilimit==60))
			{
				printf((const far rom char *)"timer:%2u\r",timer);
				timer --;
			}

			if ((TempEVSE <55) && (Error == TEMP_HIGH))		// Temperature below limit?
			{
					Error=NO_ERROR;								// clear Error
			}
				
			if ((timeout == 0) && (Error == NO_ERROR)) 	// timout if CT current measurement takes > 10 secs
			{
				Error = CT_NOCOMM;
				State = STATE_A;						// switch back to state A
				printf((const far rom char *)"Error, no communication with CT's!\r\n");
			}
			else if (timeout>0) timeout--;

			LCD();										// once a second, update LCD
			Temp();										// once a second, measure temperature

			if ((State==STATE_B) || (State==STATE_C)) SetCurrent(CalcCurrent());	// Calculate charge current, and set PWM output		
		}



/*  RS485 serial data is received by the ISR routine, and processed here..
	Reads serial packet with Raw Current values, measured from 1-N CT's, over a RS485 serial line
	baudrate is 1200 bps
 	
	<protocol>,<version>,<nr of samples>,<sample 1>...<sample n>,<crc16>
    protocol = 0x5001 (2 bytes)
	version = 1 (1 byte)
	nr of samples = 1 byte
	samples are 4 bytes (double)
	crc16 is 2 bytes
	total 4+(n*4)+2 bytes (+ HDLC overhead)
*/

		if (ISRFLAG > 1) 	// complete packet detected?
		{
			crc16 = calc_crc16(U1buffer, ISRFLAG);

            if (ISRFLAG>10 && U1buffer[2]==0x50 && U1buffer[3]==0x01 && crc16==GOODFCS16) // check CRC
            {
				n=6;
				Imeasured=0;									// reset Imeasured value
                for (x=0; x<U1buffer[5]; x++)					// Nr of CTs    
				{        
	                pBytes = (char*)&Irms[x];
	                *pBytes++=U1buffer[n++];
	                *pBytes++=U1buffer[n++];
	                *pBytes++=U1buffer[n++];
	                *pBytes=U1buffer[n++];
	                Irms[x]= Irms[x]*ICal;						// adjust CT values with Calibration value
					if (Irms[x] > Imeasured) Imeasured=Irms[x];	// Imeasured has highest Irms of all channels
				}

#if defined(DEBUG)
   			printf((const far rom char *)"CT1:%3u.%01uA CT2:%3u.%01uA CT3:%3u.%01uA Imeas:%3u.%01uA Ipwm:%u.%01uA\r\n",(unsigned int)Irms[0]/10, (unsigned int)Irms[0]%10, (unsigned int)Irms[1]/10, (unsigned int)Irms[1]%10, (unsigned int)Irms[2]/10, (unsigned int)Irms[2]%10,(unsigned int)Imeasured/10,(unsigned int)Imeasured%10,(unsigned int)Iset/10,(unsigned int)Iset%10);
#endif
			}
			ISRFLAG=0;										// ready to receive new data
			if (Error == CT_NOCOMM) Error=NO_ERROR;			// Clear communication error, if present
			timeout=10;										// 10 second timeout for CT data
		}

		if (!Mode)				// If Normal Mode for the EVSE is selected, we'll have to set some fixed values..
		{
			timeout=10;
			Imeasured=0;
		}

	} // end of while(1) loop
}
