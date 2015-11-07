/**
 * @file 	AlbumPlayer.cpp
 * @author 	hn [at] holgernauen [dot] de
 * @date 	06.06.2015
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

#include <ComuplaWiring.h>
#include "AlbumPlayer.h"

void AlbumPlayer::begin(SdFat* pSd, SFEMP3Shield* pMP3player, TrackChangeCallbackFunction trackChangeCallback) {
	this->pSd = pSd;
	this->pMP3player = pMP3player;
	this->trackChangeCallback = trackChangeCallback;

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

	populatePlaylist(album, trackCount);
	play(0);
}

void AlbumPlayer::stop() {
	trackStarted = false;
	pMP3player->stopTrack();
	trackChangeCallback(NO_ALBUM,0);
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
		currentTrack = track;
		trackStarted = true;
		trackChangeCallback(playlist[track].album,playlist[track].track);
	}
}


void AlbumPlayer::setPlayAll(bool on) {
	this->isPlayAll = on;
	//@todo: Rebuild playlist if playing
}

void AlbumPlayer::setRandom(bool on) {
	this->isRandom = on;
	//@todo: Rebuild playlist if playing
}

void AlbumPlayer::setRepeat(bool on) {
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

boolean AlbumPlayer::setPause(bool on) {
	if (on && pMP3player->getState() == playback){
		pMP3player->pauseMusic();
		return true;
	} else if (!on && pMP3player->getState() == paused_playback) {
		pMP3player->resumeMusic();
	}
	return false;
}
