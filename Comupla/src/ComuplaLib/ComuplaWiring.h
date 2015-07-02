/*
 * Config.h
 *
 *  Created on: 30.05.2015
 *      Author: holger.nauen
 */

#ifndef COMUPLACONFIG_H_
#define COMUPLACONFIG_H_

#include <Arduino.h>

// ----------------------------------------------
// Arduino Digital Pins
// ----------------------------------------------

// Digital Led Pins
enum DIGITAL_LED_PIN {
	TCS34725_LED = 10,	// LED in TCS34725
	REPEAT_LED, 			// Repeat Album(s)
	PLAYALL_LED, 			// Play all 4 Albums
	ACTIVITY_LED			// Show Activities (== LED_BUILTIN)
};

// PWM
#define ACTIVITY_LED_OFF    0
#define ACTIVITY_LED_ON    	24
#define ACTIVITY_LED_HIGH  	255

#define FADE_DELAY 			50

// ----------------------------------------------
// Arduino Analog Pins
// ----------------------------------------------

// Analog Button Pins
#define ANALOG_NUM_BUTTONS 5
enum ANALOG_BUTTON_PIN {
	PIN_INSERT_BUTTON = A0,
	PIN_PLAY_1_BUTTON,
	PIN_PLAY_2_BUTTON,
	PIN_PLAY_3_BUTTON,
	PIN_PLAY_4_BUTTON
};
enum ANALOG_BUTTON_ID {
	INSERT_BUTTON,
	PLAY_1_BUTTON,
	PLAY_2_BUTTON,
	PLAY_3_BUTTON,
	PLAY_4_BUTTON
};

// Analog Led Pins
#define RANDOM_LED A5


// ----------------------------------------------
// MPR121 - Capacitive Touch Sensor
// ----------------------------------------------

// MPR121 Electrodes
#define MPR121_NUM_ELECTRODES 8
enum MPR121_ELECTRODE {
	RANDOM_BUTTON,
	PLAYALL_BUTTON,
	REPEAT_BUTTON,
	NEXT_BUTTON,
	PREV_BUTTON,
	VOLUME_UP_BUTTON,
	VOLUME_DOWN_BUTTON,
	ON_BUTTON
};

// MPR121 Digital Pins
#define MPR121_NUM_DIG_PINS 4
enum MPR121_PIN {
	PIN_PLAY_1_LED = 8,
	PIN_PLAY_2_LED,
	PIN_PLAY_3_LED,
	PIN_PLAY_4_LED
};


// ----------------------------------------------
// SFEMP3Shield - The MP3 Player
// ----------------------------------------------

#define MAX_VOLUME 0
#define DEFAULT_VOLUME 20
#define MIN_VOLUME 40


// ----------------------------------------------
// EEPROM - Storage of colors and album info
// ----------------------------------------------

#define COLORT_MAX_COUNT 16

typedef struct {
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t c;
} color_t;

typedef struct {
	byte volume;
	byte colorCount;
	color_t color[COLORT_MAX_COUNT];
	byte fileCount[COLORT_MAX_COUNT][4];
} eeprom_data_t;


// ----------------------------------------------
// Misc
// ----------------------------------------------

enum ERRORCODE {
	ERROR_RGB_SENSOR_BEGIN = 1,
	ERROR_MPR121_BEGIN,
	ERROR_SD_BEGIN,
	ERROR_SD_CHDIR,
	ERROR_MP3PLAYER_BEGIN,
	ERROR_MP3PLAYER_PATCH_MISSING
};


#endif /* COMUPLACONFIG_H_ */
