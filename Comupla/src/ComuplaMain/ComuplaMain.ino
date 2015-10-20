/**
 * @file 	AlbumPlayer.h
 * @author 	hn [at] holgernauen [dot] de
 * @date 	06.06.2015
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

#include <EEPROM.h>

#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>

#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <MPR121.h>

#include <ComuplaWiring.h>
#include <ComuplaTools.h>
#include <ComuplaTcs34725.h>
#include <ComuplaAnalogButton.h>
#include <ComuplaLED.h>
#include "AlbumPlayer.h"

SdFat sd;
SFEMP3Shield MP3player;
ComuplaTcs34725 tcs34725;
ComuplaAnalogButton analogButton[ANALOG_BUTTON_COUNT];
ComuplaTools tools;

#define LED_COUNT (DIGITAL_LED_COUNT+ANALOG_LED_COUNT+MPR121_LED_COUNT)
ComuplaLED led[LED_COUNT];
enum LED_IDX {
	IDX_TCS34725_LED,
	IDX_REPEAT_LED,
	IDX_PLAYALL_LED,
	IDX_PAUSE_LED,
	IDX_RANDOM_LED,
	IDX_PLAY_1_LED,
	IDX_PLAY_2_LED,
	IDX_PLAY_3_LED,
	IDX_PLAY_4_LED
};

int MPR121ElectrodeLedIdx[MPR121_ELECTRODE_COUNT] = {
	IDX_RANDOM_LED,
	IDX_PLAYALL_LED,
	IDX_REPEAT_LED,
	IDX_PLAY_4_LED, //NEXT_BUTTON,
	IDX_PLAY_3_LED, //PREV_BUTTON,
	IDX_PLAY_2_LED, //VOLUME_UP_BUTTON,
	IDX_PLAY_1_LED, //VOLUME_DOWN_BUTTON,
	IDX_PAUSE_LED
};


AlbumPlayer albumPlayer;

byte volume = DEFAULT_VOLUME;

bool isPause;
bool isRandom;
bool isPlayAll;
bool isRepeat;
bool isInserted;
int currentAlbum;

void onInsertButton(int analogPin, boolean pressed){
	isInserted = pressed;
	bool ledState = !isInserted;
	for (int i=0;i<MPR121_LED_COUNT; i++){
		led[IDX_PLAY_1_LED+i].toggle(ledState);
	}
	led[IDX_TCS34725_LED].toggle(isInserted);
	if (isInserted){
		// warm up
		tcs34725.read();
	}
}


void onPlayButton(int analogPin, boolean pressed){
	byte album = analogPin - PIN_PLAY_1_BUTTON;

	led[IDX_PLAY_1_LED+album].toggle(pressed);

	//MPR121.digitalWrite(PIN_PLAY_1_LED+album, pressed?HIGH:LOW);
	if (isInserted && !pressed){
		if (MP3player.isPlaying()){
			albumPlayer.stop();
		} else {
			//@todo: Get quartet once after insert asyncron and switch off TCS-LED afterwards.
			tcs34725.read();
			byte quartet = tools.getNearestColor(tcs34725.color);

			albumPlayer.start(quartet,album,tools.getFileCount(quartet));
		}
	}
}


void onTrackChange(int albumIdx, int trackIdx){
	if (currentAlbum != albumIdx){
		if (currentAlbum != NO_ALBUM){
			led[IDX_PLAY_1_LED+currentAlbum].toggle(false);
		}
		currentAlbum = albumIdx;
		if (currentAlbum != NO_ALBUM){
			led[IDX_PLAY_1_LED+currentAlbum].toggle(true);
		} else {
			isPause = false;
			led[IDX_PAUSE_LED].toggle(isPause);
		}
	}

	if (currentAlbum != NO_ALBUM){
		led[IDX_PLAY_1_LED+currentAlbum].blink(trackIdx+1);
	}
}


inline void setupMPR121() {
	if(!MPR121.begin(0x5C)){
		tools.halt(ERROR_MPR121_BEGIN);
	}

	MPR121.setInterruptPin(4);
	MPR121.setTouchThreshold(40);
	MPR121.setReleaseThreshold(20);
	MPR121.updateTouchData();

	MPR121.setNumDigPins(MPR121_LED_COUNT);
}

inline void setupLeds(){
	for (int i=0; i<DIGITAL_LED_COUNT; i++){
		led[IDX_TCS34725_LED+i].begin(PIN_TCS34725_LED+i);
	}
	for (int i=0; i<ANALOG_LED_COUNT; i++){
		led[IDX_RANDOM_LED+i].begin(PIN_RANDOM_LED+i);
	}
	for (int i=0; i<MPR121_LED_COUNT; i++){
		led[IDX_PLAY_1_LED+i].begin(PIN_PLAY_1_LED+i,true);
	}
}

inline void setupAnalogButtons(){
	for (int i=IDX_PLAY_1_BUTTON; i<ANALOG_BUTTON_COUNT; i++){
		analogButton[i].begin(PIN_PLAY_1_BUTTON-1+i, &onPlayButton);
	}
	analogButton[0].begin(PIN_INSERT_BUTTON, &onInsertButton);
}

void setup() {

	//Serial.begin(9600);

	tools.begin();

	if(!tcs34725.begin()){
		tools.halt(ERROR_RGB_SENSOR_BEGIN);
	}

	if(!sd.begin(SD_SEL, SPI_FULL_SPEED)){
		tools.halt(ERROR_SD_BEGIN);
	}
	if(!sd.chdir("/")) {
		tools.halt(ERROR_SD_CHDIR);
	}

	uint8_t result;
	result = MP3player.begin();
	if(result != 0) {
		tools.halt(result==6?ERROR_MP3PLAYER_PATCH_MISSING:ERROR_MP3PLAYER_BEGIN);
	}

	currentAlbum = NO_ALBUM;

	volume = tools.getVolume();
	MP3player.setVolume(volume);
	albumPlayer.begin(&sd, &MP3player, &onTrackChange);

	setupMPR121();
	setupLeds();
	setupAnalogButtons();

	isPause = false;
	isRandom = false;
	isPlayAll = false;
	isRepeat = false;
	isInserted = analogButton[IDX_INSERT_BUTTON].isPressed;
}

void loop(void) {

	if(MPR121.touchStatusChanged()){
		MPR121.updateTouchData();
		for(int i=0; i<MPR121_ELECTRODE_COUNT; i++){

			if(MPR121.isNewTouch(i)){

				led[MPR121ElectrodeLedIdx[i]].toggle(true);

			} else if(MPR121.isNewRelease(i)){

				switch (i){
				case PAUSE_BUTTON:
					isPause = !isPause;
					isPause = albumPlayer.setPause(isPause);
					led[MPR121ElectrodeLedIdx[i]].toggle(isPause);
					break;
				case RANDOM_BUTTON:
					isRandom = !isRandom;
					albumPlayer.setRandom(isRandom);
					led[MPR121ElectrodeLedIdx[i]].toggle(isRandom);
					break;
				case PLAYALL_BUTTON:
					isPlayAll = !isPlayAll;
					albumPlayer.setPlayAll(isPlayAll);
					led[MPR121ElectrodeLedIdx[i]].toggle(isPlayAll);
					break;
				case REPEAT_BUTTON:
					isRepeat = !isRepeat;
					albumPlayer.setRepeat(isRepeat);
					led[MPR121ElectrodeLedIdx[i]].toggle(isRepeat);
					break;
				case NEXT_BUTTON:
					if (currentAlbum != NO_ALBUM && !isPause){
						albumPlayer.nextTrack();
					}
					break;
				case PREV_BUTTON:
					if (currentAlbum != NO_ALBUM && !isPause){
						albumPlayer.prevTrack();
					}
					break;
				case VOLUME_UP_BUTTON:
					if (volume>MAX_VOLUME)volume--;
					MP3player.setVolume(volume);
					tools.storeVolume(volume);
					break;
				case VOLUME_DOWN_BUTTON:
					if (volume<MIN_VOLUME)volume++;
					MP3player.setVolume(volume);
					tools.storeVolume(volume);
					break;
				}

				led[MPR121ElectrodeLedIdx[i]].toggle(false);
			}
		}
	}

	for (int i=0; i<ANALOG_BUTTON_COUNT; i++){
		analogButton[i].process();
	}

	for (int i=0; i<LED_COUNT; i++){
		led[i].process();
	}

	albumPlayer.process();
}

