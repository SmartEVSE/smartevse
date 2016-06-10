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

#include "EVSE.h"
#include "LCD.h"


void LCD_write(unsigned char c)
{
	unsigned char x,y;
	x= PORTB & 0xf0;
	PORTB=x | (0x0f & (c >>4));
	PORTCbits.RC5 = 1;					// LCD Enable 1
	PORTCbits.RC5 = 0;					// LCD Enable 0
	delay(2);							// changed from 1 to 2ms
	PORTB=x | (0x0f & c);
	PORTCbits.RC5 = 1;					// LCD Enable 1
	PORTCbits.RC5 = 0;					// LCD Enable 0
	delay(2);
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

void LCD_print8(const far rom char *data )	// write 8 characters to LCD
{
	unsigned char x=0;

	LCD_write(0x80);
	PORTCbits.RC3 = 1;					// LCD RS 
	do
  	{
    	LCD_write(*data++);
  	} while (x++<8);
	PORTCbits.RC3 = 0;					// LCD RS
}

void LCD_print_menu(const far rom char *data,char RowAdr )	// write string of data to LCD, with navigation arrows
{
	LCD_write(RowAdr);					// Set Row

	PORTCbits.RC3 = 1;					// LCD RS
	if ((SubMenu && RowAdr==0xC0) || (!SubMenu && RowAdr==0x80)) LCD_write('<');
	else LCD_write(' ');	
	do
  	{
    	LCD_write(*data);
  	} while (*++data);
	if ((SubMenu && RowAdr==0xC0) || (!SubMenu && RowAdr==0x80)) LCD_write('>');
	else LCD_write(' ');	
	PORTCbits.RC3 = 0;					// LCD RS
}

void LCD_print_Amps(unsigned int Amps )					// write data to LCD
{
	LCD_write(0xC0);
	PORTCbits.RC3 = 1;					// LCD RS 
	if (SubMenu) LCD_write('<');
	else LCD_write(' ');	
	LCD_write(' ');
	if (Amps>=100) 
	{
		LCD_write( (unsigned char)(Amps/100)+0x30 );
		Amps=Amps%100;
	}
	else LCD_write(' ');
	LCD_write( (unsigned char)(Amps/10)+0x30 );
	LCD_write( (unsigned char)(Amps%10)+0x30 );
	LCD_write('A');
	LCD_write(' ');
	if (SubMenu) LCD_write('>');
	else LCD_write(' ');	
	PORTCbits.RC3 = 0;					// LCD RS 
}


void LCDHelp(void)							// Display/Scroll helptext on LCD 
{
		unsigned char x;

			switch (LCDNav)
			{
				case MENU_CONFIG:
						x=strlenpgm(MenuConfig);
						LCD_print8(MenuConfig+LCDpos);
						break;
				case MENU_MODE:
						x=strlenpgm(MenuMode);
						LCD_print8(MenuMode+LCDpos);
						break;
				case MENU_LOADBL:
						x=strlenpgm(MenuLoadBl);
						LCD_print8(MenuLoadBl+LCDpos);
						break;
				case MENU_MAINS:
						x=strlenpgm(MenuMains);
						LCD_print8(MenuMains+LCDpos);
						break;
				case MENU_MAX:
						x=strlenpgm(MenuMax);
						LCD_print8(MenuMax+LCDpos);
						break;
				case MENU_MIN:
						x=strlenpgm(MenuMin);
						LCD_print8(MenuMin+LCDpos);
						break;
				case MENU_LOCK:
						x=strlenpgm(MenuLock);
						LCD_print8(MenuLock+LCDpos);
						break;
				case MENU_CABLE:
						x=strlenpgm(MenuCable);
						LCD_print8(MenuCable+LCDpos);
						break;
				case MENU_CAL:
						x=strlenpgm(MenuCal);
						LCD_print8(MenuCal+LCDpos);
						break;

				default:
						break;
			}
			if (LCDpos++==8) ScrollTimer=Timer-4000;
			else if (LCDpos > (x-8) )
			{
				ScrollTimer=Timer-3000;
				LCDpos=8;
			}
			else ScrollTimer=Timer-4700;
		
}



// called once a second
void LCD(void)
{
		unsigned char x;

		if (LCDNav)
		{
			if (LCDTimer++ == 120)
			{	 
				LCDNav=0;						// Exit Setup menu after 120 seconds.
				read_settings();				// don't save, but restore settings
			}
			else return;						// disable LCD status messages when navigating LCD Menu	
		}

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
			LCD_print((const far rom char *)"CHARGE");
			PORTCbits.RC3 = 1;					// LCD RS 
			if (ChargeDelay>0)
			{
				LCD_write((ChargeDelay/10)+0x30 );
				LCD_write((ChargeDelay%10)+0x30 );
			}
			else
			{
				LCD_write(' ');
				LCD_write(' ');
			}
			PORTCbits.RC3 = 0;					// LCD RS 
		}
		else if (State == STATE_C)
		{
			PORTCbits.RC0 = 1;					// LCD backlight on

			LCD_write(0x80);				// address 00H / first row
			LCD_print((const far rom char *)"CHARGING");
			LCD_write(0xC0);				// address 40H / second row
			PORTCbits.RC3 = 1;					// LCD RS 
			LCD_write( (Balanced[0]/10)+0x30 );
			LCD_write( (Balanced[0]%10)+0x30 );
			LCD_write('A');
			if (Mode)						// Smart Mode?
			{
				LCD_write('(');				
				LCD_write( (unsigned char)(MaxMains/10)+0x30 );
				LCD_write( (unsigned char)(MaxMains%10)+0x30 );
				LCD_write('A');
				LCD_write(')');
			} else LCD_print((const far rom char *)"     ");	// clear rest of row

			PORTCbits.RC3 = 0;	
		}

}


