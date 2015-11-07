/**
 * @file 	ComuplaTools.cpp
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

#include "ComuplaTools.h"

#include <EEPROM.h>
#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>

void ComuplaTools::begin(){
	readFromEEPROM();
}

void ComuplaTools::halt(byte errorCode){
	while (true) {
		for (int i=0; i<errorCode; i++) {
			delay(200);
			digitalWrite(LED_BUILTIN, HIGH);
			delay(200);
			digitalWrite(LED_BUILTIN, LOW);
		}
		delay(400);
	}
}


void ComuplaTools::writeToEEPROM(){
	EEPROM.put(0, eepromData);
}

void ComuplaTools::readFromEEPROM(){
	EEPROM.get(0, eepromData);
}

byte ComuplaTools::getVolume(){
	return eepromData.volume;
}

void ComuplaTools::storeVolume(byte volume){
	eepromData.volume = volume;
	writeToEEPROM();
}

void ComuplaTools::initializeEEPROM(){
	memset(&eepromData,0,sizeof(eepromData));
	eepromData.volume = DEFAULT_VOLUME;
	eepromData.colorCount = MAX_COLORT_COUNT;
	writeToEEPROM();
}

byte ComuplaTools::getColorCount(){
	return eepromData.colorCount;
}

void ComuplaTools::storeColorCount(byte colorCount){
	if (colorCount <= MAX_COLORT_COUNT){
		eepromData.colorCount = colorCount;
		writeToEEPROM();
	} else {
		Serial.println("storeColorCount: Invalid colorCount");
	}
}

void ComuplaTools::storeColor(byte idx, color_t color) {
	if (idx <= MAX_COLORT_COUNT){
		eepromData.color[idx] = color;
		writeToEEPROM();
	} else {
		Serial.println("storeColor: Invalid idx");
	}
}

void ComuplaTools::listCurrentValues(){

	sprintf(printBuffer,"vol: %2d", eepromData.volume);
	Serial.println(printBuffer);

	sprintf(printBuffer,"cnt: %2d", eepromData.colorCount);
	Serial.println(printBuffer);

	for (byte i=0; i<MAX_COLORT_COUNT; i++){
		sprintf(printBuffer, "%02d : %05u %05u %05u %05u - %02d %02d %02d %02d", i+1,
				eepromData.color[i].r,
				eepromData.color[i].g,
				eepromData.color[i].b,
				eepromData.color[i].c,
				eepromData.fileCount[i][0],
				eepromData.fileCount[i][1],
				eepromData.fileCount[i][2],
				eepromData.fileCount[i][3]);
		Serial.println(printBuffer);
	}
}

byte ComuplaTools::getNearestColor(color_t color, double* pDistance) {

	byte nearestColor;
	double nearestDistance, distance;

	nearestDistance = /*sqrt*/(
			pow(float(color.r)-float(eepromData.color[0].r),2) +
			pow(float(color.g)-float(eepromData.color[0].g),2) +
			pow(float(color.b)-float(eepromData.color[0].b),2) +
			pow(float(color.c)-float(eepromData.color[0].c),2)
	);
	nearestColor = 0;

	for (int i=1; i<eepromData.colorCount; i++){
		distance = /*sqrt*/(
				pow(float(color.r)-float(eepromData.color[i].r),2) +
				pow(float(color.g)-float(eepromData.color[i].g),2) +
				pow(float(color.b)-float(eepromData.color[i].b),2) +
				pow(float(color.c)-float(eepromData.color[i].c),2)
		);

		if (distance < nearestDistance){
			nearestDistance = distance;
			nearestColor = i;
		}
	}

	if (pDistance != NULL){
		*pDistance = nearestDistance;
	}

	return nearestColor;

}

void ComuplaTools::checkMP3Files(SdFat sdFat){
	char directoryName[]="000";
	for (int i=0; i<MAX_COLORT_COUNT; i++) {
		for (int j=0; j<4; j++) {
			sprintf(directoryName, "%02d%1d", i+1, j+1);
			sdFat.chdir("/");
			if (sdFat.chdir(directoryName)) {

				byte fileCount = 0;
				SdFile file, f;
				char fileName[13];
				while (file.openNext(sdFat.vwd(),O_READ)) {
					file.getFilename(fileName);
					file.close();

					boolean isOk = false;
					boolean isIgnored = false;

					if ( isdigit(fileName[0]) && isdigit(fileName[1]) && strcmp(&fileName[2],".MP3")==0 ){
						fileCount++;
						isOk = true;
					} else if (fileName[0]=='_') {
						isOk = true;
						isIgnored = true;
					}

					sprintf(printBuffer,"/%s/%s - %s", directoryName, fileName, isOk?(isIgnored?"ignored":"ok"):"CHECK");
					Serial.println(printBuffer);
				}

				eepromData.fileCount[i][j] = fileCount;

				if (fileCount==0) {
					sprintf(printBuffer,"/%s - dir empty", directoryName);
					Serial.println(printBuffer);
				}

			} else {
				sprintf(printBuffer,"/%s - dir not found", directoryName);
				Serial.println(printBuffer);

				eepromData.fileCount[i][0] = 0;
				eepromData.fileCount[i][1] = 0;
				eepromData.fileCount[i][2] = 0;
				eepromData.fileCount[i][3] = 0;
			}
		}
	}

	sdFat.chdir("/");
	writeToEEPROM();
}

byte* ComuplaTools::getFileCount(byte idx) {
	return &eepromData.fileCount[idx][0];
}
