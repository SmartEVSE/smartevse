/*
;    Project:       3+ Phase Current Transmitter, for use with Smart EVSE
;    Date:          16 October 2013
;
;    Changes:
;    1.0  Initial release
;
;    (C) 2013  Michael Stegen / Stegen Electronics
;
;	 RMS current measurement calculation, from openenergymonitor.org
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


#include "p18f14k22.h"
#include <stdio.h>
#include <math.h>


// Configuration settings
#pragma	config FCMEN = OFF,	IESO = OFF, PCLKEN = ON
#pragma config PLLEN = ON, FOSC = IRC
#pragma	config BORV = 30, BOREN = OFF, PWRTEN = ON
#pragma	config WDTPS = 2048, WDTEN = OFF     // WDT timeout
#pragma config MCLRE  = OFF, HFOFST = OFF
#pragma config XINST = ON, BBSIZ = OFF, LVP = OFF, STVREN = ON
#pragma	config CP0 = OFF, CP1 = OFF, CPD = OFF, CPB = OFF
#pragma	config WRT0 = OFF, WRT1 = OFF
#pragma	config WRTD = OFF, WRTB = OFF, WRTC = OFF
#pragma	config EBTR1 = OFF, EBTR0 = OFF
#pragma	config EBTRB = OFF

// Global data
#pragma udata

#define VERSION 1
#define SAMPLES 2048				// Samples per channel
#define BAUD1K2						// Baudrate 1200 bps
#define NR_CTS	3					// Number of CT's	

char buffer[50];
double Irms[NR_CTS];
unsigned int lastSampleI,sampleI;   //sample_ holds the raw analog read value, lastSample_ holds the last sample
double lastFilteredI, filteredI;	//Filtered_ is the raw analog value minus the DC offset

unsigned int sampleI_CT[NR_CTS]={0,0,0};
double filteredI_CT[NR_CTS]={0,0,0};



void UartTX(char byte) 
{
	// output one byte 
	while(!PIR1bits.TXIF)			// set when register is empty 
		continue;
	TXREG = byte;
}

/*
void UartTXHex(unsigned char c)
{
	unsigned char h;
	
	h = (c>>4);
	if (h>9) h+=7;
	h+='0';						// conversion to ASCII
	UartTX(h);					// TX 

	h = c & 0x0f;
	if (h>9) h+=7;
	h+='0';						// conversion to ASCII
	UartTX(h);
}
*/



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




void UART_RX(void)
{
	char x;
	x = RCREG;						// read USART byte to clear PIR1bits.RCIF flag
									// No further processing is done at this time 
}


/*  Send buffer over RS485. Use PPP in HDLC-like Framing for transfers
;   for protocol spec see: http://tools.ietf.org/html/rfc1662
;   compatible with existing RS485 line used on SMA solar inverters.
;
; 	baudrate is 1200 bps
; 	
;	<protocol>,<version>,<nr of samples>,<sample 1>...<sample n>,<crc16>
;   protocol = 0x5001 (2 bytes)
;	version = 0x01 (1 byte) 
;	nr of channels = 1 byte
;	samples are 4 bytes (double)
;	crc16 is 2 bytes
;
;	total 4+(n*4)+2 bytes (+ HDLC overhead)
;
*/
void UART_SendBuf(char* buffer, unsigned char len)
{
	char ch;
	PORTBbits.RB6 = 1;					// set RS485 transceiver to transmit
	UartTX(0x7E);						// transmit sync flag

	while(len--)			
	{
		ch = *buffer++;					// load next byte
		if ((ch == 0x11) || (ch == 0x12) || (ch == 0x13) || (ch == 0x7E) || (ch == 0x7D))	// check for escape character
		{
			ch = ch^0x20;
			UartTX(0x7D);				// transmit escape character
		}
		UartTX(ch);						// transmit byte
	}
	
	UartTX(0x7E);						// transmit sync flag

	while (!TXSTAbits.TRMT)	continue;	// transmit register empty?
	PORTBbits.RB6 = 0;					// set RS485 transceiver to receive

}



unsigned int ReadAnalog(void)	// Start ADC conversion, and return result
{
	ADCON0bits.GO=1;						// initiate conversion on the selected channel
	while(ADCON0bits.GO);
	return ADRES;							// return result
}

