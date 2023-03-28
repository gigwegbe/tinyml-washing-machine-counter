#include <SD.h>
#include <Wire.h>
#include <MPU6050.h>
#include <SPI.h>

#define FREQUENCY_HZ 100
#define INTERVAL_MS (1000 / (FREQUENCY_HZ + 1))
#define sensorPin 0

MPU6050 mpu;
Sd2Card card;

static unsigned long data_collection_interval_ms = 20000;
static unsigned long last_interval_ms = 0;
const int chipSelect = 2;

void setup()
{
  pinMode(sensorPin, INPUT);
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  pinMode(sensorPin, INPUT);
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  // SD.begin();
  SD.begin(chipSelect);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1)
      ;
  }
  Serial.println("card initialized.");
}

void loop()
{
  Vector normAccel = mpu.readNormalizeAccel();
  // Log current data
  String dataString = "";

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("1_log0.txt", FILE_WRITE);

  if (last_interval_ms < data_collection_interval_ms)
  {
    if (millis() > last_interval_ms + INTERVAL_MS)
    {

      // Log current data
      dataString += last_interval_ms;
      dataString += ",";
      dataString += String(normAccel.XAxis);
      dataString += ",";
      dataString += String(normAccel.YAxis);
      dataString += ",";
      dataString += String(normAccel.ZAxis);

      // if the file is available, write to it:
      if (dataFile)
      {
        last_interval_ms = millis();
        dataFile.println(dataString);
        dataFile.close();
        // print to the serial port too:
      }
      // if the file isn't open, pop up an error:
      else
      {
        Serial.println("error opening datalog.txt");
      }
      Serial.println(dataString);
    }
  }
}
