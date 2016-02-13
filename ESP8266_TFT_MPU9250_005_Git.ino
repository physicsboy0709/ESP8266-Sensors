// ESP8266>MPU6050>TFT ILI9340
// MPU6050 = 9D0F MPU 9250 SPI/IIC 9-Axis Attitude + Gyro + Accelerator + Magnetometer Module
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9340.h>
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _mosi, _sclk, _rst, _miso);
// Use hardware SPI
#define _sclk 14           // pins for setting up the ESP8266 for SPI
#define _miso 12
#define _mosi 13
#define _cs 15
#define _dc 2
#define _rst 16
Adafruit_ILI9340 tft = Adafruit_ILI9340(_cs, _dc, _rst);

void setup() {
 
 Serial.begin(115200);
 while (!Serial);
 Wire.begin();
 Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     //  wakeup the MPU-6050
  Wire.endTransmission(true);

  tft.begin();                     // Start the Display up
  tft.fillScreen(ILI9340_BLACK);   // clear the decks and darken the Universe  
  tft.setRotation(3);              // Rotate screen to best orientation
                                  // 0=normal 1=clockwise90° 2= upsidedown 3=CounterClockwise90°
  tft.setTextColor(ILI9340_WHITE) ;tft.setTextSize(2); tft.setCursor(0, 0);  tft.print("ESP8266>MPU6050>ILI9340");
  tft.setTextColor(ILI9340_GREEN);tft.setTextSize(5); tft.setCursor(140, 50);  tft.print("Gyro"); 
  tft.setTextSize(3); tft.setCursor(110, 30);  tft.print("X"); tft.setCursor(110, 56); tft.print("Y"); tft.setCursor(110, 80); tft.print("Z");
  tft.setTextColor(ILI9340_YELLOW);tft.setTextSize(5); tft.setCursor(140, 110);  tft.print("Accel");
  tft.setTextSize(3); tft.setCursor(110, 103); tft.print("X"); tft.setCursor(110, 126);tft.print("Y"); tft.setCursor(110, 150);tft.print("Z");
  tft.setTextColor(ILI9340_RED);tft.setTextSize(3); tft.setCursor(110, 173);  tft.print("C");
  tft.setTextSize(5); tft.setCursor(140, 165);  tft.print("Temp");
}

void loop(void) {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  // debug Serial.print(" | GyX = "); Serial.print(GyX);
  //       Serial.print(" | GyY = "); Serial.print(GyY);
  //       Serial.print(" | GyZ = "); Serial.println(GyZ); 
  tft.setTextSize(3); tft.setCursor(0, 30);  
  tft.setTextColor(ILI9340_GREEN) ;  tft.println(GyX);  tft.println(GyY); tft.println(GyZ);
  tft.setTextColor(ILI9340_YELLOW);  tft.println(AcX);  tft.println(AcY); tft.println(AcZ);
  tft.setTextColor(ILI9340_RED);tft.print(Tmp/340.00+36.53);
 
 delay(100);
  tft.setTextSize(3); tft.setCursor(0, 30);
  tft.setTextColor(ILI9340_BLACK);tft.println(GyX);tft.println(GyY);tft.println(GyZ);
  tft.setTextColor(ILI9340_BLACK);tft.println(AcX);tft.println(AcY);tft.println(AcZ);
  tft.setTextColor(ILI9340_BLACK);tft.print(Tmp/340.00+36.53);
   }
