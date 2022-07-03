#include <Seeed_Arduino_FreeRTOS.h>
#include"LIS3DHTR.h"
LIS3DHTR<TwoWire> lis;

#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
//MPU6050 accelgyro;
MPU6050 accelgyro(0x68); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define OUTPUT_READABLE_ACCELGYRO
 
void setup() {
  Serial.begin(115200);
  lis.begin(Wire1);
 
  if (!lis) {
    Serial.println("ERROR");
    while(1);
  }
  lis.setOutputDataRate(LIS3DHTR_DATARATE_25HZ); //Data output rate
  lis.setFullScaleRange(LIS3DHTR_RANGE_2G); //Scale range set to 2g

    // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();


    // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");


  xTaskCreate(acceTaskWio,"Accelerometer Task Wio", 128, NULL, 1, NULL); 
  xTaskCreate(acceTaskMpu,"Accelerometer Task MPU", 128, NULL, 1, NULL); 

// Start the RTOS, this function wiTerminal never return and wiTerminal schedule the tasks.
  vTaskStartScheduler();

}

void acceTaskMpu(void * pvParameters)
  { 
    while(1){

    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    #ifdef OUTPUT_READABLE_ACCELGYRO
      // display tab-separated accel/gyro x/y/z values
      Serial.print("a/g:\t");
      Serial.print(ax); Serial.print("\t");
      Serial.print(ay); Serial.print("\t");
      Serial.print(az); Serial.print("\t");
      Serial.print(gx); Serial.print("\t");
      Serial.print(gy); Serial.print("\t");
      Serial.println(gz);
  #endif

   #ifdef OUTPUT_BINARY_ACCELGYRO
      Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
      Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
      Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
      Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
      Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
      Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
  #endif
  
    }
  }

  
void acceTaskWio(void * pvParameters)
{
  
  while(1){
  float x_values, y_values, z_values;
  
  x_values = lis.getAccelerationX();
  y_values = lis.getAccelerationY();
  z_values = lis.getAccelerationZ();
 
  Serial.print("Wio  X: "); Serial.print(x_values);
  Serial.print(" Wio  Y: "); Serial.print(y_values);
  Serial.print("  Wio  Z: "); Serial.print(z_values);
  Serial.println();
  delay(50);
       
      }
    }


void loop() {
}
