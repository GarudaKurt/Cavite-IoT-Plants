#include "LCDdisplay.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <Wire.h>
#include "configFirebase.h"
#include "waterControl.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int phSensor = A0;

// Custom glyphs
byte humidityIcon[8] = {
  B00100,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110
};

byte temperatureIcon[8] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01110,
  B11111,
  B11111,
  B01110
};

byte phIcon[8] = {
  B00100,
  B01010,
  B10001,
  B10001,
  B11111,
  B00100,
  B00100,
  B00100
};


void initLCD() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,2);
  lcd.createChar(0, humidityIcon);
  lcd.createChar(1, temperatureIcon); 
   lcd.createChar(2, phIcon);  
  lcd.print("Initialize...");
  delay(2000);
  lcd.clear();

}

void showDisplay(float temp, float humid) {

  float phlvl = analogRead(phSensor);
  Serial.print("PH Sensor");
  Serial.println(phlvl);
  runSupply(temp, humid, phlvl);
  sendData(temp, humid, phlvl);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(getTime());
  lcd.setCursor(0, 1);
  lcd.print(getDate());

  lcd.setCursor(0, 2);
  lcd.write((byte)0); // humidity icon
  lcd.print(": ");
  lcd.print(humid, 1);
  lcd.print("% ");

  lcd.write((byte)1); // temperature icon
  lcd.print(": ");
  lcd.print(temp, 1);
  lcd.print((char)223); // degree symbol
  lcd.print("C");

  lcd.setCursor(0, 3);
  lcd.write((byte)2); // pH icon
  lcd.print(": ");
  lcd.print(phlvl, 2);
}

void errorDisplay(const char * message_1, const char * message_2) {
  lcd.clear();
  lcd.setCursor(2,1);
  lcd.print(message_1);
  lcd.setCursor(2,2);
  lcd.print(message_2);
}
