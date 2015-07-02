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

#include "AlbumPlayer.h"


SdFat sd;
SFEMP3Shield MP3player;
ComuplaTcs34725 tcs34725;
ComuplaAnalogButton analogButton[ANALOG_NUM_BUTTONS];
ComuplaTools tools;

AlbumPlayer albumPlayer;

int numTouches;
byte volume = DEFAULT_VOLUME;

boolean isOn;
boolean isRandom;
boolean isPlayAll;
boolean isRepeat;
boolean isInserted;

void displayButtonFeedback(boolean pressed){
	if (pressed){
		numTouches++;
	} else {
		numTouches--;
	}
	if (isOn){
		analogWrite(ACTIVITY_LED, numTouches>0?ACTIVITY_LED_HIGH:ACTIVITY_LED_ON);
	}
}



void onInsertButton(int analogPin, boolean pressed){
	isInserted = pressed;
	if (isOn){
		digitalWrite(TCS34725_LED, isInserted?HIGH:LOW);
		analogWrite(ACTIVITY_LED, numTouches>0?ACTIVITY_LED_HIGH:ACTIVITY_LED_ON);
	}
	// No feedback, Stop fading instead
	// displayButtonFeedback(pressed);
}


void onPlayButton(int analogPin, boolean pressed){
	byte album = analogPin - PIN_PLAY_1_BUTTON;
	displayButtonFeedback(pressed);
	MPR121.digitalWrite(PIN_PLAY_1_LED+album, pressed?HIGH:LOW);
	if (isOn && isInserted && !pressed){
		if (MP3player.isPlaying()){
			albumPlayer.stop();
		} else {
			double distance = 0;
			//@todo: Get quartet once after insert asyncron and switch off TCS-LED afterwards.
			tcs34725.read();
			byte quartet = tools.getNearestColor(tcs34725.color,&distance);

			albumPlayer.start(quartet,album,tools.getFileCount(quartet));
		}
	}

}


void onAlbumChange(int albumIdx){
	for (int i=0; i<MPR121_NUM_DIG_PINS; i++){
		MPR121.digitalWrite(PIN_PLAY_1_LED+i,i==albumIdx?HIGH:LOW);
	}
}


void resetState(void){

	isOn = false;
	isRandom = false;
	isPlayAll = false;
	isRepeat = false;
	isInserted = analogButton[INSERT_BUTTON].isPressed;

	digitalWrite(TCS34725_LED, LOW);
	digitalWrite(REPEAT_LED, LOW);
	digitalWrite(PLAYALL_LED, LOW);
	digitalWrite(ACTIVITY_LED, LOW);
	digitalWrite(RANDOM_LED, LOW);

	for (int i=0; i<MPR121_NUM_DIG_PINS; i++){
		MPR121.digitalWrite(PIN_PLAY_1_LED+i,LOW);
	}

	numTouches = 0;

	numTouches = MPR121.getNumTouches();
	for (int i=PLAY_1_BUTTON; i<ANALOG_NUM_BUTTONS; i++){
		if(analogButton[i].isPressed) numTouches++;
	}

	albumPlayer.stop();

}

void setup(void) {
	Serial.begin(9600);

	tools.begin();

	if(!tcs34725.begin()){
		tools.halt(ERROR_RGB_SENSOR_BEGIN);
	}
	pinMode(TCS34725_LED, OUTPUT);

	pinMode(REPEAT_LED, OUTPUT);
	pinMode(PLAYALL_LED, OUTPUT);
	pinMode(ACTIVITY_LED, OUTPUT);
	pinMode(RANDOM_LED, OUTPUT);


	if(!MPR121.begin(0x5C)){
		tools.halt(ERROR_MPR121_BEGIN);
	}

	MPR121.setInterruptPin(4);
	MPR121.setTouchThreshold(40);
	MPR121.setReleaseThreshold(20);
	MPR121.updateTouchData();

	MPR121.setNumDigPins(MPR121_NUM_DIG_PINS);
	MPR121.pinMode(PIN_PLAY_1_LED, OUTPUT);
	MPR121.pinMode(PIN_PLAY_2_LED, OUTPUT);
	MPR121.pinMode(PIN_PLAY_3_LED, OUTPUT);
	MPR121.pinMode(PIN_PLAY_4_LED, OUTPUT);

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

	volume = tools.getVolume();
	MP3player.setVolume(volume);

	analogButton[0].begin(PIN_INSERT_BUTTON, &onInsertButton);
	analogButton[1].begin(PIN_PLAY_1_BUTTON, &onPlayButton);
	analogButton[2].begin(PIN_PLAY_2_BUTTON, &onPlayButton);
	analogButton[3].begin(PIN_PLAY_3_BUTTON, &onPlayButton);
	analogButton[4].begin(PIN_PLAY_4_BUTTON, &onPlayButton);

	albumPlayer.begin(&sd, &MP3player, &onAlbumChange);

	resetState();

}

void loop(void) {

	if(MPR121.touchStatusChanged()){
		MPR121.updateTouchData();
		for(int i=0; i<MPR121_NUM_ELECTRODES; i++){
			if(MPR121.isNewTouch(i)){
				displayButtonFeedback(true);

			} else if(MPR121.isNewRelease(i)){

				displayButtonFeedback(false);

				if (i == ON_BUTTON){
					isOn = !isOn;
					if (isOn){
						digitalWrite(TCS34725_LED, isInserted?HIGH:LOW);
						analogWrite(ACTIVITY_LED, numTouches>0?ACTIVITY_LED_HIGH:ACTIVITY_LED_ON);
					} else {
						analogWrite(ACTIVITY_LED, ACTIVITY_LED_OFF);
						resetState();
					}

				} else if (isOn) {

					switch (i){
					case RANDOM_BUTTON:
						isRandom = !isRandom;
						digitalWrite(RANDOM_LED, isRandom?HIGH:LOW);
						albumPlayer.setRandom(isRandom);
						break;
					case PLAYALL_BUTTON:
						isPlayAll = !isPlayAll;
						digitalWrite(PLAYALL_LED, isPlayAll?HIGH:LOW);
						albumPlayer.setPlayAll(isPlayAll);
						break;
					case REPEAT_BUTTON:
						isRepeat = !isRepeat;
						digitalWrite(REPEAT_LED, isRepeat?HIGH:LOW);
						albumPlayer.setRepeat(isRepeat);
						break;
					case NEXT_BUTTON:
						albumPlayer.nextTrack();
						break;
					case PREV_BUTTON:
						albumPlayer.prevTrack();
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
				}
			}
		}
	}


	for (int i=0; i<ANALOG_NUM_BUTTONS; i++){
		analogButton[i].process();
	}

	if (isOn && !isInserted && numTouches==0){
		tools.fadeLED();
	}

	albumPlayer.process();

}

