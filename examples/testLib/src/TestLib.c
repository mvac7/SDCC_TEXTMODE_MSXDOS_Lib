/* =============================================================================
	Test TEXTMODE MSXDOS Library v1.5 (fR3eL Project)
	Version: 1.4 (28/11/2023)
	Author: mvac7/303bcn [mvac7303b@gmail.com]
	Architecture: MSX
	Format: .COM (MSXDOS)
	Programming language: C and Assembler
	Compiler: SDCC 4.3 or newer 
	Description:
		Test Textmode MSXDOS library

	History of versions:
	- v1.4 (28/11/2023) update to SDCC (4.3) and textmode v1.5 library
	- v1.3 ( 7/05/2018) 
	- v1.2 (27/03/2017)
	- v1.1 (27/02/2017)
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVariables.h"
#include "../include/msxBIOS.h"
#include "../include/msxDOS.h"

#include "../include/textmode_MSX.h"


#define  HALT __asm halt __endasm   //wait for the next interrupt




// Function Declarations -------------------------------------------------------
void System(char code);

char ReadinBIOS(uint addr);

char PEEK(uint address);

char VPEEK(uint address);

char INKEY(void);

void WAIT(uint cicles);

void test_SC0(void);
void test_SC1(void);

void testWIDTH(void);
void testPRINT(void);
void testPrintNumber(void);
void testCLS(void);

void PressAnyKey(void);

void PrintExtendedGFXchar(char A);
void PrintLine(char size);
void DrawBox(char width, char height);

void SetG1colors(char octet, char INKcolor,char BGcolor);

char isText1Mode(void);




// constants  ------------------------------------------------------------------
const char text01[] = "Test Textmode Lib v1.5";

const char text_32col[] = "----5----1----1----2----2----3--         0    5    0    5    0  ";
const char text_40col[] = "----5----1----1----2----2----3----3----4         0    5    0    5    0    5    0";

const char text_LF[] = "\n"; // LF line Feed
const char text_CR[] = "\r"; // CR Carriage Return

const char testString[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";

const char presskey[] = "Press any key to continue";
// global variable definition --------------------------------------------------




// ------------------------------------------------------------------- Functions


void main(void)
{
  char colorInk;
  char colorBG;
  char colorBDR;
  char scrcolumns;
  //char MSXsion=0;

  colorInk=PEEK(FORCLR);
  colorBG=PEEK(BAKCLR);
  colorBDR=PEEK(BDRCLR);
  scrcolumns=PEEK(LINLEN);
  
  
  COLOR(WHITE,DARK_BLUE,BLACK);
  WIDTH(32);
  SCREEN1();

  SetG1colors(2,GRAY,DARK_BLUE);
  SetG1colors(3,GRAY,DARK_BLUE);

  LOCATE(2,10);
  DrawBox(28, 3);
  LOCATE(5,11);
  PRINT(text01);

//LOCATE(7,12);
//PRINT(text02);

  PressAnyKey(); 
	  
  test_SC1(); //G1 32c

  test_SC0(); //T1 40c



//EXIT MSXDOS ------------------------------------------------------------------
  //put the screen as it was.
  COLOR(colorInk,colorBG,colorBDR);
  WIDTH(scrcolumns);

  if(scrcolumns<33) SCREEN1();
  else SCREEN0();
  //

  PrintLN("END");

  System(DOS_TERM0);
//--------------------------------------------------------------------- end EXIT	
}




/*
 call system functions
 see MSX Assembly Page > MSX-DOS 2 function calls
 http://map.grauw.nl/resources/dos2_functioncalls.php
*/
void System(char code) __naked
{
code;
__asm

  ld  C,A
  jp  BDOS  // MSX-DOS entry

__endasm;
}




/* =============================================================================
   Reads a value from memory.
============================================================================= */
char PEEK(uint address) __naked
{
address;
__asm

	ld   A,(HL)

	ret
__endasm;
}



/*void POKE(char value, uint address) __naked
{
value;
address;
__asm

  ld   (DE),A
  
  ret
__endasm;
}*/



