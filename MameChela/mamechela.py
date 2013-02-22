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

if __name__ == "__main__":
    # Different States We Use
    # Idle
    # SendingBeer
    # WaitingReset
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
    
    
    if len(data) > 2:
        game_name = data[1]
        mame_pid = data[2]
       
        # Open Arduino Communication
        try:
            print "Initializing Arduino"
            arduino = serial.Serial(port,bauds)
        except:
            print "Error in Arduino Setup"
            sys.exit(1)
            
        # Some seconds to let Arduino be happy
        time.sleep(delay2)
        
        
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
                            print "No Changes"
                        
                    
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
                        
                    state = 'waitingReset'
                    
                # We wait until Reset command is received
                else:
                    print "Waiting Reset or Shutdown Command"
                    
                    #try:
                command = arduino.readline().strip().lower()
                if command == "reset":
                    print "Reset!"
                    state = "idle"
                elif command == "shutdown":
                    print "Shutting Down!"
                    os.system("kill %s" %(mame_pid))
                    arduino.close()
                    sys.exit(0)
                        
                    #except:
                    #    print "Error Waiting Commands"
                        
                time.sleep(delay)
                
        except KeyboardInterrupt:
            try:
                arduino.close()
            except:
                print "Error Closing Arduino"
                sys.exit(1)
            
            print "So Long :D"
            sys.exit(0)
    else:
        print "I Need a Game Name"
        sys.exit(1)
                    
    
