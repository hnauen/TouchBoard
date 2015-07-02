/*
 * ComuplaTools.h
 *
 *  Created on: 31.05.2015
 *      Author: holger.nauen
 */

#ifndef COMUPLATOOLS_H_
#define COMUPLATOOLS_H_

#include <Arduino.h>
#include <SdFat.h>

#include "ComuplaWiring.h"

class ComuplaTools {

public:
	void begin();
	void halt(byte errorCode);
    void fadeLED(void);
    byte getVolume();
	void storeVolume(byte volume);
	void initializeEEPROM();
    byte getColorCount();
	void storeColorCount(byte colorCount);
	void storeColor(byte idx, color_t color);
	void listCurrentValues();
    byte getNearestColor(color_t color, double *pDistance);
    void checkMP3Files(SdFat sdFat);
    byte* getFileCount(byte idx);



private:
	eeprom_data_t eepromData;
	inline void readFromEEPROM();
	inline void writeToEEPROM();

	int fadeBrightness;
    unsigned long lastFadeChange;

	char printBuffer[64];

};

#endif /* COMUPLATOOLS_H_ */
