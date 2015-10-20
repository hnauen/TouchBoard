/**
 * @file 	ComuplaWiring.h
 * @author 	hn [at] holgernauen [dot] de
 * @date 	30.05.2015
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

#ifndef COMUPLACONFIG_H_
#define COMUPLACONFIG_H_

#include <Arduino.h>

// ----------------------------------------------
// Arduino Digital Pins
// ----------------------------------------------

// Digital Led Pins
#define DIGITAL_LED_COUNT 4
enum DIGITAL_LED_PIN {
	PIN_TCS34725_LED = 10,	// LED in TCS34725
	PIN_REPEAT_LED, 		// Repeat Album(s)
	PIN_PLAYALL_LED, 		// Play all 4 Albums
	PIN_PAUSE_LED			// Pause music (== LED_BUILTIN)
};

// ----------------------------------------------
// Arduino Analog Pins
// ----------------------------------------------

// Analog Button Pins
#define ANALOG_BUTTON_COUNT 5
enum ANALOG_BUTTON_PIN {
	PIN_INSERT_BUTTON = A0,
	PIN_PLAY_1_BUTTON,
	PIN_PLAY_2_BUTTON,
	PIN_PLAY_3_BUTTON,
	PIN_PLAY_4_BUTTON
};
enum ANALOG_BUTTON_IDX {
	IDX_INSERT_BUTTON,
	IDX_PLAY_1_BUTTON,
	IDX_PLAY_2_BUTTON,
	IDX_PLAY_3_BUTTON,
	IDX_PLAY_4_BUTTON
};

// Analog Led Pins
#define ANALOG_LED_COUNT 1
enum ANALOG_LED_PIN {
	PIN_RANDOM_LED = A5
};


// ----------------------------------------------
// MPR121 - Capacitive Touch Sensor
// ----------------------------------------------

// MPR121 Electrodes
#define MPR121_ELECTRODE_COUNT 8
enum MPR121_ELECTRODE {
	RANDOM_BUTTON,
	PLAYALL_BUTTON,
	REPEAT_BUTTON,
	NEXT_BUTTON,
	PREV_BUTTON,
	VOLUME_UP_BUTTON,
	VOLUME_DOWN_BUTTON,
	PAUSE_BUTTON
};


// MPR121 Digital Pins
#define MPR121_LED_COUNT 4
enum MPR121_LED_PIN {
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

#define MAX_COLORT_COUNT 16

typedef struct {
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t c;
} color_t;

typedef struct {
	byte volume;
	byte colorCount;
	color_t color[MAX_COLORT_COUNT];
	byte fileCount[MAX_COLORT_COUNT][4];
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
