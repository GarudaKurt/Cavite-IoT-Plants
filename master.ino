#include "LCDdisplay.h"
#include "waterControl.h"
#include "configFirebase.h"

#include "Adafruit_SHT31.h"
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void setup() {
  Serial.begin(115200);
  delay(1000);
  if (! sht31.begin(0x44))        // Set to 0x45 for alternate i2c addr
  { 
    Serial.println("Encounter hardware problem in sht31! Please check");
    while (1) delay(1);
  }

  initLCD();
  initSupply();
  initFirebase();
}

void loop() {
  float temperature = sht31.readTemperature();
  float humidity = sht31.readHumidity();
  Serial.println("Temp: " + String(temperature, 2) + "°C");
  Serial.println("Humidity: " + String(humidity, 1) + "%");
  showDisplay(temperature, humidity);

  delay(1000);
}
