/*
 * TH02_demo.ino
 * Library for HopeRF TH02 temperature and humidity sensor
 * 
 * Based on:
 *   https://github.com/Seeed-Studio/Grove_Temper_Humidity_TH02
 *
 * Updated: Jan 14, 2018 by EriDom94
 *  - Add compatibility with the ATTiny85 mcu
 *  - Add compensated calulated humidity procedure
 *  - Rewrite some procedure
 *
 * History: 
 *  v1.0 2018-01-14 - First release
 *
 */
 
#ifdef __AVR_ATtiny85__
  #include <SoftwareSerial.h>
  SoftwareSerial Serial(3, 4);
#endif

#include <TH02.h>

TH02 sensor;
 
void setup() {  
  Serial.begin(9600); 
  Serial.println("**** TH02 demo ****\n");
  delay(150);
  sensor.init();
  delay(100);
}

void loop() {
  TH02_Measures data = sensor.getMeasures(true);
  
  Serial.print("Temperature: ");   
  Serial.print(data.temperature);
  Serial.println("C");
  Serial.print("Humidity compensated: ");
  Serial.print(data.humidity);
  Serial.println("%");
	 
  delay(2000);
}
 
