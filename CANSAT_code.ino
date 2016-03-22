#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <SPI.h>
#include <RTClib.h>
#include <RTC_DS3234.h>

// Avoid spurious warnings
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))
#undef PSTR
#define PSTR(s) (__extension__({static prog_char __c[] PROGMEM = (s); &__c[0];}))

// Create an RTC instance, using the chip select pin it's connected to
RTC_DS3234 RTC(8);

//Define pressure
MPL3115A2 myPressure;

void setup() {
  
  Wire.begin();       // Join I2C bus
  Serial.begin(9600); //Start serial for output
  SPI.begin();
  RTC.begin();

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    Serial.print("Setting time to... ");
    Serial.print(__DATE__);
    Serial.print(' ');
    Serial.println(__TIME__);
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  myPressure.begin(); // Get sensor online
  
  //Configure the sensor
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 

}

void loop() {
  const int len = 32;
  static char buf[len];

  DateTime now = RTC.now();

  Serial.println(now.toString(buf,len));
  
  float altitude = myPressure.readAltitude();
  Serial.print(" Altitude(m):");
  Serial.print(altitude, 2);

  float temperature = myPressure.readTempF();
  Serial.print(" Temp(f):");
  Serial.print(temperature, 2);

  

  Serial.println();

}
