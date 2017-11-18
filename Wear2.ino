boolean startAudioPlayback = false;
boolean cellNetworkReady = false;

#include "fona.h"
#include "gps.h"
#include "audio.h"
#include "orient.h"
#include "io.h"

uint16_t lastConnectionAttempt = 0;
uint16_t lastSendOrient = 0;

#define SEND_ORIENT_INTERVAL 3000

void setup() {
  //pinMode(5, OUTPUT);
  //digitalWrite(5, LOW);
  
  Serial.begin(115200);
  delay(1000);
  
  setupFONA();
  setupIO();
  setupGPS();
  setupOrient();
}

void loop() {
  uint16_t now = millis();
  if (now - lastConnectionAttempt > 500) {
    lastConnectionAttempt = now;
    runFONA();
  }
  
  if (runIO()) {
    long int lat = 0;
    long int lon = 0;
    if (runGPS(&lat, &lon)) sendData(lat, lon);
    
    if (now - lastSendOrient > SEND_ORIENT_INTERVAL) {
      lastSendOrient = now;    
      sendData(runOrient());    
    }
  }
  

  // At the moment audio playback will stop all other functions of the board
  // for the duration of playback.
  if (startAudioPlayback) {
    playAudio();
    startAudioPlayback = false;
    setupFONA();
  }
}

