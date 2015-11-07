/**
 * @file 	ComuplaLED.h
 * @author 	hn [at] holgernauen [dot] de
 * @date 	28.06.2015
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

#ifndef LIBRARIES_COMUPLALIB_COMUPLALED_H_
#define LIBRARIES_COMUPLALIB_COMUPLALED_H_

#include <MPR121.h>

#define COMUPLALED_DELAY_MILLIS 500

class ComuplaLED {

public:
	void begin(int digitalPin, bool useMPR121 = false);

	void process();
	void toggle(bool on);

private:
	inline void setPinMode();
	inline void switchOn();
	inline void switchOff();
	void led(int state);
	void delayOn();

	int digitalPin;
	int numOn;
	unsigned long deleayOnMillis;
	bool useMPR121;
};

#endif /* LIBRARIES_COMUPLALIB_COMUPLALED_H_ */
