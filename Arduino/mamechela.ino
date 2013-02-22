/* Tunes Code From https://github.com/tomds/Arduino-Tunes/blob/master/arduino_tunes.pde */

#include <Servo.h>

int ledPin = 13;
int servoPin = 8;
int speakerPin = 3;

Servo beerServo;

String command = "";

int resetButtonPin = 9;
int shutdownButtonPin = 7;
int randomSong = 1;

/* Smaller value -> all tunes play faster; bigger -> slower. Recommend you don't change this, but
instead use playTune() to pass different beatLength values to parseTune() for different songs */
const int beatLength = 50; 

// Generate a tone by passing a square wave of a certain period to the piezo
void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

/* This works out what period, in microseconds, to use for the square wave for a given note. To calculate these,
p = ((1 / freq) * 1,000,000) / 2. We divide by 2 because the signal will be HIGH for p microseconds and then LOW
for p microseconds. Frequencies for the notes obtained from http://www.phy.mtu.edu/~suits/notefreqs.html
The range defined below covers 2 octaves from C4 (middle C, or 261.63Hz) to B5 (987.77Hz). Feel free to modify. */
void playNote(char note, int duration, boolean sharp) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'A', 'B' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 851, 758, 716, 636, 568, 506 };
  
  // these are the "sharp" versions of each note e.g. the first value is for "c#"
  char names_sharp[] = { 'c', 'd', 'f', 'g', 'a', 'C', 'D', 'F', 'G', 'A'};
  int tones_sharp[] = { 1804, 1607, 1351, 1204, 1073, 902, 804, 676, 602, 536 };
  
  // C# 1204
  
  // play the tone corresponding to the note name
  if (sharp == false) {
    for (int i = 0; i < 14; i++) {
      if (names[i] == note) {
        playTone(tones[i], duration);
      }
    }
  } else {
    for (int i = 0; i < 10; i++) {
      if (names_sharp[i] == note) {
        playTone(tones_sharp[i], duration);
      }
    }
  }
}

/* Take a string representing a tune and parse it to play the notes through the piezo.

Parameters:
  char notes[]: a string that represents the notes of the song. The grammar for parsing the string is described at the top of this file.
  int beatLength: changes the tempo. Smaller value -> quicker; bigger -> slower
  boolean loopSong: if true, the song will loop indefinitely (until you press the microswitch)
*/
void parseTune(char notes[], int beatLength, boolean loopSong) {
  boolean play = true;
  
  // 1 iteration of this loop == 1 note played
  for (int i = 0; notes[i] != '.' && play == true; i++) { // stop iteration if '.' is the next char
    //updateSwitchState();
    //if (switchState == LOW) { // For every note, check to see if the button has been pressed to stop the tune
    //  play = false;
    //} else {
      if (notes[i] == ',') { // ',' signifies a rest
      
        // Look at the number (max. 2 digits) following from the ',' to work out the duration of the rest
        char len[3];
        int count = 0;
        while (notes[i+1] >= '0' && notes[i+1] <= '9' && count < 2) {
          len[count] = notes[i+1];
          count++;
          i++;
        }
        len[count] = '\0';
        int duration = atoi(len);
        
        delay(duration * beatLength); // rest duration
      } else { // play the next note, represented by a series of characters e.g. 'c4', 'a#12'
        //alternateLeds(); // alternate the red and green LEDs every note to make them "dance"
        char note = notes[i];
        boolean sharp;
        
        // if the next character is a '#' then we must make the note a sharp
        if (notes[i+1] == '#') {
          i++;
          sharp = true;
        } else {
          sharp = false;
        }
        
        // Look at the number (max. 2 digits) following from the note name to work out the note duration
        char len[3];
        int count = 0;
        while (notes[i+1] >= '0' && notes[i+1] <= '9' && count < 2) {
          len[count] = notes[i+1];
          count++;
          i++;
        }
        len[count] = '\0';
        int duration = atoi(len);
        
        playNote(note, duration * beatLength, sharp);
      }
      
      delay(beatLength / 2); // pause between notes
    }
  }
  
  //if (loopSong == false) {
  //  switchState = LOW;
  //}
//}

