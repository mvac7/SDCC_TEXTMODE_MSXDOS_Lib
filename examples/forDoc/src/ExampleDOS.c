/* =====================================================
   Example TEXTMODE MSX-DOS Library (fR3eL Project)
======================================================== */

#include "../include/textmode_MSX.h"

const char text01[] = "Example TEXTMODE Lib\n";

void main(void)
{
  unsigned int uintValue=1234;
  char charValue=71;
  
  PrintLN(text01);
  
  PRINT("Line 1\n");
  PrintLN("Line 2");
  PrintLN("Line 3\n");
    
  PRINT("\1\x42");   //print smile (2 + 64) = 42 hexadecimal
  PRINT("\n");
  
  PRINT("\n>PrintNumber:");
  PrintNumber(1024);
  
  PRINT("\n>PrintFNumber:");
  PrintFNumber(charValue,'0',4); //"0071"
  
  PRINT("\n>Print Integer:");
  PrintFNumber(uintValue,32,5); //" 1234"
  
  PRINT("\n>Print cut number:");
  PrintFNumber(uintValue,32,2); //"34"
  
  PrintLN("\n");
  PRINT("End");
  
__asm

  ld  C,#0    // TERM0 Program terminate
  jp  0x0005  // MSX-DOS entry

__endasm;
}
