/**
 * @file 	ComuplaTools.h
 * @author 	hn [at] holgernauen [dot] de
 * @date 	31.05.2015
 *
 * @section LICENSE
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * @section DESCRIPTION
 *
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
    byte getVolume();
	void storeVolume(byte volume);
	void initializeEEPROM();
    byte getColorCount();
	void storeColorCount(byte colorCount);
	void storeColor(byte idx, color_t color);
	void listCurrentValues();
    byte getNearestColor(color_t color, double *pDistance = NULL);
    void checkMP3Files(SdFat sdFat);
    byte* getFileCount(byte idx);

private:
	eeprom_data_t eepromData;
	inline void readFromEEPROM();
	inline void writeToEEPROM();

	char printBuffer[64];

};

#endif /* COMUPLATOOLS_H_ */
