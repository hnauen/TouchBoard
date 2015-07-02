# TouchBoard

## Projects

### [Comupla](./Comupla) - The COlor MUsic PLAyer

A MP3 player with physicalness. 


## TouchBoard with the Eclipse Arduino IDE

### Install the IDE

Download the software

- [Arduino Eclipse IDE](http://www.baeyens.it/eclipse/)

Update to the latest version

- Eclipse / Help / Check for Updates

Configure Eclipse Arduino IDE

- Add \*.ino and \*.pde as C++ source file
- Set Arduino IDE path
- Set Arduino library path
- Set Arduino hardware path  


### Install Bare Conductive hardware driver

[Instructions](http://www.bareconductive.com/make/setting-up-arduino-with-your-touch-board/)


### Tweak Bare Conductive hardware driver

Adapt boards.txt

- Add a special Arduino Eclipse IDE config to boards.txt and select this config in your scetches properties. See below for config details.
- e.g. ~/Documents/Arduino/hardware/bare-conductive-arduino-public/avr/boards.txt 

Add pins_arduino.h

- Copy pins_arduino.h from your Arduino App to the Bare Conductive hardware driver
- from: Arduino.app/Contents/Resources/Java/hardware/arduino/avr/variants/leonardo/pins_arduino.h
- to: ~/Documents/Arduino/hardware/bare-conductive-arduino-public/avr/variants/leonardo/pins_arduino.h

Add hardware libraries as needed 

- Copy libraries from your Arduino Dristribution to the Bare Conductive hardware driver
- from: Arduino.app/Contents/Resources/Java/hardware/arduino/avr/libraries/*
- to: ~/Documents/Arduino/hardware/bare-conductive-arduino-public/avr/libraries/*


### Eclipse config for board.txt

	baretouch_eclipse.name=Bare Conductive Touch Board (Eclipse)
	baretouch_eclipse.vid.0=0x2A6E
	baretouch_eclipse.pid.0=0x0003
	baretouch_eclipse.vid.1=0x2A6E
	baretouch_eclipse.pid.1=0x8003
	#baretouch_eclipse.upload.tool=arduino:avrdude
	baretouch_eclipse.upload.tool=avrdude
	baretouch_eclipse.upload.protocol=avr109
	baretouch_eclipse.upload.maximum_size=28672
	baretouch_eclipse.upload.maximum_data_size=2560
	baretouch_eclipse.upload.speed=57600
	baretouch_eclipse.upload.disable_flushing=true
	baretouch_eclipse.upload.use_1200bps_touch=true
	baretouch_eclipse.upload.wait_for_upload_port=true
	
	baretouch_eclipse.bootloader.tool=arduino:avrdude
	baretouch_eclipse.bootloader.low_fuses=0xff
	baretouch_eclipse.bootloader.high_fuses=0xd8
	baretouch_eclipse.bootloader.extended_fuses=0xcb
	baretouch_eclipse.bootloader.file=caterina/Caterina-BareTouch.hex
	baretouch_eclipse.bootloader.unlock_bits=0x3F
	baretouch_eclipse.bootloader.lock_bits=0x2F
	
	baretouch_eclipse.build.mcu=atmega32u4
	baretouch_eclipse.build.f_cpu=16000000L
	baretouch_eclipse.build.vid=0x2A6E
	baretouch_eclipse.build.pid=0x8003
	baretouch_eclipse.build.usb_product="Touch Board"
	baretouch_eclipse.build.usb_manufacturer="Bare Conductive"
	baretouch_eclipse.build.board=AVR_BARETOUCH
	baretouch_eclipse.build.core=arduino:arduino
	#baretouch_eclipse.build.variant=arduino:leonardo
	baretouch_eclipse.build.variant=leonardo
	baretouch_eclipse.build.extra_flags={build.usb_flags}
	
	baretouch_eclipse.variant=arduino:standard


### Working with libraries

Arduino libraries have to reside inside ~/Documents/Arduino/libraries.
If you do not want to put your Eclipse project there you have to sync the library folder with your project. A soft link did not work for me.

- Create the Arduino library, e.g. ~/Documents/Arduino/libraries/myLibrary, and use this library in your sketch.
- Create a simple Eclipse project, e.g. myLibrary, and add to a repository.
- Sync the library to project before comitting to yout repo.
  
The shell script [synclib.sh](./Basics/scripts/synclib.sh) can be used to perform the sync.
Simply configure this program as an external tool.


### Uploading

Uploading to the board does not work out of the box because of problems with the 1200bps reset.
The shell script [upload.sh](./Basics/scripts/upload.sh) can be used as a workaround.
Simply configure this program as an external tool.


## License

All files are released under creative commons license 4.0:

[Attribution-ShareAlike CC BY-SA](http://creativecommons.org/licenses/by-sa/4.0/)

The software is furnished "as is", without technical support, and with no warranty, express or implied, as to its usefulness for any purpose.
