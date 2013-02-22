# MAMECHELA
####You got a Hiscore You won a Beer

MameChela is a simple hack to mame based in the MKCHAMP hack for Hiscores.

Consist in a Python Script that Talks to an Arduino. This script checks if the files in hiscore directory for the current game are equal, if they are not you made a High Score!

0. Download Mame Sources
http://mamedev.org/

1. install mame dependencies (ubuntu)
sudo aptitude install build-essential libgtk2.0-dev libgnome2-dev libsdl1.2-dev

2. apply patches
patch -p0 -E -binary < 0148u1.diff
patch -p0 -E -binary < hi_148.diff

3. overwrite mame/src/emu/ hiscore.c & hiscore.h with mamechela's hiscore.c & hiscore.h

4. make (takes some time)
5. config mame
6. put some roms

7. connect arduino
8. verify that mamechela.py is configured correctly
9. check mamechela.sh commands

10. For turn off use Ctrl+C or Arduino's Shutdown command

usage
./mamechela.sh <game> 

