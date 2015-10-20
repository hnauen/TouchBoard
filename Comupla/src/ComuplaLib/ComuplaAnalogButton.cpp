/**
 * @file 	ComuplaAnalogButton.cpp
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

#include "ComuplaAnalogButton.h"

#include <Arduino.h>

void ComuplaAnalogButton::begin(int analogPin, AnalogButtonCallbackFunction callback){
	this->analogPin = analogPin;
	this->callback = callback;
	this->lastChange = millis();

	pinMode(this->analogPin, INPUT_PULLUP);
	this->isPressed = (analogRead(this->analogPin)==LOW);
	this->callback(this->analogPin, this->isPressed);
}


void ComuplaAnalogButton::process(){
	bool pressed = (analogRead(this->analogPin)<512);

	// Detect change
	if (this->isPressed != pressed){
		unsigned long now = millis();

		// Overflow?
		if (now < this->lastChange){
			this->lastChange = now;
		}

		// Debounce!
		if ((now - this->lastChange) > DEBOUNCE_MILLIS){
			this->lastChange = now;
			this->isPressed = pressed;
			this->callback(this->analogPin, this->isPressed);
		}
	}
}

