#/bin/bash
#
#
#
# Call from Eclipse with three parameters:
# $1: Path to Arduino documents folder, e.g. /path/to/Documents/Arduino
# $2: Project location, e.g. ${project_loc} 
# $3: Project name, e.g. ${project_name}
#

ARDUINOLIB=$1/libraries/$3
PROJECTLOC=$2 

if [ ! -d $ARDUINOLIB ]; then
	echo "Error: $ARDUINOLIB not found!"
    exit 3
fi


if [ ! -d $PROJECTLOC ]; then
	echo "Error: $PROJECTLOC not found!"
    exit 3
fi

# Sync the files
rsync -v -r $ARDUINOLIB/ $PROJECTLOC


