#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  // Variables
  int running = 1;
  SDL_Event key;

  // Flags
  int opt;
  while ((opt = getopt(argc, argv, "vh")) != -1) {
    switch (opt) {
    case 'v':
      printf("Version 1.0.2 Alpha\n");
      return EXIT_SUCCESS;
    case 'h':
      printf("Usage: \n");
      printf("[-v] Shows current version\n");
      printf("[-h] Displays help menu and exits\n");
      return EXIT_SUCCESS;
    default:
      fprintf(stderr, "Invalid Option\n Usage: %s [-h] [-v]\n", argv[0]);
      return EXIT_FAILURE;
    }
  }

  SDL_Window *window = SDL_CreateWindow("Atulo", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 200, 100, 0);

  if (!window) {
    printf("SDL_ERROR: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (optind >= argc) {
    fprintf(stderr, "no input file provided\n");
    return EXIT_FAILURE;
  }

  char *filename = argv[optind];
  char *format = strrchr(filename, '.');

  // Checking if the file is wav or not (Will be removed later)
  if (strcmp(format, ".wav") != 0) {
    fprintf(stderr, "File have to be WAV format\n");
    return 1;
  }

  // Error Handle
  if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
    printf("SDL_ERROR: %s, try again\n", SDL_GetError());
  }

  // initializing an audio specification
  SDL_AudioSpec wav_spec;
  Uint8 *wav_buffer;
  Uint32 wav_length;

  // Error Handle
  if (SDL_LoadWAV(filename, &wav_spec, &wav_buffer, &wav_length) == NULL) {
    printf("SDL_ERROR: %s, try again\n", SDL_GetError());
  }

  // Error Handle
  SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
  if (deviceId == 0) {
    printf("SDL_ERROR: %s, try again\n", SDL_GetError());
  }

  SDL_QueueAudio(deviceId, wav_buffer, wav_length);

  SDL_PauseAudioDevice(deviceId, 0);
  printf("Press Enter to exit...\n");
  while (running) {
    while (SDL_PollEvent(&key)) {
      if (key.type == SDL_KEYDOWN) {
        if (key.key.keysym.sym == SDLK_RETURN) {
          printf("Quiting... \n");
          running = 0;
        }
      }
    }
    SDL_Delay(10);
  }

  SDL_CloseAudioDevice(deviceId);
  SDL_FreeWAV(wav_buffer);
  SDL_Quit();

  return EXIT_SUCCESS;
}
