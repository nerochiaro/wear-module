#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno = Adafruit_BNO055();

void setupOrient() {
  bno.begin();
  bno.setExtCrystalUse(true);
}

imu::Vector<3> runOrient() {
    return bno.getVector(Adafruit_BNO055::VECTOR_EULER);
}

