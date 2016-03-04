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
				case 10:
						x=strlenpgm(MenuConfig);
						LCD_print8(MenuConfig+LCDpos);
						break;
				case 20:
						x=strlenpgm(MenuMode);
						LCD_print8(MenuMode+LCDpos);
						break;
				case 30:
						x=strlenpgm(MenuMains);
						LCD_print8(MenuMains+LCDpos);
						break;
				case 40:
						x=strlenpgm(MenuMax);
						LCD_print8(MenuMax+LCDpos);
						break;
				case 50:
						x=strlenpgm(MenuMin);
						LCD_print8(MenuMin+LCDpos);
						break;
				case 60:
						x=strlenpgm(MenuLock);
						LCD_print8(MenuLock+LCDpos);
						break;
				case 70:
						x=strlenpgm(MenuCable);
						LCD_print8(MenuCable+LCDpos);
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
			if (Mode)						// Smart Mode?
			{
				LCD_write('(');				
				LCD_write( (unsigned char)(Ilimit/100)+0x30 );
				LCD_write( (unsigned char)((Ilimit/10)%10)+0x30 );
				LCD_write('A');
				LCD_write(')');
			} else LCD_print((const far rom char *)"     ");	// clear rest of row

			PORTCbits.RC3 = 0;	
		}

}


//##############################################################################################################################
// 10 CONFIG			- Set to Fixed Cable or Type 2 Socket
// 20 MODE  			- Set to Smart mode, or Normal EVSE mode
// 30		MAINS 		- Set max MAINS Current (25-100) (Mode=Smart)
// 40 MAX   			- Set MAX Charge Current for the EV (16-80)
// 50		MIN   		- Set MIN Charge Current the EV will accept (Mode=Smart)
// 60 LOCK				- Cable lock Enable/disable
// 70 CABLE				- Set Fixed Cable Current limit 
// 80 EXIT				- Exit Menu

// unimplemented (80		CAL   		- Calibrate CT1)

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
const far rom char StrEnable[] =	"Enable";
const far rom char StrDisable[] =	"Disabl";
const far rom char StrCable[] = 	" CABLE";

