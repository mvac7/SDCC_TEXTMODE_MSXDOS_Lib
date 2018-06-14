# SDCC MSX-DOS TEXTMODE Functions Library (object type)

Version: 1.2 (5/5/2018)

Author: mvac7/303bcn

Architecture: MSX

Format: C Object (SDCC .rel)

Programming language: C

WEB:
 
mail: mvac7303b@gmail.com




## 1. Introduction

This project is an Open Source library of functions for creating aplications in text mode.

Works in T1 (screen 0, 40 columns), T2 (screen 0, 80 columns), and G1 (screen 1, 32 columns) modes.

This library uses the functions of the BIOS designed to print text on the
screen, but they are called through the interslot (CALSLT), to be used in MSXDOS applications.

Use them for developing MSX applications using Small Device C Compiler (SDCC) compilator.

Include a application for test and learning purposes.



## 2. Acknowledgments
  
Thanks for Info & help, to:

* Avelino Herrera > http://msx.atlantes.org/index_es.html
* Nerlaska > http://albertodehoyonebot.blogspot.com.es
* Fubu > http://www.gamerachan.org/fubu/
* Marq/Lieves!Tuore > http://www.kameli.net/lt/
* Sapphire/Z80ST > http://z80st.auic.es/
* Pentacour > http://pentacour.com/
* JamQue/TPM > http://www.thepetsmode.com/
* Andrear > http://andrear.altervista.org/home/msxsoftware.php
* Konamiman > https://www.konamiman.com
* MSX Assembly Page > http://map.grauw.nl/resources/msxbios.php
* Portar MSX Tech Doc > http://nocash.emubase.de/portar.htm
* MSX Resource Center > http://www.msx.org/
* Karoshi MSX Community > http://karoshi.auic.es/
* BlueMSX >> http://www.bluemsx.com/
* OpenMSX >> http://openmsx.sourceforge.net/
* Meisei  >> ?



## 3. Requirements

* Small Device C Compiler (SDCC) v3.6 http://sdcc.sourceforge.net/
* Hex2bin v2.2 http://hex2bin.sourceforge.net/ 



## 4. Functions

* void SCREEN0() - Switch to T1 or T2 mode (SCREEN 0), 40 or 80 columns x 24 lines.
* void SCREEN1() - Switch to G1 mode (SCREEN 1), 32 columns x 24 lines.
* void WIDTH(char columns) - Specifies the number of characters per line in text mode.
* void COLOR(char ink, char background, char border) - Specifies the colors of the foreground, background, and border area.
* void CLS() - Clear the contents of the screen.
* void LOCATE(char x, char y) - Moves the cursor to the specified location.
* void PRINT(char* text) - Displays a text string on the screen.
* void PrintNumber(unsigned int value) - Prints an unsigned integer on the screen.
* void PrintFNumber(unsigned int value, char emptyChar, char length) - Prints an unsigned integer on the screen with formatting parameters.
* void num2Dec16(unsigned int aNumber, char *address, char emptyChar) - 16-bit Integer to ASCII (decimal) (by baze)
* void bchput(char value) - Displays one character (MSX BIOS CHPUT).
