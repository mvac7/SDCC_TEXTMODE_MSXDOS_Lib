# TEXTMODE MSX-DOS SDCC Library (fR3eL Project)

```
Author: mvac7 [mvac7303b@gmail.com]
Architecture: MSX
Format: C Object (SDCC .rel)
Programming language: C and Z80 assembler
```



## Description

This project is an Open Source library of functions for creating aplications in text mode.

Works in T1 (screen 0, 40 columns), T2 (screen 0, 80 columns), and G1 (screen 1, 32 columns) modes.

This library uses the functions of the BIOS designed to print text on the screen, 
but they are called through the interslot (CALSLT), to be used in MSXDOS applications.

Use them for developing MSX applications using Small Device C Compiler (SDCC).

Two applications are included in the source code for testing and learning purposes.

I have adapted a routine for converting a 16 Bits value to ASCII, extracted from the Baze collection [(WEB)](http://baze.sk/3sc/misc/z80bits.html#5.1), for printing numbers. 

This library is part of the [MSX fR3eL Project](https://github.com/mvac7/SDCC_MSX_fR3eL).

Enjoy it!



## History of versions:

* v1.4 (04/09/2019)< Integer printing functions improved (PrintNumber & PrintFNumber). num2Dec16 becomes PrintFNumber
* v1.3 (29/08/2019)  nakeds and PrintNumber improvements
* v1.2 (05/05/2018)
* v1.1 (27/02/2017)



## Requirements

* Small Device C Compiler (SDCC) v3.9 http://sdcc.sourceforge.net/
* Hex2bin v2.5 http://hex2bin.sourceforge.net/



## Functions

* void **SCREEN0**() - Switch to T1 or T2 mode (SCREEN 0), 40 or 80 columns x 24 lines.
* void **SCREEN1**() - Switch to G1 mode (SCREEN 1), 32 columns x 24 lines.
* void **WIDTH**(char columns) - Specifies the number of characters per line in text mode.
* void **COLOR**(char ink, char background, char border) - Specifies the colors of the foreground, background, and border area.
* void **CLS**() - Clear the contents of the screen.
* void **LOCATE**(char x, char y) - Moves the cursor to the specified location.
* void **PRINT**(char* text) - Displays a text string on the screen.
* void **PrintNumber**(unsigned int value) - Prints an unsigned integer on the screen.
* void **PrintFNumber**(unsigned int value, char emptyChar, char length) - Prints an unsigned integer on the screen with formatting parameters.
* void **bchput**(char value) - Displays one character (MSX BIOS CHPUT).



## Acknowledgments
  
I want to give a special thanks to all those who freely share their knowledge with the MSX developer community.

* Baze > [WEB](http://baze.sk/3sc/misc/z80bits.html)
* Avelino Herrera > [WEB](http://msx.atlantes.org/index_es.html)
* Nerlaska > [Blog](http://albertodehoyonebot.blogspot.com.es)
* Marq/Lieves!Tuore > [Marq](http://www.kameli.net/marq/) [Lieves!Tuore](http://www.kameli.net/lt/)
* [Fubukimaru](https://github.com/Fubukimaru) > [Blog](http://www.gamerachan.org/fubu/)
* Andrear > [Blog](http://andrear.altervista.org/home/msxsoftware.php)
* Ramones > [MSXblog](https://www.msxblog.es/tutoriales-de-programacion-en-ensamblador-ramones/) - [MSXbanzai](http://msxbanzai.tni.nl/dev/faq.html)
* Sapphire/Z80ST > [WEB](http://z80st.auic.es/)
* Fernando García > [youTube](https://www.youtube.com/user/bitvision)
* Eric Boez > [gitHub](https://github.com/ericb59)
* MSX Assembly Page > [WEB](http://map.grauw.nl/resources/msxbios.php)
* Portar MSX Tech Doc > [WEB](http://nocash.emubase.de/portar.htm)
* MSX Resource Center > [WEB](http://www.msx.org/)
* Karoshi MSX Community (RIP 2007-2020)
* BlueMSX emulator >> [WEB](http://www.bluemsx.com/)
* OpenMSX emulator >> [WEB](http://openmsx.sourceforge.net/)
* Meisei emulator >> ?