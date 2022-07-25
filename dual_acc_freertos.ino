#include <Seeed_Arduino_FreeRTOS.h>

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


MPU6050 accelgyro;
MPU6050 accelgyro2(0x69);

typedef int TaskProfiler; 
TaskProfiler Accele1Profiler; 
TaskProfiler Accele2Profiler; 

TaskHandle_t Handle_aTask;
TaskHandle_t Handle_bTask;

void setup() {
 // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);
    vNopDelayMS(1000); // prevents usb driver crash on startup, do not omit this
    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    accelgyro2.initialize();
    // verify connection
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    Serial.println(accelgyro2.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
      
    xTaskCreate(acceTask,"accelerometer 1", 256, NULL, 1, &Handle_aTask); 
    xTaskCreate(acceTask2,"accelerometer 2", 256, NULL, 2, &Handle_bTask);

// Start the RTOS, this function wiTerminal never return and wiTerminal schedule the tasks.
  vTaskStartScheduler();

}


void acceTask(void * pvParameters)
{   
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
    
  while(1){
    Serial.print("This is Accelerometer 1: ");  
    Accele1Profiler++;
    Serial.println(Accele1Profiler);
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.print("a/g:\t");
    Serial.print(ax); Serial.print("\t");
    Serial.print(ay); Serial.print("\t");
    Serial.print(az); Serial.print("\t");
    Serial.print(gx); Serial.print("\t");
    Serial.print(gy); Serial.print("\t");
    Serial.println(gz);
    vTaskDelay(100);
      
  }
}


void acceTask2(void * pvParameters)
{   
  int16_t ax2, ay2, az2;
  int16_t gx2, gy2, gz2;
    
  while(1){
    Serial.print("This is Accelerometer 2: ");  
    Accele2Profiler++;
    Serial.println(Accele2Profiler);
    accelgyro2.getMotion6(&ax2, &ay2, &az2, &gx2, &gy2, &gz2);
    Serial.print("a2/g2:\t");
    Serial.print(ax2); Serial.print("\t");
    Serial.print(ay2); Serial.print("\t");
    Serial.print(az2); Serial.print("\t");
    Serial.print(gx2); Serial.print("\t");
    Serial.print(gy2); Serial.print("\t");
    Serial.println(gz2);
    vTaskDelay(100);
      
  }
}
 
void loop() {
}
