#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "nerochiaro"
#define AIO_KEY         "13146cce703f4c1a83b7d2467bcbc9fd"
#define RECONNECT_EVERY_MS 2000L

#include <TinyGsmClient.h>
#include <PubSubClient.h>

TinyGsmClient GSMclient(modem);
PubSubClient mqtt(GSMclient);

const char* topicLocation = "nerochiaro/f/lo";
const char* topicOrient = "nerochiaro/f/or";
const char* topicControl = "nerochiaro/f/sn";

boolean* cellReadyFlag = 0;

boolean mqttConnect() {
  Serial.print(F("MQTT:I:"));
  Serial.print(AIO_SERVER);
  if (!mqtt.connect("", AIO_USERNAME, AIO_KEY)) {
    Serial.println(F("E:"));
    return false;
  }
  Serial.println(F("OK"));
  mqtt.subscribe(topicControl);
  return mqtt.connected();
}

void mqttCallback(char* topic, byte* payload, unsigned int len) {
  Serial.print(F("CTL:"));
  Serial.print(topic);
  Serial.print(':');
  Serial.write(payload, len);
  Serial.println();

  if (strcmp(topic, topicControl) == 0) {
    if (len > 0 && payload[0] == '1') {
      startAudioPlayback = true;
    }
  }
}

void setupIO() {
  mqtt.setServer(AIO_SERVER, AIO_SERVERPORT);
  mqtt.setCallback(mqttCallback);
}

bool reconnecting = false;
uint16_t lastReconnectAttempt = 0;

boolean runIO() {
    if (mqtt.connected()) {
      mqtt.loop();
      return true;
    }

    // if the cell modem is not ready, don't bother trying to connect yet
    if (!cellNetworkReady) return false;
    
    // wait a bit before retrying a reconnect, if we have been disconnected
    unsigned long t = millis();
    if (!reconnecting || t - lastReconnectAttempt > RECONNECT_EVERY_MS) {
      lastReconnectAttempt = t;
      if (mqttConnect()) {
        lastReconnectAttempt = 0;
        reconnecting = false;
        return true;
      } else {
        reconnecting = true;
      }
    }
    
    return false;

return true;
}

void sendData(long int lat, long int lon) {
    char sendbuffer[30] = {0};
    formatLocation(sendbuffer, lat, lon);   
    mqtt.publish(topicLocation, sendbuffer);
}

void sendData(imu::Vector<3> orient) {
    char sendbuffer[25] = {0};
    formatOrient(sendbuffer, orient);
    mqtt.publish(topicOrient, sendbuffer);
}


