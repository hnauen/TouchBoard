/**
 * @file 	ComuplaTcs34725.cpp
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

#include "ComuplaTcs34725.h"
#include <Arduino.h>

//
// Gain/time combinations to use and the min/max limits for hysteresis
// that avoid saturation. They should be in order from dim to bright.
//
// Also set the first min count and the last max count to 0 to indicate
// the start and end of the list.
//
const ComuplaTcs34725::tcs_agc ComuplaTcs34725::agc_lst[] = {
		{ TCS34725_GAIN_60X, TCS34725_INTEGRATIONTIME_700MS,     0, 47566 },
		{ TCS34725_GAIN_16X, TCS34725_INTEGRATIONTIME_154MS,  3171, 63422 },
		{ TCS34725_GAIN_4X,  TCS34725_INTEGRATIONTIME_154MS, 15855, 63422 },
		{ TCS34725_GAIN_1X,  TCS34725_INTEGRATIONTIME_2_4MS,   248,     0 }
};

ComuplaTcs34725::ComuplaTcs34725() {
	agc_cur= 0;
	isAvailable = false;
	atime = 0;
}

// initialize the sensor
boolean ComuplaTcs34725::begin(void) {
	tcs = Adafruit_TCS34725(agc_lst[agc_cur].at, agc_lst[agc_cur].ag);
	if ((isAvailable = tcs.begin()))
		setGainTime();
	return(isAvailable);
}

// Set the gain and integration time
void ComuplaTcs34725::setGainTime(void) {
	tcs.setGain(agc_lst[agc_cur].ag);
	tcs.setIntegrationTime(agc_lst[agc_cur].at);
	atime = int(agc_lst[agc_cur].at);
}

// Retrieve data from the sensor and do the calculations
void ComuplaTcs34725::getData(void) {
	uint16_t r, g, b, c;

	// read the sensor and autorange if necessary
	tcs.getRawData(&r, &g, &b, &c);
	while(1) {
		if (agc_lst[agc_cur].maxcnt && c > agc_lst[agc_cur].maxcnt)
			agc_cur++;
		else if (agc_lst[agc_cur].mincnt && c < agc_lst[agc_cur].mincnt)
			agc_cur--;
		else break;

		setGainTime();
		delay((256 - atime) * 2.4 * 2); // shock absorber
		tcs.getRawData(&r, &g, &b, &c);
		break;
	}

	// DN40 calculations
	uint16_t ir = (r + g + b > c) ? (r + g + b - c) / 2 : 0;
	color.r = r - ir;
	color.g = g - ir;
	color.b = b - ir;
	color.c = c - ir;

}

void ComuplaTcs34725::read(void) {
	do {
		getData();
	} while (color.r == 65535 || color.g == 65535 || color.b == 65535 || color.c == 65535);
}


