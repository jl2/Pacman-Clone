/*
  Copyright (c) 2009, Jeremiah LaRocco

  Permission to use, copy, modify, and/or distribute this software for
  any purpose with or without fee is hereby granted, provided that the
  above copyright notice and this permission notice appear in all
  copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
  WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
  AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
  DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
  PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
  TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
  PERFORMANCE OF THIS SOFTWARE.
*/


#ifndef PACMAN_UI_H
#define PACMAN_UI_H
#include <SDL/SDL.h>

#define WIDTH (800)
#define HEIGHT (600)
#define BOX_SIZE (40)

typedef struct {
    int width;
    int height;
    int bitDepth;
    int channels;
    unsigned char *imgData;
} image_t;

typedef struct {
    SDL_AudioSpec wavSpec;
    unsigned int length;
    unsigned char *data;
} sound_t;

void ui_initialize();
void ui_cleanup();
int ui_event_loop(game_state *gs);

int read_png(const char* fname, image_t *img);
void free_png(image_t *img);
void ui_play_hitit();
#endif