//##############################################################################################################################
// 10 CONFIG			- Set to Fixed Cable or Type 2 Socket
// 20 MODE  			- Set to Smart mode, or Normal EVSE mode
// 25 LOADBL			- Load Balance, set to Disable, Master or Slave
// 30		MAINS 		- Set max MAINS Current (25-100) (Mode=Smart)
// 40 MAX   			- Set MAX Charge Current for the EV (16-80)
// 50		MIN   		- Set MIN Charge Current the EV will accept (Mode=Smart)
// 60 LOCK				- Cable lock Enable/disable
// 70 CABLE				- Set Fixed Cable Current limit 
// 80 CAL 		  		- Calibrate CT1
// 90 EXIT				- Exit Menu



const far rom char StrConfig[] = 	"CONFIG";
const far rom char StrMode[] = 		" MODE ";
const far rom char StrFixed[] = 	" Fixed";
const far rom char StrSocket[] = 	"Socket";
const far rom char StrSmart[] =		" Smart";
const far rom char StrNormal[] =	"Normal";
const far rom char StrMains[] = 	" MAINS";
const far rom char StrMax[] =	 	"  MAX ";
const far rom char StrMin[] = 		"  MIN ";
const far rom char StrLock[] = 		" LOCK ";
const far rom char StrSolenoid[] =	"Soleno";
const far rom char StrMotor[] =		"Motor ";
const far rom char StrDisable[] =	"Disabl";
const far rom char StrCable[] = 	" CABLE";
const far rom char StrCal[] =	 	"  CAL ";
const far rom char StrLoadBl[] =	"LOADBL";
const far rom char StrMaster[] =	"Master";
const far rom char StrSlave1[] =	"Slave1";
const far rom char StrSlave2[] =	"Slave2";
const far rom char StrSlave3[] =	"Slave3";


