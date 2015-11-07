#/bin/bash
#
# Albums reside inside in sub-directories with a 3-digit name:
# 2-digits: Zero padded number of Quartet: 01-16 
# 1-digit : Number of Album: 1-4
#

 for QUARTET in `seq 1 16`;
 do
 	for ALBUM in `seq 1 4`;
 	do
 		printf -v folderName "%02d%d" $QUARTET $ALBUM
 		mkdir -p $folderName
	done
done
