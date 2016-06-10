/*
;	 Project:       Smart EVSE
;    Date:          1 February 2016
;
;
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

#ifndef __EVSE_MAIN
#define __EVSE_MAIN

#include "p18f25k22.h"
#include <stdio.h>
#include <string.h>
#include "LCD.h"

#define VERSION "1.07"					// SmartEVSE software version
//#define DEBUG_P							// Debug print enable/disable

#define ICAL 3.00						// Irms Calibration value (for Current transformers) 
#define MAX_MAINS 25					// max Current the Mains connection can supply
#define MAX_CURRENT 13					// max charging Current for the EV
#define MIN_CURRENT 6					// minimum Current the EV will accept
#define MODE 0							// Normal EVSE mode
#define LOCK 0							// No Cable lock
#define CABLE_LIMIT 13					// Manual set Cable Limit (for use with Fixed Cable)
#define CONFIG 0						// Configuration: 0= TYPE 2 socket, 1= Fixed Cable
#define LOADBL 0						// Load Balancing disabled
#define CHARGEDELAY 60					// Seconds to wait after overcurrent, before tryting again

#define GOODFCS16 0x0f47				// crc16 frame check value
#define ACK_TIMEOUT 1000				// 1000ms timeout

#define STATE_A 1						// Vehicle not connected
#define STATE_B 2						// Vehicle connected / not ready to accept energy
#define STATE_C 3						// Vehicle connected / ready to accept energy / ventilation not required
#define STATE_D 4						// Vehicle connected / ready to accept energy / ventilation required
#define STATE_COMM_A 5					// Waiting for response from Master
#define STATE_COMM_B 6					// Waiting for response from Master
#define STATE_COMM_C 7					// Waiting for response from Master
#define STATE_COMM_CB 8					// Waiting for response from Master

#define PILOT_12V 1
#define PILOT_9V 2
#define PILOT_6V 3
#define PILOT_DIODE 4 
#define PILOT_NOK 0

#define NO_ERROR 0
#define LESS_6A 1
#define CT_NOCOMM 2
#define TEMP_HIGH 3
#define NOCURRENT 4						// No Current! ERROR=LESS_6A, switch to STATE A

#define SOLENOID_LOCK		{LATAbits.LATA4 = 1;LATAbits.LATA5 = 0;}
#define SOLENOID_UNLOCK		{LATAbits.LATA4 = 0;LATAbits.LATA5 = 1;}
#define SOLENOID_OFF		{LATAbits.LATA4 = 1;LATAbits.LATA5 = 1;}

#define CONTACTOR_OFF LATBbits.LATB4 = 0;				// Contactor OFF
#define CONTACTOR_ON  LATBbits.LATB4 = 1;				// Contactor ON

#define MENU_CONFIG 10
#define MENU_MODE 20
#define MENU_LOADBL 100
#define MENU_MAINS 30
#define MENU_MAX 40
#define MENU_MIN 50
#define MENU_LOCK 60
#define MENU_CABLE 70
#define MENU_CAL 80
#define MENU_EXIT 90

#ifdef DEBUG_P
 #define DEBUG_PRINT(x) printf(x)
#else
 #define DEBUG_PRINT(x)
#endif 



#pragma udata
extern unsigned int MaxMains;		// Max Mains Amps (hard limit, limited by the MAINS connection)
extern unsigned int MaxCurrent;		// Max Charge current
extern unsigned int MinCurrent;		// Minimal current the EV is happy with
extern double ICal;					// CT calibration value
extern char Mode;					// EVSE mode
extern char Lock;					// Cable lock enable/disable
extern unsigned int CableLimit;		// Fixed Cable Current limit (only used when config is set to Fixed Cable)
extern char Config;					// Configuration (Fixed Cable or Type 2 Socket)
extern char LoadBl;					// Load Balance Setting (Disable, Master or Slave)

#define EEPROM_BYTES 16				// total 16 bytes


extern double Irms[4];				// Momentary current per Phase (Amps *10) (23= 2.3A)
									// Max 4 phases supported
extern unsigned int crc16;
extern unsigned char State;
extern unsigned char Error;
extern unsigned char NextState;

extern unsigned int MaxCapacity;		// Cable limit (Amps)(limited by the wire in the charge cable, set automatically, or manually if Config=Fixed Cable)
extern unsigned int Imeasured;			// Max of all CT inputs (Amps *10)
//extern int Iset;						// PWM output is set to this current level (Amps *10)
extern int Balanced[4];					// Amps value per EVSE (max 4)

extern unsigned char RX1byte;
extern unsigned char idx,idx2,ISRFLAG,ISR2FLAG;
extern unsigned char menu;
extern unsigned int locktimer,unlocktimer;	// solenoid timers
extern unsigned long Timer;				// mS counter
extern unsigned int ChargeTimer;		// seconds counter
extern unsigned char LCDTimer;
extern unsigned char TempEVSE;			// Temperature EVSE in deg C (0-125)
extern unsigned char ButtonState;		// Holds latest push Buttons state (LSB 2:0)
extern unsigned char OldButtonState;	// Holds previous push Buttons state (LSB 2:0)
extern unsigned char LCDNav;
extern unsigned char SubMenu;
extern unsigned long ScrollTimer;
extern unsigned char LCDpos;
extern unsigned char ChargeDelay;		// Delays charging up to 60 seconds in case of not enough current avilable.


extern const far rom char MenuConfig[];
extern const far rom char MenuMode[];
extern const far rom char MenuLoadBl[];
extern const far rom char MenuMains[];
extern const far rom char MenuMax[];
extern const far rom char MenuMin[];
extern const far rom char MenuCable[];
extern const far rom char MenuLock[];
extern const far rom char MenuCal[];

void delay(unsigned int d);
void read_settings(void);
void write_settings(void);

#endif