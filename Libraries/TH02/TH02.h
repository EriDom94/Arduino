/*
 * TH02.h
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
 
#ifndef _TH02_H
#define _TH02_H

/****************************************************************************/
/***        Including Files                                               ***/
/****************************************************************************/
#include <Arduino.h>

#ifdef __AVR_ATtiny85__
	#include "TinyWireM.h"
	#define Wire TinyWireM
#else
	#include <Wire.h>
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define TH02_I2C_DEV_ID     0x40

#define REG_STATUS          0x00 
#define REG_DATA_H          0x01
#define REG_DATA_L          0x02
#define REG_CONFIG          0x03
#define REG_ID              0x11

#define STATUS_RDY_MASK     0x01

#define CMD_MEASURE_HUMI    0x01
#define CMD_MEASURE_TEMP    0x11

#define TH02_WR_REG_MODE    0xC0
#define TH02_RD_REG_MODE    0x80

// THO2 Linearization Coefficients
#define TH02_A0							-4.7844
#define TH02_A1    					0.4008
#define TH02_A2   					-0.00393

// TH02 Temperature compensation Linearization Coefficients
#define TH02_Q0   					0.1973
#define TH02_Q1   					0.00237

/****************************************************************************/
/***        Structure Definition                                          ***/
/****************************************************************************/
typedef struct {
	float temperature;
	float humidity;
} TH02_Measures;

/****************************************************************************/
/***        Class Definition                                              ***/
/****************************************************************************/
class TH02
{
public:
	TH02();
	void init();
	TH02_Measures getMeasures(bool compensated);
private:	
	bool _isAvailable(void);
	void _writeCmd(uint8_t u8Cmd);
	uint8_t _readReg(uint8_t u8Reg);
	void _writeReg(uint8_t u8Reg, uint8_t u8Data);
	uint16_t _readData(void);
	uint16_t _readData2byte(void);
	float _computeCompensatedHum(TH02_Measures ret);
	float _readTemperature(void);
	float _readHumidity(void);
};

#endif