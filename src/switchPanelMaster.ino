#include <Keyboard.h>
#include <SoftwareSerial.h>
/*
 MASTER - ARDUINO MICRO - Uses Serial1 by default on the rX and tX pins
 Controller recieves button press data from slave1 controller as well as debouncing and handling 14 two-way switches. This code implements an 
 key command system that references ASCII characters by their integer
 values, then uses the Arduino Keyboard library to send the command.
*/
const int pins[] = {4,5,6,7,8,9,10,16,14,15,18,19,20,21};
int states[14] = {1};
int prevStates[14] = {1};
const int debounceDelay = 20;
const int asciiIntStartPoint = 97;
const int fnStartPoint = 91;

void setup(){
  Keyboard.begin();
  Serial1.begin(9600); 
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, INPUT_PULLUP); 
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
}

void loop(){
  /*
   Read incoming data from slave 1
   contains data from both slaves
  */
  while(Serial1.available() > 0){
    int data = Serial1.read(); 
    keyboardCommand(data);
  }
  /*
   Debounce and handle onboard switches
  */
  for(int i = 0; i < 14; i++){
    int state = digitalRead(pins[i]);
    states[i] = state;
  }
  for(int i = 0; i < 14; i++){
    if(states[i] != prevStates[i]){
      if(states[i] == LOW){ // on button press
       	keyboardCommand(i + 74);
      } else {
        keyboardCommand(i + 90);
      }
      prevStates[i] = states[i];
     delay(debounceDelay);
    }
  }
}

void keyboardCommand(int data){ 
  /*
    System for assigning double key press for any key code over 26. There are about 90 possible instructions from this device
    1-26 => a-z => [97-122]
    27-52 => [a-[z => 91 + [97-122]
    53-78 => \a-\z => 92 + [97-122]
    79-104 =>]a-]z => 93 + [97-122]
  */
  int position = ((data + asciiIntStartPoint) % 26) + 1;
  int keyCode = position + asciiIntStartPoint - 1;
  if(data <= 26){
    Keyboard.press(data + asciiIntStartPoint);
  } else if(data > 26 && data <= 52){
    Keyboard.press(fnStartPoint);
    Keyboard.press(keyCode);
  } else if(data > 52 && data < 78){
    Keyboard.press(fnStartPoint + 1);
    Keyboard.press(keyCode);
  } else {
    Keyboard.press(fnStartPoint + 2);
    Keyboard.press(keyCode);
  }
  delay(200);
  Keyboard.releaseAll();
}
