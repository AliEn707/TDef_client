#include <stdio.h>
#include <SDL2/SDL.h>
#include "stb_vorbis.h"

#define MUS_PATH "test.wav"

// prototype for our audio callback
// see the implementation for more information
void my_audio_callback(void *userdata, Uint8 *stream, int len);

// variable declarations
static Uint8 *audio_pos; // global pointer to the audio buffer to be played
static Uint32 audio_len; // remaining length of the sample we have to play
static Uint32 audio_len2; // remaining length of the sample we have to play

/*
** PLAYING A SOUND IS MUCH MORE COMPLICATED THAN IT SHOULD BE
*/
#undef main
int main(int argc, char* argv[]){
	printf("test\n");
	// Initialize SDL.
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
			return 1;

	// local variables
	static Uint32 wav_length=0; // length of our sample
	static Uint32 channels=0; // length of our sample
	static Uint8 *wav_buffer; // buffer containing our audio file
	static SDL_AudioSpec wav_spec; // the specs of our piece of music
	int shift1,shift2;
//	file=stb_vorbis_open_filename(argv[1], NULL, NULL);
	/* Load the WAV */
	// the specs, length and buffer of our wav are filled
//	if((
		wav_length=stb_vorbis_decode_filename(argv[1], &channels, (short**)&wav_buffer);
//	)<0){
//	if( SDL_LoadWAV(argv[1], &wav_spec, &wav_buffer, &wav_length) == NULL ){
//		printf("OK\n");
//	  return 1;
//	}
	// set the callback function
	
	wav_spec.freq = 44100;
	wav_spec.format = AUDIO_S16SYS;
	wav_spec.callback = my_audio_callback;
	wav_spec.userdata = NULL;
	wav_spec.channels = channels;
	
	// set our global static variables
	audio_pos = wav_buffer;//+100; // copy sound buffer
	audio_len = wav_length*channels*2; // copy file length, why 4 dont now
	printf("audio length: %d\n",audio_len);
	printf("channels: %d\n",channels);
	/* Open the audio device */
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 ){
	  fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	  exit(-1);
	}
	
	/* Start playing */
	SDL_PauseAudio(0);

	// wait until we're don't playing
	while ( audio_len > 0 ) {
		SDL_Delay(100); 
	}
	
	// shut everything down
	SDL_CloseAudio();
	SDL_FreeWAV(wav_buffer);
	//SDL_Free(wav_buffer);

}

// audio callback function
// here you have to copy the data of your audio buffer into the
// requesting audio buffer (stream)
// you should only copy as much as the requested length (len)
void my_audio_callback(void *userdata, Uint8 *stream, int len) {
	
	if (audio_len ==0)
		return;
	
	len = ( len > audio_len ? audio_len : len );
//	printf("%d\n",len);
//	SDL_memcpy (stream, audio_pos, len); 	// simply copy from one buffer into the other
	SDL_memset(stream, 0, len);
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	
	audio_pos += len;
	audio_len -= len;
}