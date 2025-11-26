#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Options Menu
  int opt;
  while ((opt = getopt(argc, argv, "vhf:")) != -1) {
    switch (opt) {
    case 'v':
      printf("Version 0.1.1 Alpha\n");
      return 0;
    case 'h':
      printf("Usage: \n");
      printf("[-v] Shows current version\n");
      printf("[-f filename] Open file in player\n");
      printf("[-h] Displays help menu and exits\n");
      return 0;
    case 'f':
      char *filename = optarg;
      char *format = strrchr(filename, '.');
      if (strcmp(format, ".wav") != 0) {
        fprintf(stderr, "File have to be WAV format\n");
        return 1;
      }

      // Audio Player
      if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Oops! Failed to initialize player, try again\n");
      }

      SDL_AudioSpec wav_spec;
      Uint8 *wav_buffer;
      Uint32 wav_length;

      if (SDL_LoadWAV(filename, &wav_spec, &wav_buffer, &wav_length) == NULL) {
        printf(
            "Oops! an error occured when trying to play the file, try again\n");
      }

      SDL_AudioDeviceID deviceId =
          SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
      if (deviceId == 0) {
        printf(
            "Oops! an error occured when trying to play the file, try again\n");
      }

      SDL_QueueAudio(deviceId, wav_buffer, wav_length);
      SDL_PauseAudioDevice(deviceId, 0);

      Uint32 delay =
          (wav_length * 1000) / (wav_spec.freq * wav_spec.channels *
                                 (wav_spec.format == AUDIO_U8 ? 1 : 2));

      SDL_Delay(delay);
      SDL_CloseAudioDevice(deviceId);
      SDL_FreeWAV(wav_buffer);
      SDL_Quit();
      return 0;
    default:
      fprintf(stderr, "Usage: %s [-v] [-h] [-f filename]\n", argv[0]);
      return 1;
    }
  }

  return EXIT_SUCCESS;
}
