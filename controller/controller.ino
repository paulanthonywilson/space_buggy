#include <VirtualWire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(13, OUTPUT);
  vw_setup(2000);
  
}


void loop() {
  flash();
  transmit(analogRead(2), analogRead(3));
  delay(10);
}

long lastFlash = 0;
void flash() {
  if (millis() - lastFlash > 5000) {
    lastFlash = millis();
    digitalWrite(13, HIGH);
  }
  if (millis() - lastFlash > 50) {
    digitalWrite(13, LOW);
  }
}


void transmit(int left, int right){
  char message[50];
  
  sprintf(message, "2860B2 %X %X", left, right);
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone

  Serial.print(strlen(message));
  Serial.print("   ");
  Serial.println(message);
}
