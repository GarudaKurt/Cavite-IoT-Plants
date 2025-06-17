#include "waterControl.h"
#include "LCDdisplay.h"
#include <Arduino.h>

const int reservoirPin = D5;
const int nutriantPin = D6;
const int mainpumpPin = D7;

unsigned long prevTime = 0;
unsigned long prevTime1 = 0;

void initSupply() {
  pinMode(reservoirPin, OUTPUT);
  pinMode(nutriantPin, OUTPUT);
  pinMode(mainpumpPin, OUTPUT);

  digitalWrite(reservoirPin, LOW);
  digitalWrite(nutriantPin, LOW);
  digitalWrite(mainpumpPin, LOW);

}

void runSupply(float data, float humid, float phlvl) {
  unsigned long currentTime = millis();
  if(data >= 32.00 || humid >= 75.00 )
  {
    if(currentTime - prevTime > 6500){
      digitalWrite(reservoirPin, HIGH);
      digitalWrite(mainpumpPin, HIGH);
      Serial.println("Despinsig water ");
      prevTime = currentTime;
    }
  } else {
    digitalWrite(reservoirPin, LOW);
    digitalWrite(mainpumpPin, LOW);
  }

  if(phlvl >= 1000) {
    if(currentTime - prevTime1 > 8000) {
      digitalWrite(nutriantPin, HIGH);
      digitalWrite(mainpumpPin, HIGH);
      Serial.println("Despinsing nutriants!");
      prevTime1 = currentTime;
    }
  } else {
    digitalWrite(nutriantPin, LOW);
    digitalWrite(mainpumpPin, LOW);
  }

}

