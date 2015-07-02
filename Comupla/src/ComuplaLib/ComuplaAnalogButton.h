/*
 * AnalogButton.h
 *
 *  Created on: 31.05.2015
 *      Author: holger.nauen
 */

#ifndef COMUPLAANALOGBUTTON_H_
#define COMUPLAANALOGBUTTON_H_

#define DEBOUNCE_MILLIS 50

/**
  Calback for changes on button state.
  @param pressed true if the button is pressed.
 */
typedef void (*AnalogButtonCallbackFunction) (int analogPin, bool pressed);

class ComuplaAnalogButton {

public:
	void begin(int analogPin, AnalogButtonCallbackFunction callback);
	void process();
	bool isPressed;

private:
	int analogPin;
	unsigned long lastChange;
	AnalogButtonCallbackFunction callback;
};

#endif /* COMUPLAANALOGBUTTON_H_ */
