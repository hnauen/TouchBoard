/**
 * @file 	ComuplaTcs34725.h
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

#ifndef COMUPLATCS34725_H_
#define COMUPLATCS34725_H_

#include <Wire.h>
#include <Adafruit_TCS34725.h>

#include "ComuplaWiring.h"

class ComuplaTcs34725 {

	public:
		ComuplaTcs34725(void);

		boolean begin(void);
		void read(void);
		color_t color;


	private:
		Adafruit_TCS34725 tcs;

		boolean isAvailable;
		uint16_t atime;

		struct tcs_agc {
			tcs34725Gain_t ag;
			tcs34725IntegrationTime_t at;
			uint16_t mincnt;
			uint16_t maxcnt;
		};
		static const tcs_agc agc_lst[];
		uint16_t agc_cur;

		void getData(void);
		void setGainTime(void);
};

#endif /* COMUPLATCS34725_H_ */
