/*
 * TCS34725.h
 *
 * Based on the autorange class for TCS34725 from Adafruit
 *
 *  Created on: 30.05.2015
 *      Author: holger.nauen
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
