/**
 * @file 	AlbumPlayer.h
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

typedef void (*TrackChangeCallbackFunction) (int album, int track);
#define NO_ALBUM -1

class AlbumPlayer {
public:
	void begin(SdFat *pSd, SFEMP3Shield *pMP3player, TrackChangeCallbackFunction trackChangeCallback);
	void process();
	void start(byte quartet, byte album, byte trackCount[4]);
	void stop();
	void nextTrack();
	void prevTrack();
	void setPlayAll(bool on);
	void setRandom(bool on);
	void setRepeat(bool on);
	bool setPause(bool on);

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
	bool trackStarted;

	byte initialAlbum;
	byte initalTrackCount[4];

	TrackChangeCallbackFunction trackChangeCallback;

	void populatePlaylist(byte album, byte trackCount[4]);
	void addAlbum(byte album, byte trackCount);
	void play(byte track);

};

#endif /* ALBUMPLAYER_H_ */
