#include <stdlib.h>
#include <SDL2/SDL.h>
// *** IF USING XCODE ON MACOS X, CHANGE THE FOLLOWING LINE TO:  #include "SDL_mixer/SDL_mixer.h"
#include <SDL2/SDL_mixer.h>

#undef main
int main(int argc, char *argv[])
{
	SDL_Surface *screen;			//Pointer to the main screen surface
	Mix_Chunk *sound = NULL;		//Pointer to our sound, in memory
	int channel;				//Channel on which our sound is played
	  
	int audio_rate = 44100; //22050;			//Frequency of audio playback
	Uint16 audio_format = MIX_DEFAULT_FORMAT; //AUDIO_S16SYS; 	//Format of the audio we're playing
	int audio_channels = 2;			//2 channels = stereo
	int audio_buffers = 4096;		//Size of the audio buffers in memory
	
	//Initialize BOTH SDL video and SDL audio
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		printf("Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	//Initialize SDL_mixer with our chosen audio settings
	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		printf("Unable to initialize audio: %s\n", Mix_GetError());
		exit(1);
	}
	
	//Load our WAV file from disk
	sound = Mix_LoadWAV("sound.wav");//"music.ogg");
	if(sound == NULL) {
		printf("Unable to load WAV file: %s\n", Mix_GetError());
	}
	
	//Set the video mode to anything, just need a window
//	screen = SDL_SetVideoMode(320, 240, 0, SDL_ANYFORMAT);
//	if (screen == NULL) {
///		printf("Unable to set video mode: %s\n", SDL_GetError());
///		return 1;
//	}
	Mix_AllocateChannels(5);
	//Play our sound file, and capture the channel on which it is played
	channel = Mix_PlayChannel(-1, sound, 0);
//	SDL_Delay(100);
//	channel = Mix_PlayChannel(-1, sound, 0);
//	SDL_Delay(100);
//	channel = Mix_PlayChannel(-1, sound, 0);
	if(channel == -1) {
		printf("Unable to play WAV file: %s\n", Mix_GetError());
	}
	
	//Wait until the sound has stopped playing
	while(Mix_Playing(channel) != 0)
		SDL_Delay(100);
	
	//Release the memory allocated to our sound
	Mix_FreeChunk(sound);
	
	//Need to make sure that SDL_mixer and SDL have a chance to clean up
	Mix_CloseAudio();
	SDL_Quit();	
	
	//Return success!
	return 0;
}