double ReadCT(unsigned char Ch)
{
	unsigned int n;
	double sumI;			//sum = Sum
	unsigned char ADCch;

	ADCch=Ch+4;				// Set ADC channel
	ADCch=(ADCch<<2)+1;		// shift 2 pos to left, enable ADC
	ADCON0=ADCch;			// channel select

	sumI = 0;

	sampleI=sampleI_CT[Ch];										// Get Sample and Filter values
	filteredI=filteredI_CT[Ch];	

	for (n = 0; n < SAMPLES; n++)
  	{
    	lastSampleI = sampleI;									// Used for offset removal
    	sampleI = ReadAnalog();									// Read port RAx 
    	lastFilteredI = filteredI;								// Used for offset removal
    	filteredI = (0.996*(lastFilteredI+sampleI-lastSampleI)); 	// Digital high pass filters to remove 2.5V DC offset. 
		
		// Root-mean-square method current
	   	// 1) square current values
       	// 2) sum 
    	sumI += (filteredI * filteredI);
	}

	sampleI_CT[Ch]=sampleI;
	filteredI_CT[Ch]=filteredI;									// Store Sample and Filter values

	return (sqrt(sumI / SAMPLES));								// Return squareroot of uncalibrated value

}

void init(void)
{
	OSCCON = 0b11100000;	// setup 32Mhz internal oscillator (max 64Mhz)
	PORTA = 0; 				// Init PORTA
 	LATA = 0;
	ANSEL = 0b11110000;		// RC0, RC1, RC2, RC3 are analog inputs (pin 16,15,14,7)
	ANSELH = 0;				// All digital IO
	TRISA = 0b00100000;		// Set UART RX as input
	PORTC = 0;
	TRISC = 0b00001111;		// RC0-RC3 analog inputs
	PORTB = 0;
	TRISB = 0b00000000;		// all outputs



#if defined(BAUD19K2)
	SPBRGH = 1;				// Initialize UART
	SPBRG = 160;			// Baudrate 19k2 
#endif
#if defined(BAUD4K8)
	SPBRGH = 6;				// Initialize UART
	SPBRG = 130;			// Baudrate 4800
#endif	
#if defined(BAUD1K2)
	SPBRGH = 0x1A;			// Initialize UART
	SPBRG = 0x0A;			// Baudrate 1200
#endif	
	BAUDCON = 0b00001000;	// 16 bit Baudrate register is used
	TXSTA = 0b00100100;		// Enable TX, 8 bit, Asynchronous mode
	RCSTA = 0b10010000;		// Enable serial port TX and RX, 8 bit. 

	ADCON0 = 0;				// ADC disabled for now
	ADCON1 = 0;
	ADCON2 = 0b10000010;	// Right justify, Tacq = 0 uS, FOSC/32

	T0CON = 0b10000111;		// @ 8Mhz => 32 uS Timer0, 16 bit counter

}

void main(void)
{
	char *pBytes;
	char x,n;
	unsigned int cs;

	init();						// initialize ports, ADC, UART
	
	for (x=0;x<NR_CTS;x++)				// initialize the CT's / ADC registers
			Irms[x] = ReadCT(x);		// Read CT1..CTX, and ignore..


	while(1)
	{
		for (x=0;x<NR_CTS;x++)
			Irms[x] = ReadCT(x);		// Read CT1..CTX

		TMR0H =0;
		TMR0L =0;						// reset timer 0

		do {
			if (PIR1bits.RCIF)			// Uart Reception?
			{
				UART_RX();				// yes, read received byte
				TMR0H =0;
				TMR0L =0;				// reset timer 0
			}
			if (RCSTAbits.OERR)			// Uart Overrun Error?
			{
				RCSTAbits.CREN = 0;
				RCSTAbits.CREN = 1;		// Restart Uart
			}
			x= TMR0L;					// Read TMR0 
		} while (TMR0H < 5);			// no data on RS485 line for 40 ms 

										// Setup HDLC frame
		buffer[0]= 0xff;				// Address Field = ff
		buffer[1]= 0x03;				// Control Field = 03
		buffer[2]= 0x50;				// Protocol = 0x5001
		buffer[3]= 0x01;
		buffer[4]= VERSION;
		buffer[5]= NR_CTS;

		n=6;
		for (x=0;x<NR_CTS;x++)
		{	
			pBytes = (char*)&Irms[x];	// get raw 4 byte Double 
			buffer[n++] = (pBytes[0]);
			buffer[n++] = (pBytes[1]);
			buffer[n++] = (pBytes[2]);
			buffer[n++] = (pBytes[3]);
		}
										// Frame Check Sequence (FCS) Field
		cs = calc_crc16(buffer, n);		// calculate CRC16 from data			
		buffer[n++] = ((unsigned char)(cs));
		buffer[n++] = ((unsigned char)(cs>>8));	

		PORTCbits.RC5=1;				// LED on
		UART_SendBuf(buffer,n);			// send buffer to RS485 port
		PORTCbits.RC5=0;				// LED off

		//printf("RMS CT1:%3u.%1uA CT2:%3u.%1uA CT3:%3u.%1uA\r\n",Irms1/10, Irms1%10, Irms2/10, Irms2%10, Irms3/10, Irms3%10);
	}
 

}


