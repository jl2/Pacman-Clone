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

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <png.h>

#include "pacman.h"
#include "ui.h"

void handle_sdl_err(const char *msg);
void gl_initialize();
void repaint(game_state *gs);

static image_t img;
static unsigned int textName;
static int skipJoystick = 0;

static int wavChan;

SDL_Joystick *stick;
Mix_Chunk *wav1;
/* sound_t wav1; */

void handle_sdl_err(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
    exit(1);
}

void gl_initialize() {
    read_png("images/pacman_frame01.png", &img);

    glClearColor( 0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glPolygonMode(GL_FRONT, GL_FILL);

    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,800.0, 0.0,600.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glGenTextures(1, &textName);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, textName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    /* glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.imgData); */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.imgData);
}

void repaint(game_state *gs) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
  glEnable(GL_TEXTURE_2D);
  
  /* glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); */
      
  glBindTexture(GL_TEXTURE_2D, textName);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);;

      glTranslatef(gs->pac.xPos - BOX_SIZE,
                 gs->pac.yPos - BOX_SIZE,
                 0.0);

    glBegin(GL_QUADS); {
        glColor4f(1.0,1.0,1.0,1.0);
    /* glEnable(GL_TEXTURE_2D); */
    /* glBindTexture(GL_TEXTURE_2D, textName); */
    /* glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); */

        glTexCoord2f(0.0, 0.0); glVertex2f(0.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex2f(BOX_SIZE, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex2f(BOX_SIZE, BOX_SIZE);
        glTexCoord2f(0.0, 1.0); glVertex2f(0.0, BOX_SIZE);

    } glEnd();

    glFlush();
    glDisable(GL_TEXTURE_2D);

    glLoadIdentity();
    SDL_GL_SwapBuffers();
}

void ui_play_hitit() {
    if (wavChan>0)
        Mix_HaltChannel(wavChan);
    wavChan = Mix_PlayChannel(-1, wav1, 1);
}

void ui_initialize() {
    int res;

    res = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK);

    if (res < 0) handle_sdl_err("Couldn't initialize SDL");

    skipJoystick = 0;
    if (SDL_NumJoysticks() < 1) {
        skipJoystick = 1;
    }

    atexit(SDL_Quit);

    SDL_Surface *screen = SDL_SetVideoMode(800, 600, 24, SDL_OPENGL|SDL_FULLSCREEN);
    if (screen == NULL) handle_sdl_err("Couldn't set 800x600x24 video mode");
    stick = SDL_JoystickOpen( 0 );

    gl_initialize();

    Mix_OpenAudio(44100, AUDIO_S16, 2, 4096);
    
    /* SDL_AudioSpec *tmp = SDL_LoadWAV("sfx/hitit.wav", &(wav1.wavSpec), &(wav1.data), &(wav1.length)); */
    wav1 = Mix_LoadWAV("sfx/hitit.wav");

    /* if (tmp == NULL) handle_sdl_err("Couldn't open \"sfx/hitit.wav\""); */
}

void ui_cleanup() {
    free_png(&img);
    Mix_CloseAudio();
    SDL_JoystickClose( stick );
}

int ui_event_loop(game_state *gs) {
    SDL_Event event;
    int retVal = 1;

    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym==SDLK_ESCAPE) {
            retVal = 0;

        } else if (event.key.keysym.sym==SDLK_LEFT) {
            gs->keys[KEY_LEFT] = 1;

        } else if (event.key.keysym.sym==SDLK_RIGHT) {
            gs->keys[KEY_RIGHT] = 1;

        } else if (event.key.keysym.sym==SDLK_DOWN) {
            gs->keys[KEY_DOWN] = 1;

        } else if (event.key.keysym.sym==SDLK_UP) {
            gs->keys[KEY_UP] = 1;
        }
        break;

    case SDL_KEYUP:
        if (event.key.keysym.sym==SDLK_LEFT) {
            gs->keys[KEY_LEFT] = 0;
        } else if (event.key.keysym.sym==SDLK_RIGHT) {
            gs->keys[KEY_RIGHT] = 0;
        } else if (event.key.keysym.sym==SDLK_DOWN) {
            gs->keys[KEY_DOWN] = 0;
        } else if (event.key.keysym.sym==SDLK_UP) {
            gs->keys[KEY_UP] = 0;
        }
        break;

    case SDL_JOYAXISMOTION:
        /* printf("Got %d %d %d %d\n", */
        /*        ++joyEvents, */
        /*        event.jaxis.which, */
        /*        event.jaxis.axis, */
        /*        event.jaxis.value); */

        if (event.jaxis.which == 0) {
            if (event.jaxis.axis == 0) {
                if (event.jaxis.value>(32768/4)) {
                    gs->keys[KEY_RIGHT] = 1;
                    gs->keys[KEY_LEFT] = 0;
                } else if (event.jaxis.value<(-32768/4)){
                    gs->keys[KEY_RIGHT] = 0;
                    gs->keys[KEY_LEFT] = 1;
                } else {
                    gs->keys[KEY_RIGHT] = 0;
                    gs->keys[KEY_LEFT] = 0;
                }
            } else if (event.jaxis.axis == 1) {
                if (event.jaxis.value>(32768/4)) {
                    gs->keys[KEY_UP] = 1;
                    gs->keys[KEY_DOWN] = 0;
                } else if (event.jaxis.value<(-32768/4)){
                    gs->keys[KEY_UP] = 0;
                    gs->keys[KEY_DOWN] = 1;
                } else {
                    gs->keys[KEY_UP] = 0;
                    gs->keys[KEY_DOWN] = 0;
                }
            }
        }
        break;
        
    case SDL_QUIT:
        retVal = 0;
        break;
    default:
        break;
    }
    repaint(gs);
    SDL_Delay(25);
    
    return retVal;
}


int read_png(const char* fname, image_t *img) {
    FILE *fp = fopen(fname, "rb");
    if (!fp) return 1;
    png_byte sig[8];

    int retVal = 0;

    fread(sig, 1, 8, fp);
    if (png_sig_cmp(sig, 0, 8)) {
        return 1;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 1;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 1;
    }
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 1;
    }
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);


    png_read_info(png_ptr, info_ptr);

    img->width = png_get_image_width(png_ptr, info_ptr);
    img->height = png_get_image_height(png_ptr, info_ptr);
    img->bitDepth = png_get_bit_depth(png_ptr, info_ptr);
    img->channels = png_get_channels(png_ptr, info_ptr);
    
    if ((img->channels != 4) || (img->bitDepth!=8)) {
        retVal = 1;
        goto cleanup;
    }

    int rowBytes = png_get_rowbytes(png_ptr, info_ptr);

    img->imgData = malloc(img->height*img->width*img->channels);

    png_bytep *row_pointers = png_malloc(png_ptr, img->height*sizeof(png_bytep));


    for (int i=0; i< img->height; ++i) {
        row_pointers[img->height-i-1] = img->imgData + i*rowBytes;
    }
    png_read_image(png_ptr, row_pointers);

    /* png_set_rows(png_ptr, info_ptr, row_pointers); */
    /* png_get_rows(png_ptr, info_ptr); */
  
    png_free(png_ptr, row_pointers);

    cleanup:
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return retVal;
}

void free_png(image_t *img) {
    free(img->imgData);
    img->width = img->height = img->bitDepth = img->channels = 0;
}
