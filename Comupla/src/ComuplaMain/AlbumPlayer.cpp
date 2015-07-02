/*
 * AlbumPlayer.cpp
 *
 *  Created on: 06.06.2015
 *      Author: holger.nauen
 */

#include <ComuplaWiring.h>
#include "AlbumPlayer.h"

void AlbumPlayer::begin(SdFat* _pSd, SFEMP3Shield* _pMP3player, AlbumChangeCallbackFunction _albumChangeCallback) {
	pSd = _pSd;
	pMP3player = _pMP3player;
	albumChangeCallback = _albumChangeCallback;

	playlistEntries = 0;
	isRandom = false;
	isPlayAll = false;
	isRepeat = false;
	trackStarted = false;
	currentTrack = 0;
	initialAlbum=0;

	randomSeed(millis());
}

void AlbumPlayer::process() {
	if (trackStarted && !pMP3player->isPlaying()){
		trackStarted = false;
		nextTrack();
	}
}

void AlbumPlayer::start(byte quartet, byte album, byte trackCount[4]) {
	currentQuartet = quartet;
	initialAlbum = album;
	memcpy(initalTrackCount,trackCount,4);

	sprintf(buffer,"q=%d a=%d c=%d,%d,%d,%d",quartet,album,trackCount[0],trackCount[1],trackCount[2],trackCount[3]);
	Serial.println(buffer);


	populatePlaylist(album, trackCount);
	play(0);
}

void AlbumPlayer::stop() {
	trackStarted = false;
	pMP3player->stopTrack();
	albumChangeCallback(NO_ALBUM);
}

void AlbumPlayer::nextTrack() {
	if (currentTrack+1==playlistEntries){
		if (isRepeat){
			start(currentQuartet, initialAlbum, initalTrackCount);
		} else {
			stop();
		}
	} else {
		play(currentTrack+1);
	}

}

void AlbumPlayer::prevTrack() {
	if (currentTrack==0 || pMP3player->currentPosition()>5000){
		play(currentTrack);
	} else {
		play(currentTrack-1);
	}
}

void AlbumPlayer::play(byte track) {
	char directoryName[4];
	char fileName[7];

	pMP3player->stopTrack();
	pSd->chdir("/");
	sprintf(directoryName, "%02d%1d", currentQuartet+1, playlist[track].album+1);

	if (pSd->chdir(directoryName)) {
		sprintf(fileName, "%02d.mp3", playlist[track].track+1);
		pMP3player->playMP3(fileName,0);

		Serial.print(directoryName);
		Serial.print("/");
		Serial.println(fileName);

		currentTrack = track;
		trackStarted = true;
		albumChangeCallback(playlist[track].album);
	}
}


void AlbumPlayer::setPlayAll(boolean on) {
	this->isPlayAll = on;
	//@todo: Rebuild playlist if playing
}

void AlbumPlayer::setRandom(boolean on) {
	this->isRandom = on;
	//@todo: Rebuild playlist if playing
}

void AlbumPlayer::setRepeat(boolean on) {
	this->isRepeat = on;
}

void AlbumPlayer::populatePlaylist(byte album, byte trackCount[4]) {
	playlistEntries = 0;
	addAlbum(album,trackCount[album]);

	if (isPlayAll){
		for (int i=album; i<4; i++){
			addAlbum(i,trackCount[i]);
		}
		for (int i=0; i<album; i++){
			addAlbum(i,trackCount[i]);
		}
	}

	playlist_entry_t entry;
	int pos;

	if (isRandom) {
		for (int i=0; i<playlistEntries; i++){
			pos = random (0,playlistEntries);
			entry = playlist[pos];
			playlist[pos] = playlist[i];
			playlist[i] = entry;
		}
	}

}

void AlbumPlayer::addAlbum(byte album, byte trackCount) {
	for (int i=0; i<trackCount && playlistEntries<=PLAYLIST_SIZE; i++){
		playlist[playlistEntries].album = album;
		playlist[playlistEntries].track = i;
		playlistEntries++;
	}
}