void LCDMenu(unsigned char Buttons)
{
	static unsigned long ButtonTimer=0;
	static unsigned char ButtonRelease=0;			// keeps track of LCD Menu Navigation
	static unsigned int CT1,CT1old;
	static double Iold;
	
// Main Menu Navigation

	if ((LCDNav==0) && (Buttons==0x5) && (ButtonRelease==0))		// Button 2 pressed ?
	{
		LCDNav=1;							// about to enter menu
		ButtonTimer=Timer;					
	}
	else if  (LCDNav==1 && ((ButtonTimer+2000)<Timer))		// <CONFIG>
	{
		LCDNav=MENU_CONFIG;					// Main Menu entered
		PORTCbits.RC0 = 1;					// LCD backlight on
		ButtonRelease=1;
	}
	else if ((LCDNav==1) && (Buttons==0x7))	// Button 2 released before entering menu?
	{
		LCDNav=0;
		LCD();
	}
	else if ( (LCDNav>0) && ((LCDNav%10)==0) && (Buttons==0x3) && (ButtonRelease==0) )		// Button 1 > pressed 
	{
		switch (LCDNav)
		{
			case MENU_CONFIG:
					if (SubMenu)
					{
						if (Config) Config=0;
						else Config=1;
					} 
					else LCDNav=MENU_MODE;
					break;
			case MENU_MODE:
					if (SubMenu)
					{
						if (Mode) Mode=0;
						else Mode=1;
					} 
					else LCDNav=MENU_LOADBL;
					break;
			case MENU_LOADBL:
					if (SubMenu)
					{
						if (LoadBl==4) LoadBl=0;					// last menu item? goto first
						else LoadBl++;								// goto next
					} 
					else 
					{
						if (Mode) LCDNav=MENU_MAINS;				// Smart Mode?
						else LCDNav=MENU_MAX;
					}
					break;
			case MENU_MAINS:
					if (SubMenu)
					{
						MaxMains++;									// Set new MaxMains
						if (MaxMains>100) MaxMains=100;				// Max 100A
					}
					else LCDNav=MENU_MAX;
					break;
			case MENU_MAX:
					if (SubMenu)
					{
						MaxCurrent++;								// Set new MaxCurrent
						if (MaxCurrent>80) MaxCurrent=80;			// Max 80A
					}
					else
					{
						if (Mode) LCDNav=MENU_MIN;					// Smart Mode?
						else if (Config) LCDNav=MENU_CABLE;			// Cable Configuration, go to Cable Current
						else LCDNav=MENU_LOCK;						// Fixed Cable, use the lock
					}
					break;
			case MENU_MIN:
					if (SubMenu)
					{
						MinCurrent++;								// Set new MinCurrent
						if (MinCurrent>16) MinCurrent=16;			// Max 16A
					}
					else
					{
						if (Config) LCDNav=MENU_CABLE;				// Cable Configuration, go to Cable Current
						else LCDNav=MENU_LOCK;						// Fixed Cable, use the lock
					}
					break;
			case MENU_LOCK:
					if (SubMenu)
					{
						if (Lock==2) Lock=0;
						else Lock++;
						break;
					} 
			case MENU_CABLE:
					if (SubMenu)
					{
						CableLimit++;								// Set new CableLimit
						if (CableLimit>80) CableLimit=80;			// Max 80A
					}
					else 
					{
						if (Mode) LCDNav=MENU_CAL;
						else LCDNav=MENU_EXIT;
					}
					break;
			case MENU_CAL:
					if (SubMenu)
					{
						if (CT1>=100 && CT1<1000) CT1++;			// Increase CT1 measurement value by 0.1A
																	// Max 99.9A
					}
					else 
					{
						LCDNav=MENU_EXIT;
					}
					break;
			case MENU_EXIT:
					LCDNav=MENU_CONFIG;
			default:
					break;
		}
		ButtonRelease=1;
	}
	else if ( (LCDNav>0) && ((LCDNav%10)==0) && (Buttons==0x6) && (ButtonRelease==0) )		// Button 3 < pressed 
	{
		switch (LCDNav)
		{
			case MENU_EXIT:
					if (Mode) LCDNav=MENU_CAL;					// Smart Mode? Goto Cal CT1
					else if (Config) LCDNav=MENU_CABLE;			// Cable Configuration, go to Cable Current
					else LCDNav=MENU_LOCK;						// Fixed Cable, use the lock
					break;
			case MENU_CAL:
					if (SubMenu)
					{
						if (CT1>100) CT1--;						// Min 10.0A
					}
					else 
					{
						if (Config) LCDNav=MENU_CABLE;			// Cable Configuration, go to Cable Current
						else LCDNav=MENU_LOCK;					// Fixed Cable, use the lock
					}
					break;
			case MENU_CABLE:
					if (SubMenu)
					{
						CableLimit--;							// Set new CableLimit
						if (CableLimit<13) CableLimit=13;		// Min 13A
						break;
					}
			case MENU_LOCK:
					if (SubMenu)
					{
						if (Lock==0) Lock=2;
						else Lock--;
					} 
					else
					{
						if (Mode) LCDNav=MENU_MIN;				// Smart Mode?
						else LCDNav=MENU_MAX;
					}
					break;
			case MENU_MIN:
					if (SubMenu)
					{
						MinCurrent--;							// Set new MinCurrent
						if (MinCurrent<6) MinCurrent=6;			// Min 6A
					}
					else LCDNav=MENU_MAX;
					break;
			case MENU_MAX:
					if (SubMenu)
					{
						MaxCurrent--;							// Set new MaxCurrent
						if (MaxCurrent<10) MaxCurrent=10;		// Min 10A
					}
					else
					{
						if (Mode) LCDNav=MENU_MAINS;			// Smart Mode?
						else LCDNav=MENU_LOADBL;
					}
					break;
			case MENU_MAINS:
					if (SubMenu)
					{
						MaxMains--;								// Set new MaxMains
						if (MaxMains<25) MaxMains=25;			// Min 25A
					} else LCDNav=MENU_LOADBL;
					break;
			case MENU_LOADBL:
					if (SubMenu)
					{
						if (LoadBl==0) LoadBl=4;					// first menu item? goto last
						else LoadBl--;								// goto previous
					} 
					else LCDNav=MENU_MODE;
					break;
			case MENU_MODE:
					if (SubMenu)
					{
						if (Mode) Mode=0;
						else Mode=1;
					} 
					else LCDNav=MENU_CONFIG;
					break;
			case MENU_CONFIG:
					if (SubMenu)
					{
						if (Config) Config=0;
						else Config=1;
					} 
					else LCDNav=MENU_EXIT;
			default:
					break;
		}
		ButtonRelease=1;
	}
	else if (LCDNav>=10 && Buttons==0x5 && ButtonRelease==0)		// Button 2 pressed?
	{
		if (SubMenu)									// Are we in Submenu?
		{
			SubMenu=0;									// yes, exit Submenu
			if (LCDNav==MENU_CAL)						// Exit CT1 calibration?
			{
				if (CT1!=CT1old)						// did the value change?
				{
					Iold=(double)(CT1old/ICal);
					ICal=(double)(CT1/Iold);			// Calculate new Calibration value
					Irms[0]=CT1;						// Set the Irms value, so the LCD update is instant
				}
			}
		}
		else 											// We are curently not in Submenu.
		{
			SubMenu=1;									// Enter Submenu now
			if (LCDNav==MENU_CAL)						// CT1 calibration start
			{
				CT1=(unsigned int)Irms[0];				// make working copy of CT1 value
				CT1old=CT1;								// and a backup
			}
			else if (LCDNav==MENU_EXIT)					// Exit Main Menu
			{
				LCDNav=0;			
				SubMenu=0;
				PORTCbits.RC0 = 0;						// LCD backlight off
				Error=NO_ERROR;							// Clear Errors
				write_settings();						// Write to eeprom
				LCD();
			}
		}
		ButtonRelease=1;
	}	
	else if (Buttons==0x7)		// Buttons released
	{
		ButtonRelease=0;
	}	

//
// here we update the LCD
//
	if (LCDNav==1)
	{
		LCD_write(0x80);
		LCD_print((const far rom char *)"Hold 2s ");
		LCD_write(0xC0);
		LCD_print((const far rom char *)"for Menu");
	}
	else if (LCDNav==MENU_CONFIG)
	{
		LCD_print_menu(StrConfig,0x80);			// add navigation arrows on both sides
		if (Config)	LCD_print_menu(StrFixed,0xC0);	// add spaces on both sides
		else LCD_print_menu(StrSocket,0xC0);
	}
	else if (LCDNav==MENU_MODE)
	{ 
		LCD_print_menu(StrMode,0x80);
		if (Mode) LCD_print_menu(StrSmart,0xC0);
		else LCD_print_menu(StrNormal,0xC0);
	}
	else if (LCDNav==MENU_LOADBL)
	{ 
		LCD_print_menu(StrLoadBl,0x80);
		if (LoadBl==0) LCD_print_menu(StrDisable,0xC0);
		else if (LoadBl==1) LCD_print_menu(StrMaster,0xC0);
		else if (LoadBl==2) LCD_print_menu(StrSlave1,0xC0);
		else if (LoadBl==3) LCD_print_menu(StrSlave2,0xC0);
		else LCD_print_menu(StrSlave3,0xC0);
	}
	else if (LCDNav==MENU_MAINS)
	{ 
		LCD_print_menu(StrMains,0x80);
		LCD_print_Amps(MaxMains);
	}
	else if (LCDNav==MENU_MAX)
	{ 
		LCD_print_menu(StrMax,0x80);
		LCD_print_Amps(MaxCurrent);
	}
	else if (LCDNav==MENU_MIN)
	{ 
		LCD_print_menu(StrMin,0x80);
		LCD_print_Amps(MinCurrent);
	}
	else if (LCDNav==MENU_LOCK)
	{
		LCD_print_menu(StrLock,0x80);
		LCD_write(0xC0);
		if (Lock==1) LCD_print_menu(StrSolenoid,0xC0);
		else if (Lock==2) LCD_print_menu(StrMotor,0xC0);
		else LCD_print_menu(StrDisable,0xC0);
	}
	else if (LCDNav==MENU_CABLE)
	{ 
		LCD_print_menu(StrCable,0x80);
		LCD_print_Amps(CableLimit);
	}
	else if (LCDNav==MENU_CAL)
	{ 
		LCD_print_menu(StrCal,0x80);
		LCD_write(0xC0);
		PORTCbits.RC3 = 1;					// LCD RS 
		if (SubMenu) 
		{
			LCD_write('<');
			LCD_write(' ');
			LCD_write( (unsigned char)(CT1/100)+0x30 );
			LCD_write( (unsigned char)(CT1%100)/10+0x30 );
			LCD_write('.');
			LCD_write( (unsigned char)(CT1%10)+0x30 );
			LCD_write('A');
			LCD_write('>');
		}
		else 
		{
			LCD_write(' ');
			LCD_write(' ');
			LCD_write( (unsigned char)((unsigned int)Irms[0]/100)+0x30 );
			LCD_write( (unsigned char)((unsigned int)Irms[0]%100/10)+0x30 );
			LCD_write('.');
			LCD_write( (unsigned char)((unsigned int)Irms[0]%10)+0x30 );
			LCD_write('A');
			LCD_write(' ');
		}
		PORTCbits.RC3 = 0;					// LCD RS 
	}
	else if (LCDNav==MENU_EXIT)
	{
		LCD_write(0x80);
		LCD_print((const far rom char *)"< EXIT >");
		LCD_write(0xC0);
		LCD_print((const far rom char *)"  MENU  ");
	}

	ScrollTimer=Timer;						// reset timer for HelpMenu text
	LCDpos=8;								// reset position of scrolling text
	OldButtonState=Buttons;
	LCDTimer=0;

}



void LCD_init(void)						// initialize the LCD
{
	unsigned char x;

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

	LCD_write(0x80);						// address 00H / first row
	LCD_print((const far rom char *)"Version ");
	LCD_write(0xC0);						// address 40H / second row
	LCD_print((const far rom char *)VERSION); 
	LCD_print((const far rom char *)"    ");
}

