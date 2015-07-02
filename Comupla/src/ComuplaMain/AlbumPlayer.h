/*
 * AlbumPlayer.h
 *
 *  Created on: 06.06.2015
 *      Author: holger.nauen
 */

#ifndef ALBUMPLAYER_H_
#define ALBUMPLAYER_H_

#include <SPI.h>
#include <SdFat.h>
#include <SdFatUtil.h>
#include <SFEMP3Shield.h>

typedef struct {
	byte album;
	byte track;

} playlist_entry_t;

#define PLAYLIST_SIZE 80

typedef void (*AlbumChangeCallbackFunction) (int album);
#define NO_ALBUM -1

class AlbumPlayer {
public:
	void begin(SdFat *_pSd, SFEMP3Shield *_pMP3player, AlbumChangeCallbackFunction _albumChangeCallback);
	void process();
	void start(byte quartet, byte album, byte trackCount[4]);
	void stop();
	void nextTrack();
	void prevTrack();
	void setPlayAll(boolean on);
	void setRandom(boolean on);
	void setRepeat(boolean on);

private:
	SdFat *pSd;
	SFEMP3Shield *pMP3player;

	boolean isRandom;
	boolean isPlayAll;
	boolean isRepeat;


	playlist_entry_t playlist[PLAYLIST_SIZE];
	byte playlistEntries;
	byte currentTrack;
	byte currentQuartet;
	boolean trackStarted;

	byte initialAlbum;
	byte initalTrackCount[4];

	AlbumChangeCallbackFunction albumChangeCallback;

	void populatePlaylist(byte album, byte trackCount[4]);
	void addAlbum(byte album, byte trackCount);
	void play(byte track);

	char buffer[80];

};

#endif /* ALBUMPLAYER_H_ */
