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

#ifndef PACMAN_PACMAN_H
#define PACMAN_PACMAN_H

#define KEY_UP (0)
#define KEY_DOWN (1)
#define KEY_LEFT (2)
#define KEY_RIGHT (3)
#define NUM_KEYS (4)

#define BOARD_WIDTH (32)
#define BOARD_HEIGHT (36)

enum block_t { EMPTY,
               LEFT_WALL,
               RIGHT_WALL,
               TOP_WALL,
               BOTTOM_WALL,
               BR_OPEN,
               BL_OPEN,
               TR_OPEN,
               TL_OPEN,
               TOP_LEFT,
               TOP_RIGHT,
               BOT_LEFT,
               BOT_RIGT,
               FILLED,
               DOT,
               PILL,
};

enum ghost_color_t {
    RED,
    BLUE,
    PINK,
    ORANGE,
};

enum direction_t {
    UP,
    LEFT,
    RIGHT,
    DOWN,
};

typedef struct {
    int xPos;
    int yPos;
    enum direction_t dir;
    enum ghost_color_t col;
    int anim_frame;
} ghost_t;

typedef struct {
    int xPos;
    int yPos;
    enum direction_t dir;
    int anim_frame;
} pacman_t;

typedef struct {
    int width;
    int height;
    enum block_t *blocks;
} game_level;

typedef struct {
    pacman_t pac;
    ghost_t ghosts[4];
    game_level curLevel;
    char keys[NUM_KEYS];
} game_state;

void update_state(game_state *gs);
void initialize_game(game_state *gs);

void debug_print_level(game_level *gl);

int lvl_read(const char *fname, game_level **gl);
void lvl_free(game_level **gl);

#endif
