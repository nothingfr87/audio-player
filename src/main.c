#include "SDL/include/SDL.h"
#include "SDL/include/SDL_audio.h"
#include "SDL/include/SDL_stdinc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char *filename = argv[1];
  char *format = strrchr(filename, '.');
  if (strcmp(format, ".wav") != 0) {
    fprintf(stderr, "File have to be WAV format\n");
    return 1;
  }

  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    printf("Oops! Failed to initialize player, try again\n");
  }

  SDL_AudioSpec wav_spec;
  Uint8 *wav_buffer;
  Uint32 wav_length;

  if (SDL_LoadWAV(filename, &wav_spec, &wav_buffer, &wav_length) == NULL) {
    printf("Oops! an error occured when trying to play the file, try again\n");
  }

  SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
  if (deviceId == 0) {
    printf("Oops! an error occured when trying to play the file, try again\n");
  }

  SDL_QueueAudio(deviceId, wav_buffer, wav_length);
  SDL_PauseAudioDevice(deviceId, 0);

  Uint32 delay = (wav_length * 1000) / (wav_spec.freq * wav_spec.channels *
                                        (wav_spec.format == AUDIO_U8 ? 1 : 2));

  SDL_Delay(delay);
  SDL_CloseAudioDevice(deviceId);
  SDL_FreeWAV(wav_buffer);
  SDL_Quit();

  int opt;
  while ((opt = getopt(argc, argv, "vh")) != -1) {
    switch (opt) {
    case 'v':
      printf("Version 1.0.2 Alpha\n");
      return 0;
    case 'h':
      printf("Usage: \n");
      printf("[-v] Shows current version\n");
      printf("[-h] Displays help menu and exits\n");
      return 0;
    default:
      fprintf(stderr, "Invalid Option\n Usage: %s [-h] [-v]\n", argv[0]);
      return -1;
    }
  }
}
