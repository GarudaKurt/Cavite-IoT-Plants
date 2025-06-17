#include "configFirebase.h"
#include "LCDdisplay.h"

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>
// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define WIFI_SSID "PLDTHOMEFIBRKgFD2" //change this based on your mobile hotspot name and make sure to set 2.4Ghz your mobile hotspot
#define WIFI_PASSWORD "Kielalleckmartires#24" //change this  based on youe hotspot password
#define API_KEY "AIzaSyD6DXz1FdLHYWceZG6K0eHOSKOqFTO3KT8"
#define DATABASE_URL "https://aeroponics-951d1-default-rtdb.firebaseio.com"
#define USER_EMAIL "aeroponics@gmail.com"
#define USER_PASSWORD "aeroponicsangpassword"
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

int timeZone = 8 * 3600;  // Philippine Time
int dst = 0;

void initFirebase() {
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
   errorDisplay("Fail connect to WiFi", "Error please check" );
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Time is needed for Firebase token generation
  configTime(timeZone, dst, "pool.ntp.org", "time.nist.gov");
  Serial.println("Syncing time...");
  while (!time(nullptr)) {
    Serial.println("Waiting for time sync...");
    delay(1000);
  }

  // Set Firebase config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;

  // Firebase init
  Firebase.begin(&config, &auth);
  Firebase.reconnectNetwork(true);

  // Set larger SSL buffer
  fbdo.setBSSLBufferSize(8192, 2048);

  // Retry token generation
  Serial.println("Waiting for Firebase token...");
  int retries = 0;
  while (auth.token.uid == "" && retries < 10) {
    Serial.println("Still waiting...");
    delay(1000);
    retries++;
  }

  if (auth.token.uid == "") {
    Serial.println("❌ Failed to get Firebase token. Check credentials or network.");
  } else {
    Serial.println("✅ Firebase token acquired!");
  }
}

const char* getTime() {
  static char buffer[9];
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", 
           p_tm->tm_hour, 
           p_tm->tm_min, 
           p_tm->tm_sec);
  return buffer;
}

const char* getDate() {
  static char buffer[11];
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  snprintf(buffer, sizeof(buffer), "%04d/%02d/%02d", 
           p_tm->tm_year + 1900,
           p_tm->tm_mon +1, 
           p_tm->tm_mday
          );
  return buffer;
}

void sendData(float temp, float humid, float phlvl) {
  Serial.println("Hellow World");
  if (Firebase.ready()) {
    bool isSuccessTemp = Firebase.setFloat(fbdo, "/Monitoring/Temperature", temp);
    bool isSuccessHumid = Firebase.setFloat(fbdo, "/Monitoring/Humidity", humid);
    bool isSuccessPhlvl = Firebase.setFloat(fbdo, "/Monitoring/pHLevel", phlvl);
    Serial.print("Send data temp: ");
    Serial.println(temp);
    Serial.print("Send data humid: ");
    Serial.println(humid);
    Serial.print("Send data phlvl: ");
    Serial.println(phlvl);
    if(!isSuccessTemp || !isSuccessHumid || !isSuccessPhlvl)
      Serial.println("Failed to send to firebase");
  } else
    Serial.println ("Pass Here");
}
