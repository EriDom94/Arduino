/*
 * TH02.cpp
 * Library for HopeRF TH02 temperature and humidity sensor
 * 
 * Based on:
 * 	https://github.com/Seeed-Studio/Grove_Temper_Humidity_TH02
 *
 * Updated: Jan 14, 2018 by EriDom94
 * 	- Add compatibility with the ATTiny85 mcu
 *  - Add compensated calulated humidity procedure
 * 	- Rewrite some procedure
 *
 * History: 
 * 	v1.0 2018-01-14 - First release
 *
 */

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "Arduino.h"
#include "TH02.h"

/****************************************************************************/
/***       Class member Functions                                         ***/
/****************************************************************************/
TH02::TH02() {
}

void TH02::init(void) {
  // Start IIC
  Wire.begin();
}

TH02_Measures TH02::getMeasures(bool compensated) {
  TH02_Measures measures;
  measures.temperature = _readTemperature();
  measures.humidity = _readHumidity();
	
	if (compensated)
		measures.humidity = _computeCompensatedHum(measures);
	
  return measures;
}

/****************************************************************************/
/***       Local Functions                                                ***/
/****************************************************************************/
boolean TH02::_isAvailable() {
  uint8_t status = _readReg(REG_STATUS);
	return !(status & STATUS_RDY_MASK);
}

void TH02::_writeCmd(uint8_t u8Cmd) {
	Wire.beginTransmission(TH02_I2C_DEV_ID);
	Wire.write(u8Cmd);
	Wire.endTransmission();
}

uint8_t TH02::_readReg(uint8_t u8Reg) {
  uint8_t data = 0;
  _writeCmd(u8Reg);	 
	Wire.requestFrom(TH02_I2C_DEV_ID, 1);	 
	while (Wire.available()) {
	  data = Wire.read();	 
	}
	return data;
} 

void TH02::_writeReg(uint8_t u8Reg,uint8_t u8Data) {           
	Wire.beginTransmission(TH02_I2C_DEV_ID);	 
	Wire.write(u8Reg);	 
	Wire.write(u8Data);	 
	Wire.endTransmission();	 
}

uint16_t TH02::_readData(void) {
	uint16_t data = _readData2byte(); 
	return data;
}

uint16_t TH02::_readData2byte() {
  uint16_t tmpData = 0;
	uint16_t tmpArray[3] = {0};
	int cnt = 0;
	_writeCmd(REG_DATA_H);	
	Wire.requestFrom(TH02_I2C_DEV_ID, 3);	 
	while (Wire.available()) {
	  tmpArray[cnt] = (uint16_t)Wire.read();        	        	
		cnt++;
	}
	tmpData = (tmpArray[1]<<8) | (tmpArray[2]); 
	return tmpData;
}

float TH02::_computeCompensatedHum(TH02_Measures measures) {
  float rhvalue, rhlinear;

  rhvalue = measures.humidity;
	// apply linear compensation
  rhlinear = rhvalue - ((rhvalue * rhvalue) * TH02_A2 + rhvalue * TH02_A1 + TH02_A0);
  // apply Temperature compensation
  rhvalue += (measures.temperature - 30.0) * (rhlinear * TH02_Q1 + TH02_Q0);
  
  return rhvalue;
}

float TH02::_readTemperature(void) {    
  // Start a new temperature conversion
	_writeReg(REG_CONFIG, CMD_MEASURE_TEMP);
	
	// Wait until conversion is done
	while(!_isAvailable());
	
	uint16_t data = _readData();
	data = data >> 2;
	
	// Formula: Temperature(C) = (data / 32) - 50
	return ((data / 32.0) - 50.0);
}
 
float TH02::_readHumidity(void) {
	// Start a new humility conversion
	_writeReg(REG_CONFIG, CMD_MEASURE_HUMI);
	
	// Wait until conversion is done
	while(!_isAvailable());
	
	uint16_t data = _readData();
	data = data >> 4;
	
	// Formula: Humidity(%) = (data / 16) - 24
	return ((data / 16.0) - 24.0);
}