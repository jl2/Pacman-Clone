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
#include <assert.h>

#include <unistd.h>

#include "pacman.h"
#include "ui.h"

int test_lvl_read() {
    const char *lvl_name = "test_level1.txt";

    FILE *of = fopen(lvl_name, "w");
    if (of == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", lvl_name);
        return 1;
    }
    int expected[] = {1,1,4,7,
                      1,1,0,3,
                      2,0,0,3,
                      8,5,5,9};
    
    fprintf(of, "4 4\n");
    int k=0;
    for (int i=0; i<4; ++i) {
        for (int j=0; j<4; ++j) {
            fprintf(of, "%d ", expected[k]);
            ++k;
        }
        fprintf(of, "\n");
    }
    fclose(of);

    game_level *gl;
    int rv = lvl_read(lvl_name, &gl);
    if (rv == 0) {
        fprintf(stderr, "Could not open file \"%s\".\n", lvl_name);
        return 1;
    }
    assert(gl->width==4);
    assert(gl->height==4);

    for (int i=0; i<(sizeof(expected)/sizeof(int)); ++i) {
        assert(expected[i] == gl->blocks[i]);
    }

    lvl_free(&gl);

    unlink(lvl_name);
    return 0;
}    

int test_png_read() {
    image_t img;

    int rv = read_png("images/pacman_frame01.png", &img);

    printf("Image Info:\n\tWidth: %d\n\tHeight: %d\n\tBit Depth: %d\n\tChannels: %d\n",
           img.width, img.height, img.bitDepth, img.channels);
    
    FILE *to = fopen("testing.ppm", "w");

    fprintf(to,"P3\n%d %d\n255\n", img.width, img.height);
    int curOff = 0;
    int numOnLine = 0;
    for (int i=0;i<img.height; ++i) {
        for (int j = 0; j<img.width; ++j) {
            for (int k = 0; k<3;  ++k) {
                
                fprintf(to, "%d ",img.imgData[curOff]);
                                ++curOff;
            }
                ++curOff;
            if (numOnLine ==70) {
                fprintf(to, "\n");
                numOnLine = 0;
            }
        }
    }
    fprintf(to, "\n");
    fclose(to);
    
    free_png(&img);
    return rv;
}

int main(int argc, char *argv[]) {
    int numFailing = 0;
    
    numFailing += test_lvl_read();

    numFailing += test_png_read();

    printf("%d tests failed.\n", numFailing);
    
    return 0;
}
