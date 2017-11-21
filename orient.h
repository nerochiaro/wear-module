#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055();

void setupOrient() {
  bno.begin();
  bno.setExtCrystalUse(true);
}

imu::Vector<3> runOrient() {
    return bno.getVector(Adafruit_BNO055::VECTOR_EULER);
}

void formatOrient(char *p, imu::Vector<3> data) {
  ltoa((long int)(data.x() * 10000), p, 10);
  p += strlen(p);
  p[0] = ',';
  p++;
  ltoa((long int)(data.y() * 10000), p, 10);
  p += strlen(p);
  p[0] = ',';
  p++;
  ltoa((long int)(data.z() * 10000), p, 10);
  p += strlen(p);
  p[0] = 0;
}


