#/bin/bash
#
# Truncate all MP3 filenames in subdirectories recursively to the first TWO digits. 
#
# Example: 
#	"./path/to/01 file name.mp3" is renamed to "./path/to/01.mp3" 
#
 
find . -type f -regex ".*/.*/[0-9][0-9][^\/]*mp3" |
while read filename
do 
	newFilename=$(echo "$filename" | sed 's/\(.*\/[0-9][0-9]\)[^\/]*mp3/\1.mp3/g')
	echo  "$filename" "${newFilename}"
    mv -n "$filename" "${newFilename}"
done


