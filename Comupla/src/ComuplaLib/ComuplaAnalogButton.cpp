/*
 * AnalogButton.cpp
 *
 *  Created on: 31.05.2015
 *      Author: holger.nauen
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