void LCDMenu(unsigned char Buttons)
{
	static unsigned long ButtonTimer=0;
	static unsigned char ButtonRelease=0;			// keeps track of LCD Menu Navigation
	
// Main Menu Navigation

	if ((LCDNav==0) && (Buttons==0x5) && (ButtonRelease==0))		// Button 2 pressed ?
	{
		LCDNav=1;							// about to enter menu
		ButtonTimer=Timer;					
	}
	else if  (LCDNav==1 && ((ButtonTimer+2000)<Timer))		// <CONFIG>
	{
		LCDNav=10;							// Main Menu entered
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
			case 10:
					if (SubMenu)
					{
						if (Config) Config=0;
						else Config=1;
					} 
					else LCDNav=20;
					break;
			case 20:
					if (SubMenu)
					{
						if (Mode) Mode=0;
						else Mode=1;
					} 
					else 
					{
						if (Mode) LCDNav=30;					// Smart Mode?
						else LCDNav=40;
					}
					break;
			case 30:
					if (SubMenu)
					{
						MaxMains++;								// Set new MaxMains
						if (MaxMains>100) MaxMains=100;			// Max 100A
						Ilimit=MaxMains*10;						// Update Ilimit
					}
					else LCDNav=40;
					break;
			case 40:
					if (SubMenu)
					{
						MaxCurrent++;								// Set new MaxCurrent
						if (MaxCurrent>80) MaxCurrent=80;			// Max 80A
					}
					else
					{
						if (Mode) LCDNav=50;					// Smart Mode?
						else if (Config) LCDNav=70;				// Cable Configuration, go to Cable Current
						else LCDNav=60;							// Fixed Cable, use the lock
					}
					break;
			case 50:
					if (SubMenu)
					{
						MinCurrent++;								// Set new MinCurrent
						if (MinCurrent>16) MinCurrent=16;			// Max 16A
					}
					else
					{
						if (Config) LCDNav=70;					// Cable Configuration, go to Cable Current
						else LCDNav=60;							// Fixed Cable, use the lock
					}
					break;
			case 60:
					if (SubMenu)
					{
						if (Lock) Lock=0;
						else Lock=1;
						break;
					} 
		
			case 70:
					if (SubMenu)
					{
						CableLimit++;								// Set new CableLimit
						if (CableLimit>80) CableLimit=80;			// Max 80A
					}
					else LCDNav=80;
					break;
			case 80:
					LCDNav=10;
			default:
					break;
		}
		ButtonRelease=1;
	}
	else if ( (LCDNav>0) && ((LCDNav%10)==0) && (Buttons==0x6) && (ButtonRelease==0) )		// Button 3 < pressed 
	{
		switch (LCDNav)
		{
			case 80:
					if (Config) LCDNav=70;					// Cable Configuration, go to Cable Current
					else LCDNav=60;							// Fixed Cable, use the lock
					break;
			case 70:
					if (SubMenu)
					{
						CableLimit--;								// Set new CableLimit
						if (CableLimit<13) CableLimit=13;			// Min 13A
						break;
					}
			case 60:
					if (SubMenu)
					{
						if (Lock) Lock=0;
						else Lock=1;
					} 
					else
					{
						if (Mode) LCDNav=50;						// Smart Mode?
						else LCDNav=40;
					}
					break;
			case 50:
					if (SubMenu)
					{
						MinCurrent--;								// Set new MinCurrent
						if (MinCurrent<6) MinCurrent=6;				// Min 6A
					}
					else LCDNav=40;
					break;
			case 40:
					if (SubMenu)
					{
						MaxCurrent--;								// Set new MaxCurrent
						if (MaxCurrent<10) MaxCurrent=10;			// Min 10A
					}
					else
					{
						if (Mode) LCDNav=30;					// Smart Mode?
						else LCDNav=20;
					}
					break;
			case 30:
					if (SubMenu)
					{
						MaxMains--;								// Set new MaxMains
						if (MaxMains<25) MaxMains=25;			// Min 25A
						Ilimit=MaxMains*10;						// Update Ilimit
					} else LCDNav=20;
					break;
			case 20:
					if (SubMenu)
					{
						if (Mode) Mode=0;
						else Mode=1;
					} 
					else LCDNav=10;
					break;
			case 10:
					if (SubMenu)
					{
						if (Config) Config=0;
						else Config=1;
					} 
					else LCDNav=80;
			default:
					break;
		}
		ButtonRelease=1;
	}
	else if (LCDNav>=10 && Buttons==0x5 && ButtonRelease==0)		// Button 2 pressed?
	{
		if (SubMenu) SubMenu=0;
		else 
		{
			SubMenu=1;							// Enter or Exit Submenu
			if (LCDNav==80)						// Exit Main Menu
			{
				LCDNav=0;			
				SubMenu=0;
				PORTCbits.RC0 = 0;				// LCD backlight off
				Error=NO_ERROR;					// Clear Errors
				write_settings();				// Write to eeprom
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
	else if (LCDNav==10)
	{
		LCD_print_menu(StrConfig,0x80);			// add navigation arrows on both sides
		if (Config)	LCD_print_menu(StrFixed,0xC0);	// add spaces on both sides
		else LCD_print_menu(StrSocket,0xC0);
	}
	else if (LCDNav==20)
	{ 
		LCD_print_menu(StrMode,0x80);
		if (Mode) LCD_print_menu(StrSmart,0xC0);
		else LCD_print_menu(StrNormal,0xC0);
	}
	else if (LCDNav==30)
	{ 
		LCD_print_menu(StrMains,0x80);
		LCD_print_Amps(MaxMains);
	}
	else if (LCDNav==40)
	{ 
		LCD_print_menu(StrMax,0x80);
		LCD_print_Amps(MaxCurrent);
	}
	else if (LCDNav==50)
	{ 
		LCD_print_menu(StrMin,0x80);
		LCD_print_Amps(MinCurrent);
	}
	else if (LCDNav==60)
	{
		LCD_print_menu(StrLock,0x80);
		LCD_write(0xC0);
		if (Lock) LCD_print_menu(StrEnable,0xC0);
		else LCD_print_menu(StrDisable,0xC0);
	}
	else if (LCDNav==70)
	{ 
		LCD_print_menu(StrCable,0x80);
		LCD_print_Amps(CableLimit);
	}
	else if (LCDNav==80)
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

