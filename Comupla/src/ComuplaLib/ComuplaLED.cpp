/**
 * @file 	ComuplaLED.cpp
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

#include <ComuplaLED.h>
#include <Arduino.h>

void ComuplaLED::begin(int digitalPin, bool useMPR121) {
	this->digitalPin = digitalPin;
	this->useMPR121 = useMPR121;
	this->numOn = 0;
	this->deleayOnMillis = 0;
	setPinMode();
	led(LOW);
}

void ComuplaLED::process() {
	if (deleayOnMillis != 0 && deleayOnMillis < millis()){
		deleayOnMillis = 0;
		if (numOn > 0){
			led(HIGH);
		}
	}
}

void ComuplaLED::toggle(bool on) {
	if (on){
		switchOn();
	} else {
		switchOff();
	}
}

void ComuplaLED::switchOn() {
	if (numOn > 0) {
		delayOn();
	} else {
		led(HIGH);
	}
	numOn++;
}

void ComuplaLED::switchOff() {
	if (numOn > 0) {
		numOn--;
	}
	if (numOn > 0) {
		delayOn();
	} else {
		led(LOW);
	}
}

void ComuplaLED::setPinMode() {
	if (useMPR121){
		MPR121.pinMode(digitalPin,OUTPUT);
	} else {
		pinMode(digitalPin,OUTPUT);
	}
}

void ComuplaLED::led(int state) {
	if (useMPR121){
		MPR121.digitalWrite(digitalPin,state);
	} else {
		digitalWrite(digitalPin,state);
	}
}

void ComuplaLED::delayOn() {
	led(LOW);
	deleayOnMillis = millis() + COMUPLALED_DELAY_MILLIS;
}


