# MameChela
####You got a Hiscore You won a Beer

MameChela is a simple hack to mame based in the MKCHAMP hack for Hiscores.

Consist in a Python Script that Talks to an Arduino. This script checks if the files in hiscore directory for the current game are equal, if they are not you made a High Score!
***

0. Download Mame Sources
<a href= "http://mamedev.org/">MameDev</a>

1. Install mame dependencies (ubuntu)

		$ sudo aptitude install build-essential libgtk2.0-dev libgnome2-dev libsdl1.2-dev

2. Apply patches

		$ patch -p0 -E -binary < 0148u1.diff

		$ patch -p0 -E -binary < hi_148.diff

3. Overwrite mame/src/emu/ hiscore.c & hiscore.h with mamechela's hiscore.c & hiscore.h

		$ cp mamechela/hiscore* ../src/emu/


4. Make (takes some time)

		$ make

5. Config mame
6. Put some roms

7. Connect Arduino (more to come)

8. Check that mamechela.py is configured correctly (Arduino Config)

9. Check mamechela.sh commands

10. For turn off use Ctrl+C or Arduino's Shutdown command

####Usage

		./mamechela.sh <game> 

