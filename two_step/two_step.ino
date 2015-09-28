#include <VirtualWire.h>

#define MESSAGE_CHECK 2646194

byte message[VW_MAX_MESSAGE_LEN]; // a buffer to store the incoming messages
char messageChr[VW_MAX_MESSAGE_LEN + 1];

int left_pins[4] = {2, 3, 4, 5}, right_pins[4] = {6, 7, 8, 9};

void setup(){
  Serial.begin(115200);
  for (int i = 0; i < 4; i+=1){
    pinMode(left_pins[i], OUTPUT);
    pinMode(right_pins[i], OUTPUT);
  }
  vw_setup(2000);
  vw_rx_start();
}


int left_position = 0, right_position = 0;
int left_speed = 0, right_speed = 0;
void loop(){
  readRf();
  step_motor(left_pins, left_position);
  step_motor(right_pins, right_position);
  left_position = changeStepPosition(left_position, left_speed);
  right_position = changeStepPosition(right_position, right_speed);
  
  delay(1);
}

void readRf(){
  byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message
  if (vw_get_message(message, &messageLength)) {
    for (int i = 0; i < messageLength; i++) {
      messageChr[i] = message[i];
    }
    messageChr[messageLength] = 0;
    decodeMessage(messageChr);
  } 
}


void decodeMessage(char* message){
  //Serial.println(message);
  char *next;
  long check = strtol(message, &next, 16);
  if(MESSAGE_CHECK != check) {
    Serial.println("WRONG");
    return;
  }
  
  long left = strtol(next, &next, 16);
  long right = strtol(next, &next, 16);
  left_speed = toSpeed(left);
  right_speed = toSpeed(right); 
}

int toSpeed(long rfIn){
  
  return max(-100, min(100, (int) (rfIn - 512) / 5));
}


int changeStepPosition(int current, int changeBy){
  return (800 + current + changeBy) % 800;
}


void step_motor(int pins[], int step_position){
  switch(step_position / 100){
   case 0:
     digitalWrite(pins[0], LOW); 
     digitalWrite(pins[1], LOW);
     digitalWrite(pins[2], LOW);
     digitalWrite(pins[3], HIGH);
   break; 
   case 1:
     digitalWrite(pins[0], LOW); 
     digitalWrite(pins[1], LOW);
     digitalWrite(pins[2], HIGH);
     digitalWrite(pins[3], HIGH);
   break; 
   case 2:
     digitalWrite(pins[0], LOW); 
     digitalWrite(pins[1], LOW);
     digitalWrite(pins[2], HIGH);
     digitalWrite(pins[3], LOW);
   break; 
   case 3:
     digitalWrite(pins[0], LOW); 
     digitalWrite(pins[1], HIGH);
     digitalWrite(pins[2], HIGH);
     digitalWrite(pins[3], LOW);
   break; 
   case 4:
     digitalWrite(pins[0], LOW); 
     digitalWrite(pins[1], HIGH);
     digitalWrite(pins[2], LOW);
     digitalWrite(pins[3], LOW);
   break; 
   case 5:
     digitalWrite(pins[0], HIGH); 
     digitalWrite(pins[1], HIGH);
     digitalWrite(pins[2], LOW);
     digitalWrite(pins[3], LOW);
   break;
   case 6:
     digitalWrite(pins[0], HIGH); 
     digitalWrite(pins[1], LOW);
     digitalWrite(pins[2], LOW);
     digitalWrite(pins[3], LOW);
   break;
   case 7:
     digitalWrite(pins[0], HIGH); 
     digitalWrite(pins[1], LOW);
     digitalWrite(pins[2], LOW);
     digitalWrite(pins[3], HIGH);
   break;
  }
}
