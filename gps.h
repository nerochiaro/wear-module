#include <TinyGPS.h>

TinyGPS gps;
HardwareSerial& ss = Serial1;

// Control how often the sentences are output from the GPS module
#define NMEA_SEND_10_S  "$PMTK220,10000*2F"
#define NMEA_SEND_5_S  "$PMTK220,5000*1B"
#define NMEA_SEND_1_S  "$PMTK220,1000*1F"
#define NMEA_SEND_500_MS  "$PMTK220,500*2B"
#define NMEA_SEND_200_MS  "$PMTK220,200*2C"
#define NMEA_SEND_100_MS "$PMTK220,100*2F"

// Control how often a fix is attempted inside the GPS module
#define FIX_EVERY_10_S  "$PMTK300,10000,0,0,0,0*2C"
#define FIX_EVERY_5_S  "$PMTK300,5000,0,0,0,0*18"
#define FIX_EVERY_1_S  "$PMTK300,1000,0,0,0,0*1C"
#define FIX_EVERY_200_MS  "$PMTK300,200,0,0,0,0*2F"

#define NMEA_OUTPUT_RMC "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
#define NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define NMEA_OUTPUT_ALL "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define NMEA_OUTPUT_NONE "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

void setupGPS()
{
  ss.begin(9600);
  delay(200);

  // configure the module
  ss.println(NMEA_OUTPUT_RMC);
  ss.println(FIX_EVERY_200_MS);
  ss.println(NMEA_SEND_1_S);
}

bool runGPS(long int* lat, long int* lon)
{
  while (ss.available())
  {
    char c = ss.read();
    Serial.write(c); // uncomment this line if you want to see the GPS data flowing
    if (gps.encode(c)) { // Did a new valid sentence come in?
      gps.get_position(lat, lon);
      return (*lat != TinyGPS::GPS_INVALID_F_ANGLE && *lon != TinyGPS::GPS_INVALID_F_ANGLE);
    }
    return false;
  }
}

void formatLocation(char *p, long int lat, long int lon) {
  ltoa(lat, p, 10);
  p += strlen(p);
  p[0] = ','; p++;

  ltoa(lon, p, 10);
  p += strlen(p);

  p[0] = 0;
}

