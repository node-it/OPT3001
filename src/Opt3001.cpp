/*
  OPT3001.cpp
  Created by Adrian Fernandez & Dung Dang, Dec 2013
  
  Released into the public domain.
*/

#include <Arduino.h>
#include "Opt3001.h"
#define slaveAdr 0x44

#define RESULT_REG 0x00
#define CONFIG_REG 0x01
#define LOWLIMIT_REG 0x02
#define HIGHLIMIT_REG 0x03
#define MANUFACTUREID_REG 0x7E
#define DEVICEID_REG 0x7F

void Opt3001::begin(uint16_t config)
{
	/* Begin Transmission at address of device on bus */
	Wire.beginTransmission(slaveAdr);

	/* Send Pointer Register Byte */
	Wire.write(CONFIG_REG);

	/* Read*/
	Wire.write((unsigned char)(config >> 8));
	Wire.write((unsigned char)(config & 0x00FF));

	/* Sends Stop */
	Wire.endTransmission();
	return;
    
}

void Opt3001::begin()
{
    begin(DEFAULT_CONFIG_800);
}

uint16_t Opt3001::readRegister(uint8_t registerName)
{
	int8_t lsb;
	int8_t msb;
	int16_t result;


	/* Begin Transmission at address of device on bus */
	Wire.beginTransmission(slaveAdr);

	/* Send Pointer to register you want to read */
	Wire.write(registerName);

	/* Sends Stop */
	Wire.endTransmission(true);

	/* Requests 2 bytes from Slave */
	Wire.requestFrom(slaveAdr, 2);

	/* Wait Until 2 Bytes are Ready*/
	while(Wire.available() < 2)	{}

	/* Read*/
	msb = Wire.read();
	lsb = Wire.read();
	result = (msb << 8) | lsb;

	return result;
}


uint16_t Opt3001::readManufacturerId()
{

	return readRegister(MANUFACTUREID_REG);
	
}

uint16_t Opt3001::readDeviceId()
{
	return readRegister(DEVICEID_REG);
		
}

uint16_t Opt3001::readConfigReg()
{
	return readRegister(CONFIG_REG);
}

uint16_t Opt3001::readLowLimitReg()
{
	return readRegister(LOWLIMIT_REG);
	
	
}

uint16_t Opt3001::readHighLimitReg()
{
	return readRegister(HIGHLIMIT_REG);
}


float Opt3001::readResult()
{
	uint16_t exponent;
	uint16_t result;
    uint16_t raw;
	
    raw = readRegister(RESULT_REG);
    /* Convert to LUX */
	result = raw & 0x0fff;
	exponent = (raw & 0xf000) >> 12;

//    Serial.print("Calc: result = ");
//    Serial.print(result, DEC);
//    Serial.print(", exponent = ");
//    Serial.println(exponent, DEC);

	return result * (0.01 * exp2(exponent));
	
}

uint8_t Opt3001::interruptPin()
{
	return (digitalRead(OPT_INTERRUPT_PIN)==0?1:0);
}

boolean Opt3001::isConversionReady()
{
    return readConfigReg() & OPT3001_CFG_CRF ? true : false;
}

void Opt3001::startConversion()
{
    begin(DEFAULT_CONFIG_100_OS);
}

void Opt3001::shutDown()
{
    begin(DEFAULT_CONFIG_SHDWN);
}

// EOF
