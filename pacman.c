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

#include <stdio.h>
#include <stdlib.h>

#include "pacman.h"
#include "ui.h"

#include <GL/gl.h>
#include <GL/glu.h>

static const char* images[] = {
    "images/empty.png",
    "images/left_wall.png",
    "images/right_wall.png",
    "images/top_wall.png",
    "images/bottom_wall.png",
    "images/br_open.png",
    "images/bl_open.png",
    "images/tr_open.png",
    "images/tl_open.png",
    "images/top_left.png",
    "images/top_right.png",
    "images/bot_left.png",
    "images/bot_rigt.png",
    "images/filled.png",
    "images/dot.png",
    "images/pill.png"};

int lvl_get_block(game_level *gl, int x, int y) {
    return gl->blocks[y*gl->width + x];
}

void update_state(game_state *gs) {
    if (gs->keys[KEY_UP]) {
        gs->pac.yPos += 5.0;
        if (gs->pac.yPos > HEIGHT) {
            ui_play_hitit();
            gs->pac.yPos = HEIGHT;
        }
    }
    if (gs->keys[KEY_DOWN]) {
        gs->pac.yPos -= 5.0;
        if (gs->pac.yPos < BOX_SIZE) {
            ui_play_hitit();
            gs->pac.yPos = BOX_SIZE;
        }
    }
    if (gs->keys[KEY_RIGHT]) {
        gs->pac.xPos += 5.0;
        if (gs->pac.xPos > WIDTH) {
            ui_play_hitit();
            gs->pac.xPos = WIDTH;
        }
    }
    if (gs->keys[KEY_LEFT]) {
        gs->pac.xPos -= 5.0;
        if (gs->pac.xPos < BOX_SIZE) {
            ui_play_hitit();
            gs->pac.xPos = BOX_SIZE;
        }
    }
}

int lvl_read(const char *fname, game_level **gl) {
    /* printf("%s\n", images[0]); */
    FILE *inf = fopen(fname, "r");

    if (inf == NULL) {
        return 0;
    }
    
    int width, height;
    
    fscanf(inf, "%d %d\n", &width, &height);

    *gl = malloc(sizeof(game_level));
    
    (*gl)->width = width;
    (*gl)->height = height;

    (*gl)->blocks = malloc(width*height*sizeof(enum block_t));

    int k = 0;
    enum block_t nb;
    for (int i = 0; i<height; ++i) {
        for (int j = 0; j<width; ++j) {
            fscanf(inf, "%d", (int*)&nb);
            (*gl)->blocks[k] = nb;
            ++k;
        }
    }
    fclose(inf);
    return 1;
}

void lvl_free(game_level **gl) {
    free( (*gl)->blocks );
    free(*gl);
    *gl = NULL;
}

void lvl_draw(game_level *gl, float x, float y, float w, float h) {
    int i,j,k;
    int bl;

    for (i=0; i< gl->height; ++i) {

        for (k = 0; k<=1; ++k) {

            for (j=0; j<gl->width; ++j) {
                
                bl = lvl_get_block(gl, j,i);
                
            }
        }
    }
}

void initialize_game(game_state *gs) {
    gs->pac.xPos = WIDTH / 2.0;
    gs->pac.yPos = HEIGHT / 2.0;
    gs->keys[KEY_UP] = gs->keys[KEY_DOWN] = gs->keys[KEY_LEFT] = gs->keys[KEY_RIGHT] = 0;
}