char VPEEK(uint address) __naked
{
address;
__asm
  push IX

  ld   IX,#BIOS_RDVRM
  ld   IY,(#EXPTBL-1)
  call BIOS_CALSLT
  ei

  pop  IX
  ret
__endasm;
}




/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY(void) __naked
{
__asm
  push IX

  ld   IX,#BIOS_CHGET
  ld   IY,(#EXPTBL-1)
  call BIOS_CALSLT
  ei

  pop  IX
  ret
__endasm;
}



/* =============================================================================
Read one byte in BIOS. For reading system constants in MSXDOS environment.
============================================================================= */
char ReadinBIOS(uint addr) __naked
{
addr;
__asm
  push IX

  ld   A,(#EXPTBL)
  call BIOS_RDSLT
  ei

  pop  IX
  ret
__endasm;
}



/* =============================================================================
   Generates a pause in the execution of n interruptions.
   PAL: 50=1second. ; NTSC: 60=1second. 
============================================================================= */
void WAIT(uint cicles)
{
	uint i;
	for(i=0;i<cicles;i++) HALT;
}




// ############################################################### TEST functions

/* =============================================================================
   Test set in TEXT 1 mode (Screen 0/40col)
============================================================================= */
void test_SC0(void)
{
	COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);
	WIDTH(40);
	SCREEN0();
	
	//LOCATE(0,0);  
	PrintLN("Test TEXT 1 mode (Screen 0)");

	testWIDTH();
	
	testPRINT();

	testPrintNumber();

	testCLS();

	PressAnyKey();
}



/* =============================================================================
   Test set in Graphic 1 mode (Screen 0)
============================================================================= */
void test_SC1(void)
{
	COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
	WIDTH(32);
	SCREEN1();  
	
	//LOCATE(0,0);
	PrintLN("Test GRAPHIC 1 mode (Screen 1)");
	
	SetG1colors(0,LIGHT_YELLOW,DARK_BLUE);
	SetG1colors(2,CYAN,DARK_BLUE);
	SetG1colors(3,CYAN,DARK_BLUE);

	testWIDTH();
	
	testPRINT();

	testPrintNumber();

	testCLS();

	PressAnyKey();
}



/* =============================================================================

============================================================================= */
void testWIDTH(void)
{
	char columns = GetColumns();
	
	PrintLine(columns);
   
	PRINT("Test WIDTH - Columns=");
	PrintNumber(columns);

	PRINT(text_LF);

	if(columns<40) PRINT(text_32col);  
	else PRINT(text_40col);
}



/* =============================================================================
   Test the functions to print texts. (LOCATE, PRINT and PrintLN)
============================================================================= */
void testPRINT(void)
{
	//char cursorX;
	char cursorY;
	char columns = GetColumns();
	
	PrintLine(columns);
	
	PrintLN("Test PRINT and PrintLN");
	PRINT(testString);
	
	PRINT(text_LF);	// CR Carriage Return	
	PrintLine(columns);
	
	PrintLN("Test PRINT Escape Sequences:");
	PRINT("\t<tab>Line 1\n\t<tab>Line 2\n\t<tab>Line 3"); // \t Horizontal Tab
	PRINT(text_CR);	// CR Carriage Return
	PRINT(">CR"); 
	PRINT(text_LF);	// LF line Feed
	
	//PRINT("\v");	// Place the cursor at the top of the screen
	
	cursorY = GetCursorRow(); 
	LOCATE(0,cursorY);
	DrawBox(columns, 7);

/*	
	LOCATE(1,10);
	cursorX = GetCursorColumn();
	cursorY = GetCursorRow();
	
	LOCATE(1,19);
	PRINT("cursorX");
	PrintNumber(cursorX);
	
	LOCATE(1,20);
	PRINT("cursorY");
	PrintNumber(cursorY);*/
	
	
	cursorY++;
	LOCATE(1,cursorY++);
	PRINT("\1\x42 <-- extended graphic chars");  // print extended graphic characters (0x42 = smile)
	LOCATE(1,cursorY++);
	PRINT("\\ <-- Backslash");  // Backslash
	LOCATE(1,cursorY++);
	PRINT("\' <-- Single quotation mark");  // Single quotation mark
	LOCATE(1,cursorY++);
	PRINT("\" <-- Double quotation mark");         // \" Double quotation mark
	LOCATE(1,cursorY);
	PRINT("\? <-- Question mark");  // Question mark
	
	PressAnyKey();
	PrintLN("\f <-- Formfeed (clear screen)");	// Formfeed (CLS)
	PRINT("\a [Beep!]");  // Beep
	PressAnyKey();
}



/* =============================================================================
   Test the functions to print numbers. (PrintNumber and PrintFNumber)
============================================================================= */
void testPrintNumber(void)
{
	char charValue=42;
	unsigned int uintValue=12345;
	char columns = GetColumns();
	
	CLS();

	PrintLN("Test Print Numbers");
	PrintLine(columns);
	
	PrintLN("Variables:");
	
	PrintLN("char value = 42");
	PrintLN("uint value = 12345");
		
	PRINT(text_LF);
	
	PrintLine(columns);

	PRINT(">PrintNumber(7)   :");
	PrintNumber(7);
	
	PRINT("\n>PrintNumber(2400):"); 
	PrintNumber(2400);

	PRINT("\n>PrintNumber(uint):");
	PrintNumber(uintValue);

	PRINT("\n>PrintNumber(char):");
	PrintNumber(charValue);

	PRINT(text_LF);

	PRINT("\n>PrintFNumber(2400,32,6) :");
	PrintFNumber(2400,32,6); //" 2400" Testing when the length parameter is above the maximum size (5) 

	PRINT("\n>PrintFNumber(uint,32,5) :");
	PrintFNumber(uintValue,32,5); //"12345"

	PRINT("\n>PrintFNumber(12345,0,3) :");
	PrintFNumber(12345,0,3); //"345"

	PRINT("\n>PrintFNumber(7,0,3)     :");
	PrintFNumber(7,0,3); //"7"

	PRINT("\n>PrintFNumber(char,' ',3):");
	PrintFNumber(charValue,' ',3); //" 42"

	PRINT("\n>PrintFNumber(7,48,3)    :");
	PrintFNumber(7,48,3);  //"007"

	PRINT("\n>PrintFNumber(char,'0',4):");
	PrintFNumber(charValue,'0',4); //"0042"
	
	PRINT("\n>PrintFNumber(char,'_',5):");
	PrintFNumber(charValue,'_',5); //"___42"

	PressAnyKey();
}



/* =============================================================================
   Test the CLS (Clear Screen) function.
============================================================================= */
void testCLS(void)
{
	uint VRAMaddr;
	
	CLS();

	//PRINT("Test ERROR and 0,0");
	
	LOCATE(0,21);
	PRINT(">Test CLS() ");
	
	if(isText1Mode()==true) VRAMaddr=0x0000;
	else VRAMaddr=0x1800;
	
	if (VPEEK(VRAMaddr)!=32) PRINT(">>> ERROR <<<");  // ">"=62
	else PRINT("> OK");
}



/* =============================================================================
   Print a message on line 24 and wait for a key press.
============================================================================= */
void PressAnyKey(void)
{	
	LOCATE(0,23);
	PRINT(presskey);
	INKEY();	
}



/* =============================================================================
   Print an Extended Graphic char
============================================================================= */
void PrintExtendedGFXchar(char A)
{
	bchput(1);
	bchput(A);
}



/* =============================================================================
   Print a line.
============================================================================= */
void PrintLine(char size)
{
	char i;
	
	for(i=0;i<size;i++) PrintExtendedGFXchar(0x57);	
}



/* =============================================================================
   Prints a box from the cursor position.
============================================================================= */
void DrawBox(char width, char height)
{
	char i;
	char box_winside = width-2;
	
	char x = GetCursorColumn();
	char y = GetCursorRow();
	
/*  PrintLN("\1\x58\1\x57\1\x57\1\x59");
	PrintLN("\1\x56  \1\x56");
	PrintLN("\1\x5A\1\x57\1\x57\1\x5B");*/
	
	PrintExtendedGFXchar(0x58);
	PrintLine(box_winside);
	PrintExtendedGFXchar(0x59);
	
	width--;
	
	for(i=y+1;i<y+(height-1);i++)
	{
		LOCATE(x,i);
		PrintExtendedGFXchar(0x56);
		LOCATE(x+width,i);
		PrintExtendedGFXchar(0x56);
	}
	
	LOCATE(x,y+height-1);
	PrintExtendedGFXchar(0x5A);
	PrintLine(box_winside);
	PrintExtendedGFXchar(0x5B);	
}



/* =============================================================================
 SetG1colors

 Description: 
			Assigns colors to a group of GRAPHIC1 tiles.
			MSX-DOS environment.
		   
 Input:		(char) Octet. Group of 8 tiles.
			(char) Ink color (0-15)
			(char) Background color (0-15)      
 Output:   -
============================================================================= */
void SetG1colors(char octet, char INKcolor,char BGcolor)
{
octet;		//A
INKcolor;	//L
BGcolor;	//Stack	
__asm
  push IX
  ld   IX,#0
  add  IX,SP
	
  ld   B,L	
	
  ld   HL,#0x2000
  ld   D,#0
  ld   E,A
  add  HL,DE
	
  ld   C,4(IX)
  ld   A,B
  SLA  A
  SLA  A
  SLA  A
  SLA  A	
  or   C	
	
  ld   IX,#0x004D     ;MSX BIOS   WRTVRM Writes data in VRAM
  ld   IY,(#0xFCC0)   ;System var EXPTBL-1 (FCC1h-1) main BIOS-ROM slot address
  call 0x001C         ;MSX BIOS   CALSLT Executes inter-slot call
  ei
	
  pop  IX
__endasm;	
}



/* =============================================================================
   Indicates whether Text 1 mode is active.
   Output:	1=Yes/True ; 0=No/False
============================================================================= */
char isText1Mode(void)
{
	char *A;
	A=(unsigned int *) RG1SAV;
	if (*A&0b00010000) return 1; //Text 40col Mode
	return 0;	
}