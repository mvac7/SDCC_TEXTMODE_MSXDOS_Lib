/* =============================================================================
	Test 80 columns TEXTMODE MSXDOS Library v1.5 (fR3eL Project)
	Version: 1.0 (28/10/2024)
	Author: mvac7/303bcn
	Architecture: MSX
	Format: .COM (MSXDOS)
	Programming language: C and Assembler
	Compiler: SDCC 4.3 or newer      
	Description:
		Test TEXTMODE Library to 80 columns screen mode.
		For MSX computers with V9938 and BIOS that supports 80 columns.

	History of versions:
	- v1.0 (23/10/2024) first version.
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

void test_SC080(void);

void testWIDTH(void);
void testPRINT(void);
void testPrintNumber(void);
void testCLS(void);

void PressAnyKey(void);
void PausePage(void);

void PrintExtendedGFXchar(char A);
void PrintLine(char size);
void DrawBox(char width, char height);



// constants  ------------------------------------------------------------------
const char text01[] = "Test80c Textmode Lib v1.5";

const char text_80col[] = "----5----1----1----2----2----3----3----4----4----5----5----6----6----7----7----8         0    5    0    5    0    5    0    5    0    5    0    5    0    5    0";

const char text_LF[] = "\n"; // LF line Feed
const char text_CR[] = "\r"; // CR Carriage Return

const char testString[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat."; 
// Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

const char presskey[] = "Press a key to continue";
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
  
  PrintLN(text01);
  PressAnyKey();
	  
  test_SC080();

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
   Test set in TEXT 2 mode (Screen 0/80col)
============================================================================= */
void test_SC080(void)
{
	COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);      
	WIDTH(80);
	SCREEN0();

	PrintLN("Test TEXT 2 mode (Screen 0 80col)");

	testWIDTH();

	testPRINT();
	testPrintNumber();

	testCLS();
	
	PausePage();

	return;
}



void testWIDTH(void)
{
	char columns = GetColumns();

	PrintLine(columns);
   
	PRINT("Test WIDTH - Columns=");
	PrintNumber(columns);

	PRINT(text_LF);
	PRINT(text_80col);
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
	
	PausePage();
	PrintLN("\f <-- Formfeed (clear screen)");	// Formfeed (CLS)
	PRINT("\a [Beep!]");  // Beep
	PausePage();
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

	PausePage();
}



/* =============================================================================
   Test the CLS (Clear Screen) function.
============================================================================= */
void testCLS(void)
{
	CLS();   //Comment this, to test an error.
	
	LOCATE(0,21);
	PRINT(">Test CLS() ");
	
	if (VPEEK(0x0000)!=32) PRINT(">>> ERROR <<<"); 
	else PRINT("> OK");
}



/* =============================================================================
   Print a message and wait for a key press.
============================================================================= */
void PressAnyKey(void)
{	
	PRINT(presskey);
	INKEY();	
}


void PausePage(void)
{	
	LOCATE(0,23);
	PressAnyKey();	
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

