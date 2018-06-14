/* =============================================================================
  SDCC MSX-DOS TEXTMODE Functions Library (object type)
  Version: 1.2 (5/5/2018)
  Author: mvac7/303bcn
  Architecture: MSX
  Format: C Object (SDCC .rel)
  Programming language: C
  WEB: 
  mail: mvac7303b@gmail.com

  Description:
    Opensource library with functions to work in text mode in Screen 0 (40/80
    columns), and Screen 1.
     
    This library uses the functions of the BIOS designed to print text on the 
    screen, but they are called through the interslot (CALSLT), to be used in 
    MSXDOS applications.

    Use them for developing MSX applications using Small Device C Compiler (SDCC) 
    compilator.
============================================================================= */
#include "../include/textmode.h"

#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"


char CSTATE;



/* =============================================================================
 SCREEN0
 
 Description: 
           Switch to T1 or T2 mode (SCREEN 0), 40 or 80 columns x 24 lines.
           Notice: To set the T2 mode, you must first set 80 columns with the 
           WIDTH instruction.
 Input:    -
 Output:   -
============================================================================= */
void SCREEN0()
{
__asm

  push IX
  
  ld   A,(#LINLEN)
  ld   (#LINL40),A   ;copy columns seting with WIDTH to LINL40 system var
   
  ld   IX,#INITXT
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
    
  pop  IX
  
__endasm;
}



/* =============================================================================
 SCREEN1
 
 Description: 
           Switch to G1 mode (SCREEN 1), 32 columns x 24 lines.
 Input:    -
 Output:   -
============================================================================= */
void SCREEN1()
{
__asm
     
  push IX
  
  ld   A,(#LINLEN)   ;get a last value set with WIDTH function 
  ld   (#LINL32),A   ;set system variable
   
  ld   IX,#INIT32
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
    
  pop  IX
  
__endasm;
}



/* =============================================================================
 WIDTH
 
 Description: 
           Specifies the number of characters per line in text mode.
 Input:     1 to 40 in T1 40 columns mode
           41 to 80 in T2 80 columns mode (only MSX with V9938 and a BIOS that 
                                           supports this mode)
            1 to 32 in G1 mode
 Output:   - 
============================================================================= */
void WIDTH(char columns)
{
columns;
__asm
  push IX
  ld   IX,#0
  add  IX,SP

  ld   A,4(IX)
  ld   (#LINLEN),A
  
  pop  IX
__endasm;  
}



/* =============================================================================
 COLOR
 
 Description: 
           Specifies the colors of the foreground, background, and border area.
 Input:    (char) ink (0 to 15)
           (char) background (0 to 15)
           (char) border (0 to 15)
 Output:   -
============================================================================= */
void COLOR(char ink, char background, char border)
{
ink;background,border;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   IY,#FORCLR
  ld   A,4(IX)
  ld   (IY),A

  ld   A,5(IX)
  ld   1(IY),A
  
  ld   A,6(IX)
  ld   2(IY),A 
  
  ld   IX,#CHGCLR
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
  
  pop  IX
__endasm;
}



/* =============================================================================
 CLS
 
 Description: 
           Clear the contents of the screen.
           Fill screen map with 0x20 character.
 Input:    -        
 Output:   -
============================================================================= */
void CLS()
{
__asm

  push IX
  
  xor  A
   
  ld   IX,#BCLS
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
    
  pop  IX
  
__endasm;
}



/* =============================================================================
 LOCATE
 
 Description: 
           Moves the cursor to the specified location.
 Input:    (char) Position X of the cursor. (0 to 31 or 79)
           (char) Position Y of the cursor. (0 to 23)         
 Output:   -
============================================================================= */
void LOCATE(char x, char y)
{
x;y;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX) ;x
  inc  A       ;incrementa las posiciones para que se situen correctamente en la pantalla
  ld   H,A
  ld   A,5(IX) ;y
  inc  A
  ld   L,A
     
  ld   IX,#POSIT
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
  
  pop  IX
__endasm;
}



/* =============================================================================
 PRINT
  
 Description: 
           Displays a text string on the screen.             
                        
 Input:    (char*) String    
 Output:   -
 Notes:
            Supports escape sequences:
             \a (0x07) - Beep
             \b (0x08) - Backspace. Cursor left, wraps around to previous line, 
                         stop at top left of screen.
             \t (0x09) - Horizontal Tab. Tab, overwrites with spaces up to next 
                         8th column, wraps around to start of next line, scrolls
                         at bottom right of screen.
             \n (0x0A) - Newline > Line Feed and Carriage Return (CRLF) 
                         Note: CR added in this Lib.
             \v (0x0B) - Cursor home. Place the cursor at the top of the screen.
             \f (0x0C) - Formfeed. Clear screen and place the cursor at the top. 
             \r (0x0D) - CR (Carriage Return)
            
             \" (0x22) - Double quotation mark
             \' (0x27) - Single quotation mark
             \? (0x3F) - Question mark
             \\ (0x5C) - Backslash
============================================================================= */
void PRINT(char* text)
{
  char character;
  
  while(*(text)) 
  {
    character=*(text++);
    if (character=='\n')
    {
      bchput(10); //LF (Line Feed)
      bchput(13); //CR (Carriage Return)
    }else{ 
      bchput(character);
    } 
  }
}



/* =============================================================================
 PrintNumber

 Description: 
           Prints an unsigned integer on the screen.  
 Input:    (unsigned int) numeric value          
 Output:   -
============================================================================= */
void PrintNumber(unsigned int value)
{
  char character;
  char text[]="     ";
  char *p;	

  num2Dec16(value, text,32); 
  
  p = text;  
  
  while(*(p))
  {
    character=*(p++);
    if (character!=32) bchput(character);
  }   
}



/* =============================================================================
 PrintFNumber

 Description: 
           Prints an unsigned integer on the screen with formatting parameters.
 Input:    (unsigned int) numeric value
           (char) empty Char: (32=' ', 48='0', etc.)
           (char) length: 1 to 5          
 Output:   -
============================================================================= */
void PrintFNumber(unsigned int value, char emptyChar, char length)
{
  char pos=5;
  char text[]="     ";

  num2Dec16(value, text,emptyChar); //32=space, 48=zero 
  
  if(length>5) length=5;
  if(length==0) length=5;
  //coloca el puntero en la posicion donde se ha de empezar a mostrar 
  pos-=length;
  
  // muestra el numero en la pantalla
  while (length-->0){ bchput(text[pos++]);}
}




/* =============================================================================
 num2Dec16
 
 Description: 
           16-bit Integer to ASCII (decimal)
           Based on the code by baze.
 Input:    (unsigned int) a number
           (char*) Address where the output string is provided.
           (char) empty Char: 32=space, 48=zero
============================================================================= */
void num2Dec16(unsigned int aNumber, char *address, char emptyChar)
{
  aNumber;
  address;
  emptyChar;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   L,4(ix)
  ld   H,5(ix)
  
;if (HL<0) Print "-" 
  
  ld   E,6(ix)
  ld   D,7(ix)
  
  ld   A,8(ix)   ;#32
  ld   (#_CSTATE),A
  	
  ld   BC,#-10000
	call $Num1
	ld   BC,#-1000
	call $Num1
	ld	 BC,#-100
	call $Num1
	ld	 C,#-10
	call $Num1
  ;Last figure
	ld	 C,B
  ld   A,#48
  ld   (#_CSTATE),A
	call $Num1

;END
  pop  IX
  ret
    
$Num1:	
  ld	 A,#47 ; 0 ASCII code - 1
   
$Num2:	
  inc	 A
	add	 HL,BC
	jr	 C,$Num2
	
	sbc	 HL,BC
	
	cp   #48   ;"0" ASCII code    
	jr   NZ,$Num3  ;if A!=48 then goto $Num3
	
	ld   A,(#_CSTATE)
  jr   $Num4


$Num3:
  ;change space for 0 zero ASCII code
  push AF
  ld   A,#48
  ld   (#_CSTATE),A
  pop  AF	
	
$Num4:	
	ld	 (DE),A   ;save a ascii code
	inc	 DE
		
	ret
  
$Num5:
    
__endasm;
}



/* =============================================================================
 bchput
 
 Description: 
         Displays one character (MSX BIOS CHPUT).
 Input:   (char) text char
 Output:  -
============================================================================= */
void bchput(char value)
{
value;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(IX)
  
  ld   IX,#CHPUT
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
  
  pop  IX
__endasm;
}







/* =============================================================================
   Current row-position of the cursor
============================================================================= */
/*char GetRow()
{
__asm
  ld   HL,#CSRY
  ld   L,(HL)
__endasm;
} */



/* =============================================================================
Current column-position of the cursor
============================================================================= */
/*char GetColumn()
{
__asm
  ld   HL,#CSRX
  ld   L,(HL)
__endasm;
} */



/* =============================================================================
   Displays the function keys
   Visualiza las teclas de función en la pantalla
============================================================================= */
/*void KEYON()
{
__asm
  
  push IX
   
  ld   IX,#DSPFNK
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
    
  pop  IX
  
__endasm;
}*/



/* =============================================================================
   Erase functionkey display
   Oculta las teclas de función en la pantalla
============================================================================= */
/*void KEYOFF()
{
__asm

  push IX
   
  ld   IX,#ERAFNK
  ld   IY,(EXPTBL-1)
  call CALSLT
  ei
    
  pop  IX     
  
__endasm;
}*/