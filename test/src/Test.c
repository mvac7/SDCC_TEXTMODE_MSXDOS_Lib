/* =============================================================================
  Test SDCC textmode MSXDOS Library
  v1.3 (7/05/2018)
  Description:
    Test textmode MSX DOS library
    
  History of versions:
  - v1.3 ( 7/05/2018) current version
  - v1.2 (27/03/2017)
  - v1.1 (27/02/2017) 
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"
#include "../include/msxDOS.h"

#include "../include/textmode.h"


#define  HALT __asm halt __endasm   //wait for the next interrupt



// MSX-DOS Calls ---------------------------------------------------------------
#define  SYSTEM 0x0005  // MSX-DOS entry
//#define  RDSLT  0x000C  // Reads the value of an address in another slot
//#define  WRSLT  0x0014  // Writes a value to an address in another slot.
//#define  CALSLT 0x001C  // Executes inter-slot call.
//#define  ENASLT 0x0024  // Switches indicated slot at indicated page on perpetual
//#define  CALLF  0x0030  // Executes an interslot call




// Function Declarations -------------------------------------------------------
void System(char code);

char getMSXver();

char PEEK(uint address);
void POKE(uint address, char value);

char VPEEK(uint address);

char INKEY();

void WAIT(uint cicles);

void test_SC0();
void test_SC080();
void test_SC1();

void testWIDTH();
void testWIDTH80();

void testPRINT();
void testPrintNumber();

void testCLS();



// constants  ------------------------------------------------------------------
const char text01[] = "Test SDCC textmode MSXDOS Lib";
const char text02[] = "v1.3 (7/05/2018)";

const char LoremSortString[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
const char LoremString[] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

const char presskey[] = "Press a key to continue";
// global variable definition --------------------------------------------------


// Functions -------------------------------------------------------------------


//
void main(void)
{
  char colorInk=0;
  char colorBG=0;
  char colorBDR=0;
  char scrcolumns=0;
  //char MSXsion=0;
  
  colorInk=PEEK(FORCLR);
  colorBG=PEEK(BAKCLR);
  colorBDR=PEEK(BDRCLR);
  scrcolumns=PEEK(LINLEN);
  
  PRINT("\n");
  PRINT(text01);
  PRINT("\n");
  PRINT(text02);
  PRINT("\n\n");
  
  PRINT(">Test PRINT():\n");
  PRINT(LoremSortString);
  testPRINT();
  
        
  test_SC0(); //T1

  test_SC1(); //G1
  
//---------------------------------------------------------------------only MSX2 
  if (getMSXver()>0) test_SC080(); //text mode with 80 columns 
//------------------------------------------------------------------------------  


//EXIT MSXDOS ------------------------------------------------------------------
  //put the screen as it was.
  COLOR(colorInk,colorBG,colorBDR);
  WIDTH(scrcolumns);

  if(scrcolumns<33) SCREEN1();
  else SCREEN0();
  //
  
  PRINT("END");
    
  System(_TERM0); 
//--------------------------------------------------------------------- end EXIT

}



// call system functions 
// see MSX Assembly Page > MSX-DOS 2 function calls
// http://map.grauw.nl/resources/dos2_functioncalls.php
void System(char code)
{
code;
__asm
	push IX
	ld   IX,#0
	add  IX,SP

	ld   C,4(IX)
	call SYSTEM

	pop  IX
__endasm; 
}



char PEEK(uint address)
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
    
  ld   L,4(IX)
  ld   H,5(IX)
  ld   L,(HL)

  pop  IX
__endasm;
}



void POKE(uint address, char value)
{
address;value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP 
    
  ld   L,4(IX)
  ld   H,5(IX)
  ld   A,6(IX)
  ld   (HL),A

  pop  IX  
__endasm;
}



char VPEEK(uint address)
{
address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP 
    
  ld   L,4(IX)
  ld   H,5(IX) 
  
  ld   IX,#RDVRM
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
  
  ld   L,A

  pop  IX
__endasm;
}




/* =============================================================================
One character input (waiting)
============================================================================= */
char INKEY()
{
__asm   
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   IX,#CHGET
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
  
  ld   L,A
  pop  IX
__endasm;
}



char getMSXver()
{
__asm   
  push IX
  
  ld   A,(#EXPTBL)
  ld   HL,#MSXVER
  call RDSLT
  ei
  
  ld   L,A
  pop  IX
__endasm;
}




// Generates a pause in the execution of n interruptions.
// PAL: 50=1second. ; NTSC: 60=1second. 
void WAIT(uint cicles)
{
  uint i;
  for(i=0;i<cicles;i++) HALT;
  return;
}




// TEST functions ##############################################################
void test_SC0()
{
  COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);
  WIDTH(40);
  SCREEN0();
  
  LOCATE(0,0);  
  PRINT(">Test SCREEN0()\n");
  
  testWIDTH();
  
  PRINT(">Test PRINT():\n");
  PRINT(LoremSortString);
  testPRINT();
  
  testPrintNumber();

  testCLS();
  LOCATE(0,17);
  if (VPEEK(0x0000)==62) PRINT(">>> ERROR"); 
  else PRINT(">> OK");
  
  LOCATE(0,22);
  PRINT(presskey);
  INKEY();  
}



