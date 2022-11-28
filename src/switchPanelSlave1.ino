#include <SoftwareSerial.h>
/*
  SLAVE 1 - ARDUINO - 14 pin Serial interface with debounce.
  Recieves data from Slave 2 via link2. Sends all data to master via link.
  Onboard button commands 45 - 73 as bytes
*/
SoftwareSerial link(18, 19);
SoftwareSerial link2(12, 13);
const int switchPins[] = {2,3,4,5,6,7,8,9,10,11,14,15,16,17};
int prevStates[14] = {1};
int states[14] = {1};
const int debounceDelay = 20;

void setup(){
  //data links and pins
  link.begin(9600);
  link2.begin(9600);
  pinMode(18, INPUT);
  pinMode(19, OUTPUT);
  pinMode(12, INPUT);
  pinMode(13, OUTPUT);
  //switches
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
}
/*
  Debounce buttons and send send serial data accordingly.
*/
void loop(){ 
  while(link2.available() > 0){ // pass along data from slave2
    link.write(link2.read());
  }
  for(int i = 0; i < 14; i++){
      int state = digitalRead(switchPins[i]);
      states[i] = state;
  }
  for(int i = 0; i < 14; i++){
    if(states[i] != prevStates[i]){
      int isOn = false;
      int switchID = i + 45; //slave 2's IDs range from 1-42
      if(states[i] == HIGH){ // on button press
        switchID += 14;//on turn off
      }
      link.write(switchID);
      prevStates[i] = states[i];
      delay(debounceDelay);
    }
  }
}