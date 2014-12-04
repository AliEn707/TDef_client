#include <stdlib.h>
#include <SDL2/SDL.h>
// *** IF USING XCODE ON MACOS X, CHANGE THE FOLLOWING LINE TO:  #include "SDL_mixer/SDL_mixer.h"
#include <SDL2/SDL_mixer.h>

int musicPlaying;

void musicFinished() { musicPlaying = 0; }

#undef main
int main(int argc, char *argv[])
{
	SDL_Surface *screen;			//Pointer to the main screen surface
	Mix_Music *music = NULL;		//Pointer to our sound, in memory
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
	
	//Load our Ogg file from disk
	music = Mix_LoadMUS("music.ogg"); 
	
	if(music == NULL) { 
		printf("Unable to load Ogg file: %s\n", Mix_GetError()); 
		return 1; 
		}
	
	//Set the video mode to anything, just need a window
//	screen = SDL_SetVideoMode(320, 240, 0, SDL_HWSURFACE|SDL_DOUBLEBUF);
//	if (screen == NULL) {
//		printf("Unable to set video mode: %s\n", SDL_GetError());
//		return 1;
//	}
	
	//Play our sound file, and capture the channel on which it is played
	musicPlaying = 1; 
	if(Mix_PlayMusic(music, 0) == -1) { 
		printf("Unable to play Ogg file: %s\n", Mix_GetError()); 
		return 1; 
	}
	Mix_HookMusicFinished(musicFinished);
	//Wait until the sound has stopped playing
	while(musicPlaying) { 
		//Do nothing for a bit 
		SDL_Delay(100); 
	}
	
	//Release the memory allocated to our sound
	Mix_HaltMusic(); 
	Mix_FreeMusic(music);
	
	//Need to make sure that SDL_mixer and SDL have a chance to clean up
	Mix_CloseAudio();
	SDL_Quit();	
	
	//Return success!
	return 0;
}
