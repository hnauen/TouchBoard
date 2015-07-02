#/bin/bash
#
# A shell script for uploading a hex file to the Touch Board.
#
# Call from Eclipse with four parameters:
# $1: Path to AVR installation, e.g. /path/to/Arduino_1.6.1.app/Contents/Resources/Java/hardware/tools/avr
# $2: COM-Port, e.g. /dev/tty.usbmodem1421 
# $3: Project location, e.g. ${project_loc} 
# $4: Project name, e.g. ${project_name}
#

DELAY=2

AVRDUDE=$1/bin/avrdude
CONFIGFILE=$1/etc/avrdude.conf
COMPORT=$2
HEXFILE=$3/Release/$4.hex

DEVICE=atmega32u4
PROGRAMMER=avr109
BAUDRATE=57600


if [ ! -f $AVRDUDE ]; then
	echo "Error: $AVRDUDE not found!"
    exit 3
fi

if [ ! -f $CONFIGFILE ]; then
	echo "Error: $CONFIGFILE not found!"
    exit 3
fi

if [ ! -e $COMPORT ]; then
	echo "Error: $COMPORT not found!"
    exit 3
fi

if [ ! -f $HEXFILE ]; then
	echo "Error: $HEXFILE not found!"
    exit 3
fi


# Reset TouchBoard
stty -f $COMPORT 1200 hupcl

# Wait for COM port to re-connect 
sleep $DELAY 

# Upload sketch 
$AVRDUDE -C $CONFIGFILE -p $DEVICE -c $PROGRAMMER -P $COMPORT -b $BAUDRATE -D -U flash:w:$HEXFILE:i

 