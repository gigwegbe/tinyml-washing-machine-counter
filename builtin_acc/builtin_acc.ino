#include"LIS3DHTR.h"
LIS3DHTR<TwoWire> lis;

const float accelerationThreshold = 4; // threshold of significant in G's
const int numSamples = 119;

int samplesRead = numSamples;

void setup() {
  Serial.begin(9600);
  lis.begin(Wire1);

  if (!lis) {
    Serial.println("ERROR");
    while(1);
  }
  
  lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); //Data output rate
  lis.setFullScaleRange(LIS3DHTR_RANGE_2G); //Scale range set to 2g

  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
}

void loop() {
  float x, y, z;
  
  while (samplesRead == numSamples) {
    x=lis.getAccelerationX();
    y=lis.getAccelerationY();
    z=lis.getAccelerationZ();


     float aSum = fabs(x) + fabs(y) + fabs(z);

      // check if it's above the threshold
      if (aSum >= accelerationThreshold) {
        // reset the sample read count
        samplesRead = 0;
        break;
      }
  }

  while (samplesRead < numSamples) {
    x=lis.getAccelerationX();
    y=lis.getAccelerationY();
    z=lis.getAccelerationZ();

    samplesRead++;

    Serial.print(x, 3);
    Serial.print(',');
    Serial.print(y, 3);
    Serial.print(',');
    Serial.print(z, 3);
    Serial.println();

    if (samplesRead == numSamples) {
      Serial.println();
    }
  }
}
