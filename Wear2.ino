boolean startAudioPlayback = false;

#include "fona.h"
#include "gps.h"
#include "audio.h"
#include "orient.h"

uint16_t lastSend = 0;
#define SEND_INTERVAL 3000
#define SPEED 19200
#define BASE_API_URL "http://gm-console.herokuapp.com/set/"

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  
  Serial.begin(SPEED);
  delay(1000);
  Serial.println("Starting");

  setupModem();
  goOnline(false);

  setupOrient();
  setupGPS();
}

long int lat = 0;
long int lon = 0;
void loop() {
  if (runGPS(&lat, &lon)) {
    Serial.println(F("\n--FIX--"));
  }

  uint16_t now = millis();
  if (now - lastSend >= SEND_INTERVAL) {
    lastSend = now;
    
    imu::Vector<3> o = runOrient();

    char url[256] = BASE_API_URL;
    char *at = url + strlen(url);
    formatOrient(at, o);
    at = url + strlen(url);
    *at = ',';
    at++;
    formatLocation(at, lat, lon);

    boolean play = false;
    if (!sendData(url, &play)) {
      goOnline(true);
    }

    if (play) {
      playAudio();
    }
  }
}

