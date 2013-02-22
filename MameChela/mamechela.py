#!/usr/bin/python
#The MIT License (MIT)
#Copyright (c) 2013 Camilo Castro - camilocastrocabrera@gmail.com
#
#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import os
import sys
import time
import shutil
import serial

def shutdown(arduino,mame_pid):
    print "Shutting Down!"
    
    # So we only kill mame if there is a pid and mame still is open
    if(mame_pid != "mame"):
        os.system("kill %s" %(mame_pid))


    arduino.close()

    print "So Long :D"
    sys.exit(0)

if __name__ == "__main__":
    # Different States We Use
    # Idle
    # SendingBeer
    # WaitingCommand
    state = 'idle'
    
    # Delay of seconds for Main Loop pausing
    delay = 3
    
    # Delay For Arduino
    delay2 = 1.5
    
    # We need to know wich game we are playing
    data = sys.argv
    
    # Arduino Config
    port = '/dev/ttyACM0'
    bauds = 9600
    
    # Holds the commands sent by Arduino
    command = ""
    
    if len(data) > 1:
        game_name = data[1]

        mame_pid = "mame"
        
        if len(data) > 2: 
            mame_pid = data[2]
       
        # Open Arduino Communication
        try:
            print "Initializing Arduino"
            arduino = serial.Serial(port,bauds)
            
            # Some seconds to let Arduino be happy
            time.sleep(delay2)
        except:
            print "Error in Arduino Setup"
            sys.exit(1)
            
        
        # Reading HiScore Files
        path = os.curdir + os.sep + "hi" + os.sep + game_name
        
        # Original HiScore File
        hi_initial = path + ".hi"
        
        # Created on the fly with the current HiScore
        hi_cache = path + "-cache.hi"
        
        # Here we go
        print "Ready"
        
        try:
            while True:
                
                if state == 'idle':
                    # We read the files and compares them
                    # if they are different, we have a hiscore
                    try:
                        file_initial = open(hi_initial,"rb")
        
                        file_cache = open(hi_cache,"rb") 
                    
                        # If they are different, we have a hiscore
                        if file_initial.read() != file_cache.read():
                            print "HiScore!!"
                            state = 'sendingBeer'
                            
                            # So HiScore files are the same
                            shutil.copy2(hi_cache,hi_initial)
                                                      
                        else:
                            print "No Hiscores :C"
                        
                    
                        file_initial.close()
                        file_cache.close()                    
                    
                    except:
                        print "Error Reading Hi Files"
                
                # We comunicate with Arduino and call the Send Beer Command        
                elif state == 'sendingBeer':
                    print "Sending Beer :D"
                    
                    try:
                        arduino.write("Beer")
                    except:
                        print "Error Sending Command"
                        
                    # Read the command and waits until its "beer sent"
                    while(command != "beersent"):
                        command = arduino.readline().strip().lower()
                        time.sleep(delay2)
                        
                    print "BeerStastic,  Have Fun :D !!!"
                                    
                    state = 'waitingCommand'
                    
                # We wait until Reset or Shutdown command is received
                else:
                    print "Waiting Reset or Shutdown Command"
                    
                    command = arduino.readline().strip().lower()
                    
                    if command == "reset":
                        print "Reset!"
                        
                        # Just to make sure HiScore files are the same
                        shutil.copy2(hi_cache,hi_initial)
                        
                        state = "idle"
                        
                    elif command == "shutdown":
                        shutdown(arduino,mame_pid)
                        
                        
                time.sleep(delay)
                
        except KeyboardInterrupt:
            shutdown(arduino,mame_pid)
        
        except:
            print "Something bad happened"
            sys.exit(1)
    else:
        print "I Need a Game Name"
        sys.exit(1)
                    
    
