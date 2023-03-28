#include <Wire.h>
#include <MPU6050.h>
// #include <SD.h>
#include <SPI.h>

#define FREQUENCY_HZ        100
#define INTERVAL_MS         (1000 / (FREQUENCY_HZ + 1))

MPU6050 mpu;

static unsigned long data_collection_interval_ms = 0;
static unsigned long last_interval_ms = 0;

void setup() 
{
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

}

void loop()
{
  while(last_interval_ms < data_collection_interval_ms){
        Vector normAccel = mpu.readNormalizeAccel();
        if (millis() > last_interval_ms + INTERVAL_MS){
            last_interval_ms = millis(); 
            Serial.print(last_interval_ms); Serial.print("\t");
            Serial.print(normAccel.XAxis); Serial.print("\t");
            Serial.print(normAccel.YAxis); Serial.print("\t");
            Serial.println(normAccel.ZAxis);
      }
  }
}
