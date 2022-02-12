import vlc
import time
import os
import sys

class VLC:
    def __init__(self, config):
        self.Player = vlc.Instance('--loop')

    def addPlaylist(self):
        URL = os.path.join(ROOT_DIR, "videos", filename_player)
        self.mediaList = self.Player.media_list_new()
        path = r"C:\Users\dell5567\Desktop\engsong"
        songs = os.listdir(path)
        for s in songs:
            self.mediaList.add_media(self.Player.media_new(os.path.join(path,s)))
        self.listPlayer = self.Player.media_list_player_new()
        self.listPlayer.set_media_list(self.mediaList)
def durationChanged_vlc(event):
	progress = event.u.new_time/1000
	print(progress)
try:
	URL = os.path.join(ROOT_DIR, "videos", filename_player)
	Player = vlc.Instance('--loop')
	mediaList = Player.media_list_new()
	path = os.path.join(ROOT_DIR, "videos")
	songs = os.listdir(path)
	lista_de_videos = []
	for s in songs:
		if any(s[-4:] in item for item in config["VIDEO_FILES"]):
			lista_de_videos.append(s)
			mediaList.add_media(Player.media_new(os.path.join(path,s)))
	counter = 0
	for value in mediaList.__iter__():
		print("playlist", value.get_meta(0) == filename_player,  value.get_meta(0), lista_de_videos[counter])
		counter += 1
	listPlayer = Player.media_list_player_new()
	mediaPlayer = listPlayer.get_media_player()
	listPlayer.set_media_list(mediaList)
	# listPlayer.play_item_at_index(mediaList.index_of_item(filename_player))
	mediaPlayer.toggle_fullscreen()
	mediaplayer_vlc_event_manager = mediaPlayer.event_manager()
	listPlayer_vlc_event_manager = listPlayer.event_manager()
	# listPlayer.set_playback_mode(vlc.PlaybackMode.loop)
	# listPlayer_vlc_event_manager.event_attach(vlc.EventType.MediaListPlayerPlayed, playlistFinished)

	mediaplayer_vlc_event_manager.event_attach(vlc.EventType.MediaPlayerTimeChanged, durationChanged_vlc)
	# mediaplayer_vlc_event_manager.event_attach(vlc.EventType.MediaPlayerEncounteredError, printError)
	# mediaplayer_vlc_event_manager.event_attach(vlc.EventType.MediaPlayerMediaChanged, playlistPositionChanged)
	# mediaplayer_vlc_event_manager.event_attach(vlc.EventType.MediaDurationChanged, positionChanged_vlc) 
	listPlayer.play_item_at_index(0)
	print(mediaPlayer.get_length())