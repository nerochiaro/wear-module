//#define FONA_RX  9
//#define FONA_TX  8
//#define FONA_RI_INTERRUPT INT4
//#define AT_DISABLE_RINGER "AT+CRSL=0"
//#define AT_PICKUP_AFTER_1_RING "ATS0=1"

const char apn[]  = "gprs-service.com";
const char user[] = "";
const char pass[] = "";

#include <SoftwareSerial.h>
SoftwareSerial SerialAT(8, 9); // RX, TX

#define TINY_GSM_MODEM_SIM800
#include <TinyGsmClient.h>

#define AT_DEBUG
#ifdef AT_DEBUG
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, Serial); 
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

TinyGsmClient client(modem);

void setupFONA() {
  SerialAT.begin(9600);

  Serial.print(F("Modem:I:"));
  modem.restart();
  Serial.println(modem.getModemInfo());
}

boolean runFONA() {
  uint8_t s = modem.getRegistrationStatus();
  if (s != REG_OK_HOME && s != REG_OK_ROAMING) {
    cellNetworkReady = false;
    return false;
  }
 
  if (!cellNetworkReady) {
    Serial.println(F("GSM:ON"));
    Serial.print(F("\n2G:I:"));
    Serial.print(apn);
    Serial.print(':');
    if (!modem.gprsConnect(apn, user, pass)) {
      Serial.println("E:");
      return false;
    }
    cellNetworkReady = true;
    Serial.println("OK");
  }
  return true;
}