void test_SC080()
{ 
  COLOR(LIGHT_GREEN,DARK_GREEN,DARK_GREEN);
  WIDTH(80);
  SCREEN0();
  
  LOCATE(0,0);  
  PRINT(">Test SCREEN0() 80 columns\n");
  
  testWIDTH80();
  
  PRINT(">Test PRINT():\n");
  PRINT(LoremString);
  testPRINT();
  
  testPrintNumber();

  testCLS();
  LOCATE(0,17);
  if (VPEEK(0x0000)==62) PRINT(">>> ERROR"); 
  else PRINT(">> OK");
  
  LOCATE(0,22);
  PRINT(presskey);
  INKEY();
}



void test_SC1()
{
  int  intvalue=31416;
  char charValue=255;
  
  COLOR(WHITE,DARK_BLUE,LIGHT_BLUE);
  WIDTH(32);
  SCREEN1(); 
  
  LOCATE(0,0);
  PRINT(">Test SCREEN1()\n");
  
  testWIDTH();
  
  PRINT(">Test PRINT():\n");
  PRINT(LoremSortString);
  testPRINT();
  
  testPrintNumber();
  
  testCLS();
  LOCATE(0,17);
  if (VPEEK(0x1805)==67) PRINT(">>> ERROR");  // "C" 0x43
  else PRINT(">> OK");
  
  LOCATE(0,22);
  PRINT(presskey);
  INKEY();
}



void testWIDTH()
{
  char columns;
  
  columns = PEEK(LINLEN);
  
  PRINT(">Test WIDTH(");
  PrintNumber(columns);
  PRINT(")\n");
  PRINT("----5---10---15---20---25---30---35---40\n");
  PRINT("\n");
}



void testWIDTH80()
{
  char columns;
  
  columns = PEEK(LINLEN);
  
  PRINT(">Test WIDTH(");
  PrintNumber(columns);
  PRINT(")\n");
  PRINT("----5---10---15---20---25---30---35---40---45---50---55---60---65---70---75---80");
  PRINT("\n");
}



void testPRINT()
{
  PRINT("\n\n>Test PRINT() with Escape Sequences:");
  PRINT("\n\tLine 1\n\tLine 2\n\tLine 3"); // \t Horizontal Tab
  PRINT("\r");  // Carriage Return
  PRINT(">\n");
  PRINT("\\");  // Backslash
  PRINT("\a");  // Beep
  PRINT("\n");  // Newline
  //PRINT("\f");  // Formfeed (CLS)
  //PRINT("\v");  // Place the cursor at the top of the screen
  PRINT("\'Single quotation mark\'\n");  // Single quotation mark
  PRINT("\"Double quotation mark\"\n");         // \" Double quotation mark
  PRINT("Question mark\?\n");  // Question mark
  
  PRINT("\n\n");
  PRINT(presskey);
  INKEY();
}



void testPrintNumber()
{
  uint uintvalue=12345;
  char charValue=71;
  
  CLS();
   
  PRINT(">Test PrintNumber(2400):");
  PrintNumber(2400);

  PRINT("\n>Test PrintNumber(uint):");
  PrintNumber(uintvalue);

  PRINT("\n>Test PrintNumber(char):");
  PrintNumber(charValue);

  PRINT("\n>Test PrintNumber(0)   :");
  PrintNumber(0);
  
  PRINT("\n\n");
  
  PRINT(">Test PrintFNumber(2400,32,6):\n");
  PrintFNumber(2400,' ',6); //" 2400" Testing when the length parameter is above the maximum size (5) 

  PRINT("\n>Test PrintFNumber(uint,32,5):\n");
  PrintFNumber(uintvalue,' ',5); //"12345"
  
  PRINT("\n>Test PrintFNumber(uint,32,3):\n");
  PrintFNumber(uintvalue,' ',3); //"345"
  
  PRINT("\n>Test PrintFNumber(7,48,3):\n");
  PrintFNumber(7,48,3);  //"007"

  PRINT("\n>Test PrintFNumber(char,' ',4):\n");
  PrintFNumber(charValue,' ',4); //"  71"
  
  PRINT("\n>Test PrintFNumber(char,'0',4):\n");
  PrintFNumber(charValue,'0',4); //"0071"

  PRINT("\n\n");
  PRINT(presskey);
  INKEY();
}



void testCLS()
{
  CLS();
  LOCATE(0,16);
  PRINT(">Test CLS()");
}
//########################################################### END TEST functions
