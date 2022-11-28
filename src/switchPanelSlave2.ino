#include <SoftwareSerial.h>
/*
  SLAVE 2 - ARDUINO - 14 Push Button Serial interface with debounce. Sends button commands 1 - 42 as bytes via virtual serial port(pins 18 and 19)
  Implements a 2 button Fn system with LED indicators
*/
const int buttonPins[] = {2,3,4,5,6,7,8,9,10,11,14,15,16,17};
int prevStates[14] = {1};
int states[14] = {1};
bool fn1 = false; 
bool fn2 = false; 
const int debounceDelay = 20; // debounce delay time in ms
const int fn1LedPin = 13;
const int fn2LedPin = 12;
SoftwareSerial link(19, 18);

void setup(){
  //rx and tx data lines
  link.begin(9600);
  pinMode(18, OUTPUT);
  pinMode(19, INPUT);
  //Push Buttons
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  //LED's
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}
/*
  Debounce buttons, check for fn's and send send serial data accordingly.
*/
void loop(){
  for(int i = 0; i < 14; i++){
    int state = digitalRead(buttonPins[i]);
    states[i] = state;
  }
  for(int i = 0; i < 14; i++){
    if(states[i] != prevStates[i]){
      int fn = 0;
      if(states[i] == LOW){ // on button press
        if(i == 11){ //fn1 button Pin
          fn = fnToggle(1);
        } else if(i == 10){ //fn2 button pin
          fn = fnToggle(2);
        }
      }
      int buttonID = i+1; 
      if(fn1 == true){
        buttonID += 14;
      } else if(fn2 == true){
        buttonID += 28;
      } 
      if(states[i] == LOW){ // only write once.
        link.write(buttonID);//Send integers 1 - 42 as bytes
      }
      prevStates[i] = states[i];
      delay(debounceDelay);
    }
  }
}
/*
  Toggle fn1 and fn2 variables and LEDs. They cannot both be true. return fn value 1, 2 or 0
*/
int fnToggle(int fn){ 
  if(fn == 1){
    fn1 = !fn1;
    if(fn1 == true){
      digitalWrite(fn1LedPin, HIGH);
      digitalWrite(fn2LedPin, LOW);
      fn2 = false;
      return 1;
    } else {
      digitalWrite(fn1LedPin, LOW);
      return 0;
    }
  } else {
    fn2 = !fn2;
    if(fn2 == true){
      digitalWrite(fn2LedPin, HIGH);
      digitalWrite(fn1LedPin, LOW);
      fn1 = false;
      return 2;
    } else {
      digitalWrite(fn2LedPin, LOW);
      return 0;
    }
  }
}
