#include "Adafruit_FONA.h"

#define FONA_RX 9
#define FONA_TX 8
#define FONA_RST 4

char replybuffer[255];

#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

boolean sendData(const char *url, boolean* play) {
    uint16_t statuscode;
    int16_t length;

    Serial.print("Sending URL:"); Serial.println(url);
   
    if (!fona.HTTP_GET_start(url, &statuscode, (uint16_t *)&length)) {
      Serial.println("Failed!");
      return;
    }

    Serial.print("HTTP status:"); Serial.println(statuscode);
    if (statuscode != 200) {
      fona.HTTP_GET_end();
      return false;
    }

    char buf[64] = {0};
    char *at = buf;
    while (length > 0) {
      while (fona.available()) {
        char c = fona.read();
        *at = c;
        at++;
        length--;
        if (! length) break;
      }
    }
    fona.HTTP_GET_end();

    *play = (strcmp(buf, "PLAY") == 0);
    
    return true;
}

void setupModem() {
  Serial.println(F("Initializing....(May take 3 seconds)"));
  fona.setGPRSNetworkSettings(F("ac.vodafone.es"), F("vodafone"), F("vodafone"));
  fonaSerial->begin(4800);
  if (!fona.begin(*fonaSerial)) {
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }
  Serial.println(F("FONA is OK"));

//  fona.setGPRSNetworkSettings(F("ac.vodafone.es"), F("vodafone"), F("vodafone"));
}

boolean goOnline(bool reconnecting) {
  while(true) {
    uint8_t n = fona.getNetworkStatus();
    Serial.print("Status:"); Serial.println(n);
    if (n == 1 || n == 5) break;
    delay(1000);
  }

  if (reconnecting) fona.enableGPRS(false);
  while (true) {
    delay(2000);
    if (!fona.enableGPRS(true)) {
      Serial.println("Failed to go on GPRS network");
    } else break;
  }
 
   Serial.println("Online GPRS");
}


