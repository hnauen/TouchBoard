/**
 * @file 	ComuplaAnalogButton.h
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
