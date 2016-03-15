/*
;    Project:       Smart EVSE
;    Date:          15 March 2016
;
;    Changes:
;    1.00  Initial release
;    1.01  Added Normal EVSE mode + Cable Lock support
;    1.02  Changed Max Capacity to 16A for 2,5mm2 cables
;    1.03  Changed Max Charge current range to 10-80A, also changed LCD delay to 2ms
;    1.04  Changed menu so that only relevant options are shown. Added option for fixed cable installations. (no Cable Lock, override PP pin)
;    1.05  Added LCD menu support. Most options can now be set directly on the module itself. Fixed charge current > 65A.
;	 1.06  Added CT1 calibration to the LCD menu. Improved Smartmode current calculation and regulation.
;
;    (C) 2013-2016  Michael Stegen / Stegen Electronics
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
#include <usart.h>
#include <stdlib.h>
#include <math.h>
#include "EVSE.h"

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



#pragma romdata eedata=0xf00000
rom unsigned int EE_MaxMains = MAX_MAINS;
rom unsigned int EE_MaxCurrent = MAX_CURRENT;
rom unsigned int EE_MinCurrent = MIN_CURRENT;
rom double EE_ICal = ICAL;
rom char EE_Mode = MODE;
rom char EE_Lock = LOCK;
rom unsigned int EE_CableLimit = CABLE_LIMIT;
rom char EE_Config = CONFIG;
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
unsigned int CableLimit;			// Fixed Cable Current limit (only used when config is set to Fixed Cable)
char Config;						// Configuration (Fixed Cable or Type 2 Socket)

// total 15 bytes

double Irms[4]={0,0,0,0};			// Momentary current per Phase (Amps *10) (23= 2.3A)
									// Max 4 phases supported

unsigned int crc16;
unsigned char State = STATE_A;
unsigned char Error = NO_ERROR;
unsigned char NextState;

unsigned int MaxCapacity;			// Cable limit (Amps)(limited by the wire in the charge cable, set automatically, or manually if Config=Fixed Cable)
unsigned int Imeasured=0;			// Max of all CT inputs (Amps *10)
int Iset=0;							// PWM output is set to this current level (Amps *10)

unsigned char RX1byte;
unsigned char idx=0,idx2=0,ISRFLAG=0,ISR2FLAG=0;
unsigned char menu=0;
unsigned int locktimer=0,unlocktimer=0;	// solenoid timers
unsigned long Timer=0;				// mS counter
unsigned int ChargeTimer=0;			// Counts seconds in STATE C (Charging) (unused)
unsigned char LCDTimer=0;
unsigned char TempEVSE=0;			// Temperature EVSE in deg C (0-125)
unsigned char ButtonState=0x0f;		// Holds latest push Buttons state (LSB 3:0)
unsigned char OldButtonState=0x0f;	// Holds previous push Buttons state (LSB 3:0)
unsigned char LCDNav=0;
unsigned char SubMenu=0;
unsigned long ScrollTimer=0;
unsigned char LCDpos=8;
unsigned char ChargeDelay=0;		// Delays charging up to 60 seconds in case of not enough current avilable.

const far rom char MenuConfig[] = "CONFIG - Set to Fixed Cable or Type 2 Socket";
const far rom char MenuMode[] 	= "MODE   - Set to Smart mode or Normal EVSE mode";
const far rom char MenuMains[] 	= "MAINS  - Set Max MAINS Current";
const far rom char MenuMax[] 	= "MAX    - Set MAX Charge Current for the EV";
const far rom char MenuMin[] 	= "MIN    - Set MIN Charge Current the EV will accept";
const far rom char MenuCable[] 	= "CABLE  - Set Fixed Cable Current limit";
const far rom char MenuLock[] 	= "LOCK   - Cable lock Enable/Disable";
const far rom char MenuCal[]	= "CAL    - Calibrate CT1 (CT2+3 will also change)";



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
// DEBUG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   
		if(RCSTA2bits.FERR && (State == STATE_A) && RCONbits.POR)
//		if(RCSTA2bits.FERR && RCONbits.POR)
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

		TRISB = 0b10001111;				// Set PortB to read 4 button pins.
		Nop(); 
		ButtonState=(PORTB & 0x07);		// Read the state of the buttons, only three buttons are used right now.
		TRISB = 0b10000000;				// Set PortB back to outputs

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
  	for (x=0; x<15;x++)				// 15 bytes
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

	
	for (x=0;x<15;x++)				// 15 bytes
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
	printf((const far rom char *)"\r\nsettings saved\r\n");

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
	if ((ADRES > 394) && (ADRES < 434))	MaxCapacity=16; // Max cable current = 16A
	if ((ADRES > 175) && (ADRES < 193)) MaxCapacity=32;	// Max cable current = 32A
	if ((ADRES > 88) && (ADRES < 98)) MaxCapacity=63;	// Max cable current = 63A

	if (Config) MaxCapacity = CableLimit;				// Override when Fixed Cable is used.  

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
	int Idifference;

	Idifference = (MaxMains*10) - Imeasured;			// Difference between MaxMains and Measured current (can be negative)

	if (Idifference>0) Iset+=(Idifference/2);			// increase with half of difference (slowly increase current)
	else Iset+= Idifference;							// last PWM setting + difference (immediately decrease current)

	if((Iset< 0) || (Iset<60) ) Iset=60;				// minimal 6A charge
    if(Iset> (MaxCurrent*10)) Iset=(MaxCurrent*10);		// limit to Max Current (set by user)
    if(Iset> (MaxCapacity*10)) Iset=(MaxCapacity*10);	// limit to Max cable Capacity
	return Iset;
}


// Serial Command line interface
// Display Menu, and process input.
//------------------------------------------------
// Smart EVSE
// -- Main menu --
// CONFIG- Set to Fixed Cable or Type 2 Socket
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
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"MAINS") == 0) && Mode) menu=1;		// Switch to Set Max Mains Capacity (Smart mode)
		if (strcmppgm2ram(U2buffer,(const far rom char *)"MAX") == 0) menu=2;					// Switch to Set Max Current
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"MIN") == 0) && Mode) menu=3;			// Switch to Set Min Current (Smart mode)
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"CAL") == 0) && Mode) menu=4;			// Switch to Calibrate CT1 (Smart mode)
		if (strcmppgm2ram(U2buffer,(const far rom char *)"MODE") == 0) menu=5;					// Switch to Normal or Smart mode
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"LOCK") == 0) && !Config) menu=6;		// Switch to Enable/Disable Cable Lock (Config=Socket)
		if (strcmppgm2ram(U2buffer,(const far rom char *)"CONFIG") == 0) menu=7;				// Switch to Fixed cable or Type 2 Socket
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"CABLE") == 0) && Config) menu=8;		// Switch to Set fixed Cable Current limit (Config=Fixed)
	}
	else if (U2buffer[0]==0) menu=0;
	else							// menu = 1,2,3,4 read entered value from cli
	{		
		if (menu==1 || menu==2 || menu==3 || menu==8)
		{
			n=(unsigned int)atob(U2buffer);
			if ((menu==1) && (n>24) && (n<101))
			{
				MaxMains=n;						// Set new MaxMains
				write_settings();				// Write to eeprom
			}
			else if ((menu==2) && (n>9) && (n<81))
			{
				MaxCurrent=n;					// Set new MaxCurrent
				write_settings();				// Write to eeprom
			}
			else if ((menu==3) && (n>5) && (n<17))
			{
				MinCurrent=n;					// Set new MinCurrent
				write_settings();				// Write to eeprom
			}
			else if ((menu==8) && (n>12) && (n<81))
			{
				CableLimit=n;					// Set new CableLimit
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
		else if (menu==5)					// EVSE Mode
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
				Error=NO_ERROR;					// Clear Errors
			}
			
		}
		else if (menu==6)					// Cable Lock
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
		else if (menu==7)					// Configuration Mode
		{
			if (strcmppgm2ram(U2buffer,(const far rom char *)"FIXED") == 0)
			{
				Config = 1;
				write_settings();				// Write to eeprom
			}
			else if (strcmppgm2ram(U2buffer,(const far rom char *)"SOCKET") == 0)
			{
				Config = 0;
				write_settings();				// Write to eeprom
			}
		}


		menu=0;
	}


	if (menu==0)
	{
		printf((const far rom char *)"\r\n---------------------- SMART EVSE  ----------------------\r\n v");
		printf((const far rom char *)VERSION);
		printf((const far rom char *)" for detailed instructions, see www.smartevse.org\r\n");
		printf((const far rom char *)" Internal Temperature: %2u C\r\n",TempEVSE);
		printf((const far rom char *)"---------------------------------------------------------\r\n");
		//printf((const far rom char *)"CONFIG - Set to Fixed Cable or Type 2 Socket       (");
		printf(MenuConfig);	printf((const far rom char *)"       (");
		if (Config) printf((const far rom char *)"Fixed Cable)\r\n");
		else printf((const far rom char *)"Type 2 Socket)\r\n");
		//printf((const far rom char *)"MODE   - Set to Smart mode or Normal EVSE mode     (");
		printf(MenuMode); printf((const far rom char *)"     (");
		if (Mode) printf((const far rom char *)"Smart)\r\n");
		else printf((const far rom char *)"Normal)\r\n");
		if (Mode) 
		{
			//printf((const far rom char *)"MAINS  - Set Max MAINS Current (Smart mode)        (%3u A)\r\n",MaxMains);
			printf(MenuMains); printf((const far rom char *)" (Smart mode)        (%3u A)\r\n",MaxMains);
		}
		//printf((const far rom char *)"MAX    - Set MAX Charge Current for the EV         ( %2u A)\r\n",MaxCurrent);
		printf(MenuMax); printf((const far rom char *)"         ( %2u A)\r\n",MaxCurrent);
		if (Mode)
		{
		 	//printf((const far rom char *)"MIN    - Set MIN Charge Current the EV will accept ( %2u A)\r\n",MinCurrent);
			printf(MenuMin); printf((const far rom char *)" ( %2u A)\r\n",MinCurrent);
		}	
		if (Config)
		{
			//printf((const far rom char *)"CABLE  - Set Fixed Cable Current limit             ( %2u A)\r\n",CableLimit);
			printf(MenuCable); printf((const far rom char *)"             ( %2u A)\r\n",CableLimit);
		} else {
			//printf((const far rom char *)"LOCK   - Cable lock Enable/Disable                 (");
			printf(MenuLock); printf((const far rom char *)"                 (");
			if (Lock) printf((const far rom char *)"Enabled)\r\n");
			else printf((const far rom char *)"Disabled)\r\n");
		}
		if (Mode) printf((const far rom char *)"CAL    - Calibrate CT1  (CT1:%3u.%01uA CT2:%3u.%01uA CT3:%3u.%01uA)\r\n",(unsigned int)Irms[0]/10, (unsigned int)Irms[0]%10, (unsigned int)Irms[1]/10, (unsigned int)Irms[1]%10, (unsigned int)Irms[2]/10, (unsigned int)Irms[2]%10);
		printf((const far rom char *)">");
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
		printf((const far rom char *)"MAX Current set to: %u A\r\nEnter new MAX Charge Current (10-80): ",MaxCurrent);
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
	else if (menu==7)
	{
		printf((const far rom char *)"Configuration : ");
		if (Config) printf((const far rom char *)"Fixed Cable\r\n");
		else printf((const far rom char *)"Type 2 Socket\r\n");
		printf((const far rom char *)"Enter new Configuration (FIXED/SOCKET): ");
	}
	else if (menu==8)
	{
		printf((const far rom char *)"WARNING - DO NOT SET CURRENT HIGHER THAN YOUR CIRCUIT BREAKER\r\n"); 
        printf((const far rom char *)"OR GREATER THAN THE RATED VALUE OF THE CHARGING CABLE\r\n");
		printf((const far rom char *)"Fixed Cable Current limit set to: %u A\r\nEnter new limit (13-80): ",CableLimit);
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
	WPUB = 0x87;			// weak pullup on RB7, and RB<2:0>
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


	printf((const far rom char *)"\r\nSmart EVSE powerup.\r\n");
}

void main(void)
{
	char *pBytes;
	char x,n;
	unsigned char pilot,count,timeout=5;
	unsigned int i; 
	char DiodeCheck=0;

	init();									// initialize ports, ADC, UARTs etc

	delay(500);								// wait 0,5 sec
	LCD_init();

	read_settings();						// from EEprom
	Iset=MaxCurrent*10;		// initially set to MaxCurrent


	delay(2000);							// wait 2 sec
	
	while(1)								// MAIN loop
	{

		if (ISR2FLAG) RS232cli();			// RS232 command line interface

		x = ButtonState;
		if ( (x != 0x07) || (x != OldButtonState) ) LCDMenu(x);		// Any button pressed or just released?

		if ( LCDNav && (ScrollTimer+5000<Timer) && (!SubMenu) )  LCDHelp();		// Update/Show Helpmenu


		if (State == STATE_A)				// ############### EVSE State A #################
		{
			CCP1CON = 0;					// PWM off
			PORTCbits.RC2 = 1;				// Control pilot static +12V
			CONTACTOR_OFF;					// Contactor OFF
			
			if ((Imeasured <= ((MaxMains-MinCurrent)*10)) && ((Error ==LESS_6A) || (Error==OVERCURRENT)) )
			{	
				Error=NO_ERROR;
			}

			pilot = ReadPilot();
			if (pilot == PILOT_9V)			// switch to State B ?
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
							ChargeDelay=60;
						}
						if ((Error == NO_ERROR) && (ChargeDelay ==0))
						{
							DiodeCheck=0;
							State = STATE_B;				// switch to State B
							printf((const far rom char *)"STATE A->B\r\n");
							ProximityPin();					// Sample Proximity Pin
  							printf((const far rom char *)"Cable limit: %uA  Max: %uA \r\n",MaxCapacity, MaxCurrent);
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
							if (Imeasured > ((MaxMains-MinCurrent)*10))
							{
								if (Error == NO_ERROR) printf((const far rom char *)"Not enough current available!\r\n");
								Error = LESS_6A;
								State = STATE_A;
								ChargeDelay=60;
							}
							if ( (Error == NO_ERROR) && (ChargeDelay==0))
							{
								CONTACTOR_ON;					// Contactor ON
								DiodeCheck=0;
								State = STATE_C;				// switch to STATE_C
								LCDTimer=0;
								Timer =0;						// reset msTimer and ChargeTimer
								if (!LCDNav)					// Don't update the LCD if we are navigating the menu
								{
									LCD_init();					// re-init LCD
									LCD();						// immediately update LCD
								}
								printf((const far rom char *)"STATE B->C\r\n");
							}
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
						if ((Imeasured > ((MaxMains-MinCurrent)*10)) )
						{									// Not enought current avilable, go to state A
							Error = LESS_6A;
							State = STATE_A;
							ChargeDelay=60;
						}
					}
					else NextState=0;						// no State to switch to
				}
			}
			if (TMR2 > 230)									// PWM > 92%
			{
				while (TMR2 < 242);							// wait till TMR2 is in range, otherwise we'll miss it (blocking)
				if ((TMR2 > 241) && (TMR2 < 249)); 			// PWM cycle >= 96% (should be low)
				{
					pilot = ReadPilot();
					if (pilot == PILOT_DIODE) DiodeCheck=1;	// Diode found, OK
					else DiodeCheck=0;
				}
			}
		}							

		if (State == STATE_C)				// ############### EVSE State C #################
		{
											// measure voltage at ~5% of PWM cycle
			if ((TMR2 > 7) && (TMR2 < 24))	// cycle 3% - 9% (should be high)
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
						ChargeDelay=60;												
						printf((const far rom char *)"Overcurrent!\r\n");
					}
					else if ( (Imeasured > (MaxMains*10)) && (Iset<=(MinCurrent*10)) )
					{
						Error = LESS_6A;
						State = STATE_A;						// ERROR, switch back to STATE_A
						ChargeDelay=60;												
						printf((const far rom char *)"Not enough current available!\r\n");
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

			if (ChargeDelay>0) ChargeDelay --;				// Decrease Charge Delay counter

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

			if (State==STATE_C) ChargeTimer=Timer/1000;	// Update ChargeTimer (unused)

 			if ((!Mode) && ((State==STATE_B) || (State==STATE_C)) )
			{
				timeout=10;						// If Normal Mode for the EVSE is selected, we'll have to set some fixed values..
				Imeasured=0;
				SetCurrent(CalcCurrent());		// Calculate charge current, and set PWM output	(normal mode)
			}
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
					if (Irms[x] > Imeasured) Imeasured=Irms[x];	// Imeasured holds highest Irms of all channels
				}
		
 			if ((Mode) && ((State==STATE_B) || (State==STATE_C)) ) SetCurrent(CalcCurrent());		// Calculate charge current, and set PWM output	(Smart mode)

  		//	printf((const far rom char *)"STATE:%c ChargeDelay:%u CT1:%3u.%01uA CT2:%3u.%01uA CT3:%3u.%01uA Imeas:%3u.%01uA Iset:%u.%01uA\r\n",State-1+'A', ChargeDelay, (unsigned int)Irms[0]/10, (unsigned int)Irms[0]%10, (unsigned int)Irms[1]/10, (unsigned int)Irms[1]%10, (unsigned int)Irms[2]/10, (unsigned int)Irms[2]%10,(unsigned int)Imeasured/10,(unsigned int)Imeasured%10,(unsigned int)Iset/10,(unsigned int)Iset%10);

			}
			ISRFLAG=0;										// ready to receive new data
			if (Error == CT_NOCOMM) Error=NO_ERROR;			// Clear communication error, if present
			timeout=10;										// 10 second timeout for CT data
		}


	} // end of while(1) loop
}
