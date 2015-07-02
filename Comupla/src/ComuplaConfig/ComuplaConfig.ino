

#include <EEPROM.h>

#include <Wire.h>
#include <Adafruit_TCS34725.h>

#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>

#include <ComuplaWiring.h>
#include <ComuplaTools.h>
#include <ComuplaTcs34725.h>




SdFat sdFat;
ComuplaTcs34725 tcs34725;
ComuplaTools tools;


void help(){
	Serial.println(F("b: [B]link LEDs"));
	Serial.println(F("c <number>: Set new color [C]ount"));
	Serial.println(F("d: SD [D]irectory listing"));
	Serial.println(F("f: Check MP3 [F]iles"));
	Serial.println(F("i: [I]nitialize EEPROM"));
	Serial.println(F("l: [L]ist EEPROM"));
	Serial.println(F("n: Compute [N]earest color, (r)ead before"));
	Serial.println(F("r: [R]ead color from tcs34725"));
	Serial.println(F("s  <number>: [S]tore color 1..16, (r)ead before"));
}

void blink(int pin){
	digitalWrite(pin, HIGH);
	delay(100);
	digitalWrite(pin, LOW);

	for (int i=0; i<10; i++){
		analogWrite(pin,10*i);
		delay(200);
	}

	delay(100);
	digitalWrite(pin, HIGH);
	delay(100);
	digitalWrite(pin, LOW);
}

void setup()
{
	Serial.begin(9600);
	Serial.println(">");

	tools.begin();

	if(!tcs34725.begin()){
		tools.halt(ERROR_RGB_SENSOR_BEGIN);
	}
	pinMode(TCS34725_LED, OUTPUT);
	digitalWrite(TCS34725_LED, HIGH);

	if(!sdFat.begin(SD_SEL, SPI_FULL_SPEED)){
		tools.halt(ERROR_SD_BEGIN);
	}

	if(!sdFat.chdir("/")) {
		tools.halt(ERROR_SD_CHDIR);
	}

	pinMode(REPEAT_LED, OUTPUT);
	pinMode(PLAYALL_LED, OUTPUT);
	pinMode(ACTIVITY_LED, OUTPUT);
	pinMode(RANDOM_LED, OUTPUT);

}

void loop()
{
	byte byteRead;
	byte numberRead;

	if (Serial.available()) {
		while (Serial.available()) {
			byteRead = Serial.read();

			switch (byteRead) {

			case 'b': {
				blink(TCS34725_LED);
				blink(REPEAT_LED);
				blink(PLAYALL_LED);
				blink(ACTIVITY_LED);
				blink(RANDOM_LED);
			}
			break;
			case 'c': {
				numberRead = Serial.parseInt();
				if (numberRead>0 && numberRead<COLORT_MAX_COUNT) {
					tools.storeColorCount(numberRead);
					Serial.print("New color count: ");
					Serial.print(numberRead);
					Serial.println();
				} else {
					Serial.println("Usage: c number");
				}
			}
			break;

			case 'd': {
				sdFat.ls(LS_DATE|LS_SIZE|LS_R);
			}
			break;
			case 'f': {
				tools.checkMP3Files(sdFat);
			}
			break;

			case 'i': {
				tools.initializeEEPROM();
			}
			break;

			case 'l': {
				tools.listCurrentValues();
			}
			break;

			case 'n': {
				double distance = 0;
				byte idx = tools.getNearestColor(tcs34725.color, &distance);
				Serial.print(F("Nearest:"));
				Serial.print(idx+1);
				Serial.print(F(", distance:"));
				Serial.print(distance);
				Serial.println();
			}
			break;

			case 'r': {
				tcs34725.read();
				Serial.print(F(" R:"));
				Serial.print(tcs34725.color.r);
				Serial.print(F(" G:"));
				Serial.print(tcs34725.color.g);
				Serial.print(F(" B:"));
				Serial.print(tcs34725.color.b);
				Serial.print(F(" C:"));
				Serial.print(tcs34725.color.c);
				Serial.println();
			}
			break;

			case 's': {
				numberRead = Serial.parseInt();
				if (numberRead>0 && numberRead<=COLORT_MAX_COUNT) {
					tools.storeColor(numberRead,tcs34725.color);
					Serial.print("New color: ");
					Serial.print(numberRead-1);
					Serial.println();
				} else {
					Serial.println("Usage: s number");
				}
			}
			break;

			default:{
				help();
			}
			break;
			}
		}
		Serial.println(">");
	}

}