// Write your tunes in here using the grammar described at the top of this file. Can have up to 4 tunes.
void playTune (int tune) {
  if (tune == 1) { // Jingle Bells
    char notes[] = "b4b4b8b4b4b8b4D4g6a2b12,4C4C4C6C2C4b4b4b2b2b4a4a4b4a8D8b4b4b8b4b4b8b4D4g6a2b12,4,C4C4C6C2C4b4b4b2b2D4D4C4a4g12,8.";
    parseTune(notes, beatLength, false);
  } else if (tune == 2) { // The Holly and the Ivy
    char notes[] = "g4g2g2g4E4D4b6g2g2g2g4E4D8D2C2b2a2g4b2b2e2e2d4g2a2b2C2b4a4g8,8.";
    parseTune(notes, beatLength * 1.50, false);
  } else if (tune == 3) { // We Wish You a Merry Christmas
    char notes[] = "d4g4g2a2g2f#2e4c4e4a4a2b2a2g2f#4d4f#4b4b2C2b2a2g4e4d2d2e4a4f#4g8,8.";
    parseTune(notes, beatLength * 1.25, false);
  } else if (tune == 4) { // Deck the Halls
    char notes[] = "D6C2b4a4g4a4b4g4a2b2C2a2b6a2g4f#4g6,2D6C2b4a4g4a4b4g4a2b2C2a2b6a2g4f#4g6,2a6b2C4a4b6C2D4a4b2C#2D4E2F#2G4F#4E4D6,2D6C2b4a4g4a4b4g4E2E2E2E2D6C2b4a4g8,8.";
    parseTune(notes, beatLength, false);
  } else if (tune == 5) { // Frogger!
    char notes[] = "F#2D2a2D2F#2D2a2dD2G2G2,1F#2,1E2,2,4G2G2F#2F#2E2E2C#2C#2A2G2F#2E2D4,4.";
    parseTune(notes, beatLength * 2, false);
  }
}

void setup(){
  
 pinMode(speakerPin,OUTPUT);

 pinMode(resetButtonPin, INPUT);
 digitalWrite(resetButtonPin,HIGH);
 
 pinMode(shutdownButtonPin,INPUT);
 digitalWrite(shutdownButtonPin,HIGH);
 
 // Initialize serial
 Serial.begin(9600);
 
 // reserve 200 bytes for the inputString
 command.reserve(200); 
 
 // Led Pin as Output
 pinMode(ledPin,OUTPUT);
 
 randomSeed(analogRead(0));
 
 Serial.println("Ready!");
 Serial.flush();
  
}

boolean handle_button(int buttonPin){
   int button_pressed = !digitalRead(buttonPin);
   return button_pressed; 
}

void loop(){
  
       boolean reset_button_pressed = handle_button(resetButtonPin);
       boolean shutdown_button_pressed = handle_button(shutdownButtonPin);
       
       randomSong = random(1,6);
       //randomSong = 5;
    
       if(reset_button_pressed){ 
         Serial.println("reset");
         delay(200);
         //playTune(randomSong);
       }
       
        if(shutdown_button_pressed){ 
         Serial.println("shutdown");
         delay(200);
         //playTune(randomSong);
       }
       /*Serial.print(button_pressed ? "#" : ".");
       
       static int counter = 0;
       
       if((++counter & 0x3f) == 0) Serial.println();
       
       delay(20);
       */
       
       //ButtonEvent.loop();
       //analogWrite(servoPin,calibrate);
}


/* This event occurs whenever a new data comes in the 
  hardware serial RX
*/
void serialEvent(){
  
  while(Serial.available()){
    
    // get the new byte
    char inChar = (char) Serial.read();
    
    // add to the command String
    command += inChar;
    
    // if we get a Beer Command, set the flag
    if(command == "Beer"){
       
       playTune(randomSong); // Frogger
       
       Serial.println("Command Received!");
       
       digitalWrite(ledPin,HIGH); // turn the LED on
       
       delay(1000); // wait for a second
       digitalWrite(ledPin,LOW); // turn the LED off
       
       Serial.println("Beertastic!");
       delay(1000);
       
       beerServo.attach(servoPin);
       beerServo.write(45);  // Turn Servo Left to 45 degrees
       delay(1000);          // Wait 1 second
       
       beerServo.write(0);   // Turn Servo Left to 0 degrees
       delay(1000);          // Wait 1 second
       
       beerServo.write(90);  // Turn Servo back to center position (90 degrees)
       delay(1000);          // Wait 1 second
       
       beerServo.write(135); // Turn Servo Right to 135 degrees
       delay(1000);          // Wait 1 second
       
       beerServo.write(180); // Turn Servo Right to 180 degrees
       delay(1000);          // Wait 1 second
       
       beerServo.write(90);  // Turn Servo back to center position (90 degrees)
       delay(1000);          // Wait 1 second 
       
       beerServo.detach();
        
       Serial.println("Have Fun! :D ");
       Serial.flush();
       
       command = "";
    }
    
  }
  
}
