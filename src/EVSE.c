/*
;    Project:       Smart EVSE
;    Date:          10 June 2016
;
;    Changes:
;    1.00  Initial release
;    1.01  Added Normal EVSE mode + Cable Lock support
;    1.02  Changed Max Capacity to 16A for 2,5mm2 cables
;    1.03  Changed Max Charge current range to 10-80A, also changed LCD delay to 2ms
;    1.04  Changed menu so that only relevant options are shown. Added option for fixed cable installations. (no Cable Lock, override PP pin)
;    1.05  Added LCD menu support. Most options can now be set directly on the module itself. Fixed charge current > 65A.
;    1.06  Added CT1 calibration to the LCD menu. Improved Smartmode current calculation and regulation.
;    1.07  Added Load balancing option for 2-4 SmartEVSE's. Added support for motor actuated cable lock.
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
rom char EE_LoadBl = LOADBL;

#pragma romdata



// Global data
#pragma udata udata
char	U1buffer[50];		// Uart1 Receive buffer /RS485
char	U1TXbuffer[50];		// Uart1 Transmit buffer /RS485
char	U2buffer[50];		// Uart2 buffer /Serial CLI
char	Tbuffer[50];		// temp buffer


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
char LoadBl;						// Load Balance Setting (Disable, Master or Slave1-3)

// total 16 bytes

double Irms[4]={0,0,0,0};			// Momentary current per Phase (Amps *10) (23= 2.3A)
									// Max 4 phases supported

unsigned int crc16;
unsigned char State = STATE_A;
unsigned char Error = NO_ERROR;
unsigned char NextState;

unsigned int MaxCapacity;			// Cable limit (Amps)(limited by the wire in the charge cable, set automatically, or manually if Config=Fixed Cable)
unsigned int Imeasured=0;			// Max of all CT inputs (Amps *10)

// Load Balance variables
int IsetBalanced=0;					// Max calculated current availabla for all EVSE's
int Balanced[4]={0,0,0,0};			// Amps value per EVSE (max 4)
int BalancedMax[4]={0,0,0,0};		// Max Amps value per EVSE (max 4)
char BalancedState[4]={0,0,0,0};	// State of all EVSE's 0=not active (state A), 1=charge request (State B), 2= Charging (State C) 

unsigned char RX1byte, TX1byte;
unsigned char idx=0,idx2=0,ISRFLAG=0,ISR2FLAG=0,ISRTXFLAG=0;
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
unsigned char ChargeDelay=0;		// Delays charging up to 60 seconds in case of not enough current available.
unsigned char NoCurrent=0;			// counts overcurrent situations.

const far rom char MenuConfig[] = "CONFIG - Set to Fixed Cable or Type 2 Socket";
const far rom char MenuMode[] 	= "MODE   - Set to Smart mode or Normal EVSE mode";
const far rom char MenuLoadBl[]	= "LOADBL - Set Load Balancing mode";
const far rom char MenuMains[] 	= "MAINS  - Set Max MAINS Current";
const far rom char MenuMax[] 	= "MAX    - Set MAX Charge Current for the EV";
const far rom char MenuMin[] 	= "MIN    - Set MIN Charge Current the EV will accept";
const far rom char MenuCable[] 	= "CABLE  - Set Fixed Cable Current limit";
const far rom char MenuLock[] 	= "LOCK   - Cable locking actuator type";
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
	}

	if (PIR1bits.TX1IF && PIE1bits.TX1IE)	// Uart1 transmit interrupt? RS485
	{
		TX1byte = U1TXbuffer[ISRTXFLAG];
		TXREG1 = TX1byte;				// send character
		if ((ISRTXFLAG && TX1byte==0x7E)|| ISRTXFLAG==49)	// end of buffer
		{
			PIE1bits.TX1IE = 0;			// clear transmit Interrupt for RS485 after sending last character
			ISRTXFLAG=0;				// end of transmission.
										// we switch of the transmitter in the main loop, after the final character has been sent..
		}
		else ISRTXFLAG++;
	}



	while(PIR3bits.RC2IF)						// Uart2 receive interrupt?
	{
												// Check for BREAK character, then Reset
		if(RCSTA2bits.FERR && (State == STATE_A) && RCONbits.POR)
		//if(RCSTA2bits.FERR && RCONbits.POR)
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
		if (Lock==1)												// Cable lock type Solenoid?
		{
			if (Error || (State != STATE_C))
			{
				if (unlocktimer<300)								// 300ms pulse		
				{
					SOLENOID_UNLOCK;
				}
				else SOLENOID_OFF;
				if (unlocktimer++ >400)
				{
					if (PORTCbits.RC1 == 0)							// still locked...
					{
						if (unlocktimer >5000) unlocktimer =0; 		//try to unlock again in 5 seconds
					}
					else unlocktimer =400;		
				}
				locktimer=0;
			}	
			else  // State C
			{
				if (locktimer<300)									// 300ms pulse
				{
					 SOLENOID_LOCK;
				} 
				else SOLENOID_OFF;
				if (locktimer++ >400)
				{
					if (PORTCbits.RC1 == 1)							// still unlocked...
					{
						if (locktimer >5000) locktimer =0; 			//try to lock again in 5 seconds
					}
					else locktimer =400;		
				}
				unlocktimer=0;
			}
		}	
		else if (Lock==2)												// Cable lock type Motor?
		{
			if (Error || (State != STATE_C))
			{
				if (unlocktimer<600)								// 600ms pulse		
				{
					SOLENOID_UNLOCK;
				}
				else SOLENOID_OFF;
				if (unlocktimer++ >700)
				{
					if (PORTCbits.RC1 == 1)							// still locked...
					{
						if (unlocktimer >5000) unlocktimer =0; 		//try to unlock again in 5 seconds
					}
					else unlocktimer =700;		
				}
				locktimer=0;
			}	
			else  // State C
			{
				if (locktimer<600)									// 600ms pulse
				{
					 SOLENOID_LOCK;
				} 
				else SOLENOID_OFF;
				if (locktimer++ >700)
				{
					if (PORTCbits.RC1 == 0)							// still unlocked...
					{
						if (locktimer >5000) locktimer =0; 			//try to lock again in 5 seconds
					}
					else locktimer =700;		
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



void read_settings(void)
{
	char x;
	char *pBytes;
								  	// Read settings from EEprom
    pBytes = (char*)&MaxMains;
	EECON1 = 0; 					// select EEprom
	EEADR = 0;
  	for (x=0; x<EEPROM_BYTES;x++)	// EEPROM bytes to read
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

	
	for (x=0;x<EEPROM_BYTES;x++)	// EEprom bytes to write
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


// Create HDLC frame from data, and copy to output buffer
// Start RS485 transmission, by enabling TX interrupt
void RS485SendBuf(char* buffer, unsigned char len)
{
	char ch,index=0;
	unsigned long tmr;

	while (ISRTXFLAG) {}					// wait if we are still transmitting over RS485

	U1TXbuffer[index++]=0x7E;				// copy sync flag to output buffer
	while(len--)			
	{
		ch = *buffer++;						// load next byte
		if ((ch == 0x11) || (ch == 0x12) || (ch == 0x13) || (ch == 0x7E) || (ch == 0x7D))	// check for escape character
		{
			ch = ch^0x20;
			U1TXbuffer[index++]=0x7D;		// copy escape character
		}
		U1TXbuffer[index++]=ch;				// copy data to buffer
	}
	U1TXbuffer[index++]=0x7E;				// copy sync flag

	tmr=Timer+1000;	
	while ( idx && (tmr>Timer)) {}		// wait for RS485 reception to finish, with 1000ms timeout

	LATBbits.LATB5 = 1;					// set RS485 transceiver to transmit
	PIE1bits.TX1IE = 1;					// enable transmit Interrupt for RS485
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
	delay(100);
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


void SetCurrent(unsigned int current)		// current in Amps (16= 16A)
{
	unsigned int DutyCycle;

	current=current*10;						// multiply by 10 (current in Amps x10	(160= 16A) )
	if ((current >= 60) && (current <= 510)) DutyCycle = current/0.6;
											// calculate DutyCycle from current
	else if ((current > 510) && (current <=800)) DutyCycle = (current / 2.5)+640;
	else DutyCycle = 100;					// invalid, use 6A
	CCPR1L = DutyCycle >> 2;				// Msb of DutyCycle
											// 2 Lsb are part of CCP1CON, use Timer 2	
	CCP1CON =(((DutyCycle & 0x03) << 4) | 0x0C); 
											// PWM Pilot signal enabled
}



// Is there atleast 6A(configurable MinCurrent) available for a EVSE?
char IsCurrentAvailable(void)
{
	unsigned char n,ActiveEVSE=0;
	int Baseload,TotalCurrent=0;

	for (n=0;n<4;n++) if (BalancedState[n]==2)			// must be in STATE_C
	{
		ActiveEVSE++;									// Count nr of Active EVSE's
		TotalCurrent+=Balanced[n];						// Calculate total max charge current for all active EVSE's
	}
	if (ActiveEVSE==0)
	{
		if (Imeasured > ((MaxMains-MinCurrent)*10))
		{
			return 1;									// Not enough current available!, return with error
		}
	}
	else
	{	
		ActiveEVSE++;									// Do calculations with one more EVSE
		Baseload = Imeasured-(TotalCurrent*10);			// Calculate Baseload (load without any active EVSE)
		if (Baseload<0) Baseload=0;

		if (ActiveEVSE >4) ActiveEVSE=4;
		if ( (ActiveEVSE*(MinCurrent*10)+Baseload) > (MaxMains*10) )
		{
			return 1;									// Not enough current available!, return with error
		}
	}
	return 0;
}


// Calculates Balanced PWM current for each EVSE
// mod =0 normal
// mod =1 we have a new EVSE requesting to start charging.
void CalcBalancedCurrent(char mod)				
{
	int Average, MaxBalanced, Idifference;
	int BalancedLeft=0;
	int ActiveMax=0, TotalCurrent=0, Baseload;
	char CurrentSet[4]={0,0,0,0};
	char n;

	for (n=0;n<4;n++) if (BalancedState[n]==2)
	{
		BalancedLeft++;												// Count nr of Active (Charaging) EVSE's
		ActiveMax+=BalancedMax[n];									// Calculate total Max Amps for all active EVSEs
		TotalCurrent+=Balanced[n];									// Calculate total of all set charge currents
	}

	if (!mod)
	{
		Idifference = (MaxMains*10) - Imeasured;					// Difference between MaxMains and Measured current (can be negative)
	
		if (Idifference>0) IsetBalanced+=(Idifference/4);			// increase with 1/4th of difference (slowly increase current)
		else IsetBalanced+= Idifference;							// last PWM setting + difference (immediately decrease current)
		if (IsetBalanced<0) IsetBalanced=0;
	}

	Baseload = Imeasured-(TotalCurrent*10);							// Calculate Baseload (load without any active EVSE)
	if (Baseload<0) Baseload=0;

	if ((IsetBalanced > (MaxMains*10)) || (!Mode)) IsetBalanced=MaxMains*10;	// Isetbalanced limit = Max Mains
																	// for Normal mode, always set current to Max Mains

	
	if (BalancedLeft)												// Only if we have active EVSE's
	{
	
		if (mod) IsetBalanced = (MaxMains*10)-Baseload;				// Set max combined charge current to MaxMains - Baseload		

		if (IsetBalanced<0 || IsetBalanced<(BalancedLeft*(MinCurrent*10)) )
		{
			NoCurrent++;											// Flag NoCurrent left
			printf((const far rom char *)"No Current!!\n\r");
			IsetBalanced=(BalancedLeft*(MinCurrent*10));			// set minimal "MinCurrent" charge per active EVSE
		} else NoCurrent=0;				
	
		if (IsetBalanced > (ActiveMax*10)) IsetBalanced=ActiveMax*10; // limit to total maximum Amps (of all active EVSE's)

		MaxBalanced=(IsetBalanced/10);								// convert to Amps
	
		DEBUG_PRINT((const far rom char *)"Imeasured:%3u IsetBalanced:%3i Baseload:%3u ",Imeasured, IsetBalanced, Baseload);
	
// calculate average current per EVSE
		n=0;
		do
		{	
			Average= MaxBalanced/BalancedLeft;				// Average current for all active EVSE's
				
			// Check for EVSE's that have a lower MAX current
			if ((BalancedState[n]==2) && (!CurrentSet[n]) && (Average>=BalancedMax[n]))			// Active EVSE, and current not yet calculated?
			{
				Balanced[n]=BalancedMax[n];				// Set current to Maximum allowed for this EVSE
				CurrentSet[n]=1;						// mark this EVSE as set.
				BalancedLeft--;							// decrease counter of active EVSE's
				MaxBalanced-=Balanced[n];				// Update total current to new (lower) value
				n=0;									// check all EVSE's again
			} else n++;
		} while (n<4 && BalancedLeft);
	
// All EVSE's which had a Max current lower then the average are set.
// Now calculate the current for the EVSE's which had a higher Max current
		n=0;
		if (BalancedLeft)		// Any Active EVSE's left?
		{
			do
			{	// Check for EVSE's that are not set yet
				if ((BalancedState[n]==2) && (!CurrentSet[n]))		// Active EVSE, and current not yet calculated?
				{
					Balanced[n]=MaxBalanced/BalancedLeft;		// Set current to Average
					CurrentSet[n]=1;							// mark this EVSE as set.
					BalancedLeft--;								// decrease counter of active EVSE's
					MaxBalanced-=Balanced[n];					// Update total current to new (lower) value
				}
			} while (++n<4 && BalancedLeft);
		}
	
		for (n=0;n<4;n++) DEBUG_PRINT((const far rom char *)"EVSE%u[%u]:%2uA  ",n,BalancedState[n],Balanced[n]);
		DEBUG_PRINT((const far rom char *)"\n\r");
	} // BalancedLeft

}


void BroadcastCurrent(void)											// Broadcast momentary currents to all Slave EVSE's
{
		char n,x;
		unsigned int cs;

		Tbuffer[0]= 0xff;				// Address Field = ff
		Tbuffer[1]= 0x03;				// Control Field = 03
		Tbuffer[2]= 0x50;				// Protocol = 0x5002
		Tbuffer[3]= 0x02;
		Tbuffer[4]= 0x01;				// Version
		Tbuffer[5]= 0x00;				// Broadcast Address
		Tbuffer[6]= 0x01;				// Command

		n=7;
		for (x=1;x<4;x++)
		{	
			Tbuffer[n++] = 0x00;
			Tbuffer[n++] = Balanced[x];
		}
											// Frame Check Sequence (FCS) Field
		cs = calc_crc16(Tbuffer, n);		// calculate CRC16 from data			
		Tbuffer[n++] = ((unsigned char)(cs));
		Tbuffer[n++] = ((unsigned char)(cs>>8));	

		RS485SendBuf(Tbuffer,n);			// send buffer to RS485 port
}


void SendRS485(char address,char command,char data,char data2)			// Send command over RS485
{
		unsigned int cs;

		Tbuffer[0]= 0xff;				// Address Field = ff
		Tbuffer[1]= 0x03;				// Control Field = 03
		Tbuffer[2]= 0x50;				// Protocol = 0x5002
		Tbuffer[3]= 0x02;
		Tbuffer[4]= 0x01;				// Version
		Tbuffer[5]= address;			// Slave or Broadcast Address
		Tbuffer[6]= command;			// Command

		Tbuffer[7] = data;				// only used in error command
		Tbuffer[8] = data2;				// charge current
											// Frame Check Sequence (FCS) Field
		cs = calc_crc16(Tbuffer, 9);		// calculate CRC16 from data			
		Tbuffer[9] = ((unsigned char)(cs));
		Tbuffer[10] =((unsigned char)(cs>>8));	

		RS485SendBuf(Tbuffer,11);			// send buffer to RS485 port
}




// Serial Command line interface
// Display Menu, and process input.
//------------------------------------------------
// Smart EVSE
// -- Main menu --
// CONFIG- Set to Fixed Cable or Type 2 Socket
// MODE  - Set to Smart mode, or Normal EVSE mode
// LOADBL- Set Load Balancing to Disabled, Master or Slave1-3
// MAINS - Set max MAINS Current (25-100)
// MAX   - Set MAX Charge Current for the EV (16-80)
// MIN   - Set MIN Charge Current the EV will accept
// CAL   - Calibrate CT1
// LOCK  - Cable lock Disable/Solenoid/Motor
// L1: 1.2A L2: 5.3A L3: 0.4A (MAX:26A MIN:10A)
//
void RS232cli(void)
{
	unsigned int n;
	double Inew,Iold;

	printf((const far rom char *)"\r\n");
	if (menu==0)		// menu = Main Menu
	{
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"MAINS") == 0) && (Mode || (LoadBl==1)) ) menu=1;		// Switch to Set Max Mains Capacity (Smart mode)
		if (strcmppgm2ram(U2buffer,(const far rom char *)"MAX") == 0) menu=2;					// Switch to Set Max Current
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"MIN") == 0) && (Mode || (LoadBl==1)) ) menu=3;			// Switch to Set Min Current (Smart mode)
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"CAL") == 0) && Mode) menu=4;			// Switch to Calibrate CT1 (Smart mode)
		if (strcmppgm2ram(U2buffer,(const far rom char *)"MODE") == 0) menu=5;					// Switch to Normal or Smart mode
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"LOCK") == 0) && !Config) menu=6;		// Switch to Enable/Disable Cable Lock (Config=Socket)
		if (strcmppgm2ram(U2buffer,(const far rom char *)"CONFIG") == 0) menu=7;				// Switch to Fixed cable or Type 2 Socket
		if ((strcmppgm2ram(U2buffer,(const far rom char *)"CABLE") == 0) && Config) menu=8;		// Switch to Set fixed Cable Current limit (Config=Fixed)
		if (strcmppgm2ram(U2buffer,(const far rom char *)"LOADBL") == 0) menu=9;				// Switch to Set Load Balancing
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
			if (strcmppgm2ram(U2buffer,(const far rom char *)"SOLENOID") == 0)
			{
				Lock = 1;
				write_settings();				// Write to eeprom
			}
			else if (strcmppgm2ram(U2buffer,(const far rom char *)"MOTOR") == 0)
			{
				Lock = 2;
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
		else if (menu==9)					// Load Balancing Mode
		{
			if (strcmppgm2ram(U2buffer,(const far rom char *)"DISABLE") == 0)
			{
				LoadBl = 0;
				write_settings();				// Write to eeprom
			}
			else if (strcmppgm2ram(U2buffer,(const far rom char *)"MASTER") == 0)
			{
				LoadBl = 1;
				write_settings();				// Write to eeprom
			}
			else if (strcmppgm2ram(U2buffer,(const far rom char *)"SLAVE1") == 0)
			{
				LoadBl = 2;
				write_settings();				// Write to eeprom
			}
			else if (strcmppgm2ram(U2buffer,(const far rom char *)"SLAVE2") == 0)
			{
				LoadBl = 3;
				write_settings();				// Write to eeprom
			}
			else if (strcmppgm2ram(U2buffer,(const far rom char *)"SLAVE3") == 0)
			{
				LoadBl = 4;
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
		printf(MenuConfig);	printf((const far rom char *)"      - ");
		if (Config) printf((const far rom char *)"Fixed Cable\r\n");
		else printf((const far rom char *)"Type 2 Socket\r\n");
		//printf((const far rom char *)"MODE   - Set to Smart mode or Normal EVSE mode     (");
		printf(MenuMode); printf((const far rom char *)"    - ");
		if (Mode) printf((const far rom char *)"Smart\r\n");
		else printf((const far rom char *)"Normal\r\n");
		// Load Balancing menu item
		printf(MenuLoadBl); printf((const far rom char *)"                  - ");
		if (LoadBl==0) printf((const far rom char *)"Disabled\r\n");
		else if (LoadBl==1) printf((const far rom char *)"Master\r\n");
		else if (LoadBl==2) printf((const far rom char *)"Slave1\r\n");
		else if (LoadBl==3) printf((const far rom char *)"Slave2\r\n");
		else printf((const far rom char *)"Slave3\r\n");

		if (Mode || LoadBl==1) 
		{
			//printf((const far rom char *)"MAINS  - Set Max MAINS Current (Smart mode)        (%3u A)\r\n",MaxMains);
			printf(MenuMains); printf((const far rom char *)"                    - %3u A\r\n",MaxMains);
		}
		//printf((const far rom char *)"MAX    - Set MAX Charge Current for the EV         ( %2u A)\r\n",MaxCurrent);
		printf(MenuMax); printf((const far rom char *)"        -  %2u A\r\n",MaxCurrent);
		if (Mode || (LoadBl==1))
		{
		 	//printf((const far rom char *)"MIN    - Set MIN Charge Current the EV will accept ( %2u A)\r\n",MinCurrent);
			printf(MenuMin); printf((const far rom char *)"-  %2u A\r\n",MinCurrent);
		}	
		if (Config)
		{
			//printf((const far rom char *)"CABLE  - Set Fixed Cable Current limit             ( %2u A)\r\n",CableLimit);
			printf(MenuCable); printf((const far rom char *)"            -  %2u A\r\n",CableLimit);
		} else {
			//printf((const far rom char *)"LOCK   - Cable lock Disable/Solenoid/Motor         (");
			printf(MenuLock); printf((const far rom char *)"              - ");
			if (Lock==1) printf((const far rom char *)"Solenoid\r\n");
			else if (Lock==2) printf((const far rom char *)"Motor\r\n");
			else printf((const far rom char *)"Disabled\r\n");
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
		printf((const far rom char *)"Cable lock set to : ");
		if (Lock==2) printf((const far rom char *)"Motor\r\n");
		else if (Lock==1) printf((const far rom char *)"Solenoid\r\n");
		else printf((const far rom char *)"Disable\r\n");
		printf((const far rom char *)"Enter new Cable lock mode (DISABLE/SOLENOID/MOTOR): ");
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
	else if (menu==9)
	{
		printf((const far rom char *)"Load Balancing set to : ");
		if (LoadBl==0) printf((const far rom char *)"Disabled\r\n");
		else if (LoadBl==1) printf((const far rom char *)"Master\r\n");
		else printf((const far rom char *)"Slave%u\r\n",LoadBl-1);
		printf((const far rom char *)"Enter Load Balancing mode (DISABLE/MASTER/SLAVE1/SLAVE2/SLAVE3): ");
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
	unsigned char pilot,count=0,timeout=5;
	unsigned int i; 
	char DiodeCheck=0;
	char SlaveAdr, Command, Broadcast=0;
	int ChargeCurrent;
	int Idifference;

	init();									// initialize ports, ADC, UARTs etc

	delay(500);								// wait 0,5 sec
	LCD_init();

	read_settings();						// from EEprom
	IsetBalanced=MaxMains*10;				// Initially set to MaxMains


	delay(2000);							// wait 2 sec
	
	while(1)								// MAIN loop
	{

		if (ISR2FLAG) RS232cli();			// RS232 command line interface

		if (!ISRTXFLAG && TXSTA1bits.TRMT) LATBbits.LATB5 = 0;	// set RS485 transceiver to receive if the last character has been sent

		x = ButtonState;
		if ( (x != 0x07) || (x != OldButtonState) ) LCDMenu(x);		// Any button pressed or just released?

		if ( LCDNav && (ScrollTimer+5000<Timer) && (!SubMenu) )  LCDHelp();		// Update/Show Helpmenu


		if ((State == STATE_COMM_A) && (Timer>ACK_TIMEOUT))			// Wait for response from Master
		{
			SendRS485(LoadBl-1,0x01,0x00,0x00);				// Send command to Master
			printf((const far rom char *)"01 sent to Master, charging stopped\r\n");
			Timer=0;											// Clear the Timer
		}

		if (State == STATE_A)				// ############### EVSE State A #################
		{
			CCP1CON = 0;					// PWM off
			PORTCbits.RC2 = 1;				// Control pilot static +12V
			CONTACTOR_OFF;					// Contactor OFF
			BalancedState[0]=0;				// Mark as inactive

			pilot = ReadPilot();
			if (pilot == PILOT_9V)			// switch to State B ?
			{
				if (NextState == STATE_B)
				{
					if (count++ > 5) 													// repeat 5 times
					{
						if (IsCurrentAvailable()==1 ) Error = NOCURRENT;			// Enough current available to start Charging?
	
						if (ChargeDelay==0 && Error==NO_ERROR)
						{
							DiodeCheck=0;
							ProximityPin();					// Sample Proximity Pin
	 							printf((const far rom char *)"Cable limit: %uA  Max: %uA \r\n",MaxCapacity, MaxCurrent);
	
							if (LoadBl>1)												// Load Balancing : Slave 
							{
								SendRS485(LoadBl-1,0x02,0x00,MaxCapacity);				// Send command to Master, followed by Max cable capacity
								printf((const far rom char *)"02 sent to Master, reqested %uA\r\n",MaxCapacity);
								State = STATE_COMM_B;
								Timer=0;												// Clear the Timer
							}
							else
							{															// Load Balancing: Master or Disabled
								if (MaxCapacity<MaxCurrent) BalancedMax[0]=MaxCapacity;
								else BalancedMax[0]=MaxCurrent;

								BalancedState[0]=1;										// Mark as active
								State = STATE_B;										// switch to State B
								printf((const far rom char *)"STATE A->B\r\n");
							}
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


		if (State == STATE_COMM_B)			// Wait for response from Master
		{
			if (Timer>ACK_TIMEOUT) State = STATE_A;
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
						if (count++ > 5)							// repeat 5 times	
						{
							State = STATE_A;				// switch to STATE_A
							printf((const far rom char *)"STATE B->A\r\n");
							if (LoadBl>1)										// Load Balancing : Slave 
							{
								State = STATE_COMM_A;							// Tell Master we switched to State A
								Timer = ACK_TIMEOUT+1;							// Set the timer to Timeout value, so that it expires immediately
							}
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
						if (count++ > 5) 										// repeat 5 times
						{
							if ( (Error == NO_ERROR) && (ChargeDelay==0))
							{
								if (LoadBl>1)												// Load Balancing : Slave 
								{
									SendRS485(LoadBl-1,0x03,0x00,MaxCapacity);				// Send command to Master, followed by Max cable capacity
									printf((const far rom char *)"03 sent to Master, requested %uA\r\n",MaxCapacity);
									State = STATE_COMM_C;
									Timer=0;												// Clear the Timer
								}
								else
								{															// Load Balancing: Master or Disabled
									if (MaxCapacity<MaxCurrent) BalancedMax[0]=MaxCapacity;
									else BalancedMax[0]=MaxCurrent;

									if (IsCurrentAvailable()==0)
									{
										BalancedState[0]=2;										// Mark as Charging
										Balanced[0]=0;											// For correct baseload calculation set current to zero
 										CalcBalancedCurrent(1);									// Calculate charge current for all connected EVSE's

										CONTACTOR_ON;											// Contactor ON
										DiodeCheck=0;
										State = STATE_C;										// switch to STATE_C
										LCDTimer=0;
										Timer =0;												// reset msTimer and ChargeTimer
										if (!LCDNav)											// Don't update the LCD if we are navigating the menu
										{
											LCD_init();											// re-init LCD
											LCD();												// immediately update LCD
										}
										printf((const far rom char *)"STATE B->C\r\n");
									} 
									else Error = NOCURRENT;
								}
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

		if ((State == STATE_COMM_C) && (Timer>ACK_TIMEOUT))
		{
			DiodeCheck=0;
			State = STATE_B;				// switch back to STATE_B
			printf((const far rom char *)"No ack, STATE C->B\r\n");
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
						if (count++ > 5) 										// repeat 5 times
						{
							State = STATE_A;	// switch back to STATE_A
							printf((const far rom char *)"STATE C->A\r\n");
							if (LoadBl>1)										// Load Balancing : Slave 
							{
								State = STATE_COMM_A;							// Tell Master we switched to State A
								Timer = ACK_TIMEOUT+1;							// Set the timer to Timeout value, so that it expires immediately
							} 
							else BalancedState[0]=0;							// Master or Disabled
																				// Mark EVSE as disconnected
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
						if (count++ > 5) 									// repeat 5 times
						{

							CONTACTOR_OFF;									// Contactor OFF
							DiodeCheck=0;
							State = STATE_B;								// switch back to STATE_B
							if (LoadBl>1)									// Load Balancing : Slave 
							{
								State = STATE_COMM_CB;						// Send 04 command to Master
								Timer = ACK_TIMEOUT+1;						// Set the timer to Timeout value, so that it expires immediately
							}
							else BalancedState[0]=0;						// Master or Disabled
																			// Mark EVSE as inactive (still State B)
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
				}
			}
		}												// end of State C code

		if ((State == STATE_COMM_CB) && (Timer>ACK_TIMEOUT))
		{
			SendRS485(LoadBl-1,0x04,0x00,0x00);				// Send command to Master
			printf((const far rom char *)"04 sent to Master, charging stopped\r\n");
			Timer=0;											// Clear the Timer
		}

		if (Error == NOCURRENT)
		{
			if (ChargeDelay==0) printf((const far rom char *)"Not enough current available!\r\n");
			Error = LESS_6A;
			State = STATE_A;
			ChargeDelay = CHARGEDELAY;									// Set Chargedelay after the error clears
		}

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

			LCD();											// once a second, update LCD
			Temp();											// once a second, measure temperature

			if (ChargeDelay>0) ChargeDelay --;				// Decrease Charge Delay counter

			if ((TempEVSE <55) && (Error == TEMP_HIGH))		// Temperature below limit?
			{
				Error=NO_ERROR;								// clear Error
			}

			if ((Error==LESS_6A) && (LoadBl<2) && (IsCurrentAvailable()==0) )
			{	
				Error=NO_ERROR;								// Clear Errors if there is enough current available
			}
				
			if ((timeout == 0) && (Error == NO_ERROR))	 	// timout if current measurement takes > 10 secs
			{
				Error = CT_NOCOMM;
				State = STATE_A;							// switch back to state A
				printf((const far rom char *)"Error, communication error!\r\n");
				for (x=0;x<4;x++) BalancedState[x]=0;		// reset all states
			}
			else if (timeout) timeout--;

			if (TempEVSE >=60)								// Temperature too High?
			{
				Error = TEMP_HIGH;
				State = STATE_A;							// ERROR, switch back to STATE_A
				printf((const far rom char *)"Temperature too High!\r\n");
				for (x=0;x<4;x++) BalancedState[x]=0;		// reset all states
			}

			if (State==STATE_C) ChargeTimer=Timer/1000;		// Update ChargeTimer (unused)


			if (!Mode)										// Normal Mode
			{
				Imeasured=0;								// No measurements, so we set it to zero
				if (Broadcast) Broadcast--;					// once every two seconds, Broadcast charge current to all EVSE's

				if (LoadBl<2 && !Broadcast)					// Load Balancing mode: Master or Disabled
				{
					CalcBalancedCurrent(0);					// Calculate charge current for connected EVSE's
					if (LoadBl==1) BroadcastCurrent();		// Send to all EVSE's (only in Master mode)

					if ((State==STATE_B) || (State==STATE_C)) SetCurrent(Balanced[0]);		// set PWM output for Master
					Broadcast=2;							// reset counter to 2 seconds
					timeout=10;								// reset timeout counter (not checked for Master)
				}
			}
  			
		}	// end 1 second timer



/*  RS485 serial data is received by the ISR routine, and processed here..
	Reads serial packet with Raw Current values, measured from 1-N CT's, over a RS485 serial line
	baudrate is 1200 bps

	packet structure from sensorbox: 	
	<protocol>,<version>,<nr of samples>,<sample 1>...<sample n>,<crc16>
    protocol = 0x5001 (2 bytes)
	version = 1 (1 byte)
	nr of samples = 1 byte
	samples are 4 bytes (double)
	crc16 is 2 bytes
	total 4+(n*4)+2 bytes (+ HDLC overhead)


	Load balancing RS485 commands:
	protocol  0x5002		= load balancing commands
	version 	0x01
	adress		0x01		= slave 1 (communication always to/from master)
							= broadcast from master = 0x00 
	command		0x02		= request to charge
	data	  0x0020		= @ 32A
	data	  0x0000		= unused data bytes

	commands Slave -> Master: 
				0x01		= State A
				0x02		= request to charge , next two bytes = requested charge current
				0x03		= charging (State C) next two bytes = requested charge current
				0x04		= charging stopped (state C->B), followed by two empty bytes

	commands Master -> Slave:
				0x81		= Ack, State A
				0x82		= ack request to charge , next two bytes = calculated charge current
				0x83		= ACK charging , next two bytes = calculated charge current
				0x84		= ACK charging done.
			
	broadcast commands:
				0x01		= Charge current for each Slave EVSE (Smart mode: sent every 2 seconds, Normal mode: sent when needed)
							  followed by 6 bytes of data (2 bytes per EVSE)
				0x02		= Error occured, switch to State A. Error code in next databyte.
				
	
*/
	
		if (ISRFLAG > 1) 	// RS485 packet received?
		{
			crc16 = calc_crc16(U1buffer, ISRFLAG);				// calculate crc

//			for (x=0;x<ISRFLAG;x++) printf((const far rom char *)"%02x ",(unsigned char)U1buffer[x]);
//			printf((const far rom char *)"\n\r");

            if (ISRFLAG>10 && U1buffer[2]==0x50 && U1buffer[3]==0x01 && crc16==GOODFCS16) 		// Sensorbox data? 
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

				if (Mode && LoadBl<2)							// Load Balancing mode: Smart/Master or Disabled
				{
					CalcBalancedCurrent(0);						// Calculate dynamic charge current for connected EVSE's

					if (NoCurrent>2 || (Imeasured > (MaxMains*20)))	// No current left, or Overload (2x Maxmains)?
					{											// STOP charging for all EVSE's
						Error = NOCURRENT;						// Display error message
						for (x=0; x<4; x++) BalancedState[x]=0;	// Set all EVSE's to State A
							
						SendRS485(0x00,0x02,LESS_6A,ChargeDelay);	// Broadcast Error code over RS485
						NoCurrent=0;
					} else BroadcastCurrent();					// Send current to all EVSE's

					if ((State==STATE_B) || (State==STATE_C)) 
					{
						SetCurrent(Balanced[0]);				// Set current for Master EVSE in Smart Mode
					}
				//	printf((const far rom char *)"STATE:%c ChargeDelay:%u NoCurrent:%u CT1:%3u.%01uA CT2:%3u.%01uA CT3:%3u.%01uA Imeas:%3u.%01uA IsetBalanced:%u.%01uA\r\n",State-1+'A', ChargeDelay, NoCurrent, (unsigned int)Irms[0]/10, (unsigned int)Irms[0]%10, (unsigned int)Irms[1]/10, (unsigned int)Irms[1]%10, (unsigned int)Irms[2]/10, (unsigned int)Irms[2]%10,(unsigned int)Imeasured/10,(unsigned int)Imeasured%10,(unsigned int)IsetBalanced/10,(unsigned int)IsetBalanced%10);

					timeout=10;									// reset 10 second timeout
				} else Imeasured=0;								// In case Sensorbox is connected in Normal mode. Clear measurement.
 		
			}
			else if (ISRFLAG>6 && U1buffer[2]==0x50 && U1buffer[3]==0x02 && crc16==GOODFCS16) 		// We received a command
			{				
				SlaveAdr = U1buffer[5];	//EVSE 0x01 - 0x03 (slaves) 
				Command = U1buffer[6];
				ChargeCurrent = U1buffer[8];

				timeout=10;													// reset 10 second timeout

				if (SlaveAdr==0x00 && Command==0x01 && LoadBl>1)			// Broadcast message from Master->Slaves, Set Charge current
				{	
					Balanced[0]=U1buffer[4+(LoadBl*2)];
					if ((State==STATE_B) || (State==STATE_C) ) SetCurrent(Balanced[0]);		// Set charge current, and PWM output
					DEBUG_PRINT((const far rom char *)"Broadcast received, Slave %uA \r\n",Balanced[0]);
				} 
				else if (SlaveAdr==0x00 && Command==0x02 && LoadBl>1)		// Broadcast message from Master->Slaves, Error Occured!
				{	
					State=STATE_A;
					Error=U1buffer[7];
					ChargeDelay = CHARGEDELAY;												
					DEBUG_PRINT((const far rom char *)"Broadcast Error message received!\r\n");
				}

				if (SlaveAdr==(LoadBl-1))						// We are a Slave, Direct commands from the Master are handled here
				{	
					if (Command==0x81)								// SLAVE: ACK State A
					{
						State=STATE_A;
						DEBUG_PRINT((const far rom char *)"81 ACK State A\r\n");
					}
					else if (Command==0x82)							// ACK received, followed by assigned charge current
					{
						if (ChargeCurrent==0)						// If chargecurrent is zero, No current is available.
						{
							Error=NOCURRENT;
							DEBUG_PRINT((const far rom char *)"82 ACK ");	// No current available
						}
						else
						{
							SetCurrent(ChargeCurrent);
							State=STATE_B;
							DEBUG_PRINT((const far rom char *)"82 ACK State A->B, charge current: %uA\r\n",ChargeCurrent);
						}
					}
					else if (Command==0x83)							// ACK received, state C followed by charge current
					{
						if (ChargeCurrent==0)						// If chargecurrent is zero, No current is available.
						{
							Error=NOCURRENT;
							DEBUG_PRINT((const far rom char *)"83 ACK "); // No current available
						}
						else
						{
							SetCurrent(ChargeCurrent);
							CONTACTOR_ON;								// Contactor ON
							DiodeCheck=0;
							State=STATE_C;								// switch to STATE_C
							LCDTimer=0;	
							Timer =0;									// reset msTimer and ChargeTimer
							if (!LCDNav)								// Don't update the LCD if we are navigating the menu
							{
								LCD_init();								// re-init LCD
								LCD();									// immediately update LCD
							}
							DEBUG_PRINT((const far rom char *)"83 ACK State C charge current: %uA\r\n",ChargeCurrent);
							printf((const far rom char *)"STATE B->C\r\n");
						}
					}
					else if (Command==0x84)							// Charging Stopped, State B
					{
						State=STATE_B;
						DEBUG_PRINT((const far rom char *)"84 ACK State C->B, charging stopped\r\n");
					}

				}
				if (LoadBl==1)										// We are the Master, commands received from the Slaves are handled here
				{
					if (Command==0x01)								// Slave state changed to State A
					{
						BalancedState[SlaveAdr]=0;					// Keep track of the state and store it.		
						CalcBalancedCurrent(0);						// Calculate dynamic charge current for connected EVSE's
						printf((const far rom char *)"01 Slave %u State A\r\n",SlaveAdr);
						SendRS485(SlaveAdr,0x81,0x00,0x00);			// Send ACK to Slave, followed by two dummy bytes
					}
					else if (Command==0x02)							// Request to charge , next two bytes = requested charge current
					{
						if (IsCurrentAvailable()==0)				// check if we have enough current
						{											// Yes enough current..
							BalancedState[SlaveAdr]=1;				// Mark Slave EVSE as active (State B)
							BalancedMax[SlaveAdr]=ChargeCurrent;	// Set requested charge current.
							Balanced[SlaveAdr]=MinCurrent;			// Initially set current to lowest setting
						} else Balanced[SlaveAdr]=0;				// Make sure the Slave does not start charging by setting current to 0
						printf((const far rom char *)"02 Slave %u requested:%uA\r\n",SlaveAdr,ChargeCurrent);
						SendRS485(SlaveAdr,0x82,0x00,Balanced[SlaveAdr]);	// Send ACK to Slave, followed by assigned current
					}
					else if (Command==0x03)							// Charging, next two bytes = requested charge current
					{
						if (IsCurrentAvailable()==0)				// check if we have enough current
						{											// Yes
							BalancedState[SlaveAdr]=2;				// Mark Slave EVSE as Charging (State C)
							BalancedMax[SlaveAdr]=ChargeCurrent;	// Set requested charge current.
							Balanced[SlaveAdr]=0;					// For correct baseload calculation set current to zero
 							CalcBalancedCurrent(1);					// Calculate charge current for all connected EVSE's
						} else Balanced[SlaveAdr]=0;				// Make sure the Slave does not start charging by setting current to 0
						printf((const far rom char *)"03 Slave %u charging: %uA\r\n",SlaveAdr,Balanced[SlaveAdr]);
						SendRS485(SlaveAdr,0x83,0x00,Balanced[SlaveAdr]);	// Send ACK to Slave, followed by assigned current
					}
					else if (Command==0x04)							// charging stopped (state C->B), followed by two empty bytes
					{
						BalancedState[SlaveAdr]=0;					// Mark Slave EVSE as inactive (still State B)
						CalcBalancedCurrent(0);						// Calculate dynamic charge current for connected EVSE's
						printf((const far rom char *)"04 C->B Slave %u inactive\r\n",SlaveAdr);
						SendRS485(SlaveAdr,0x84,0x00,0x00);			// Send ACK to Slave
					}

				} // commands for Master

			}	

			ISRFLAG=0;																// ready to receive new data
			if (Error == CT_NOCOMM && timeout==10) Error=NO_ERROR;					// Clear communication error, if present

			if (Error == LESS_6A && ChargeDelay==0 && LoadBl>1) Error=NO_ERROR;		// Clear Error after delay (Slave)
		}


	} // end of while(1) loop
}
