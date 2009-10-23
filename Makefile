pacman: pacman.c ui.c main.c
	gcc -O3 -Wall -std=c99 -o pacman `pkg-config --cflags sdl gl` $^ -lm `pkg-config --libs sdl gl` -lpng -lSDL_mixer

test: pacman.c ui.c test.c
	gcc -g -Wall -std=c99 -o test `pkg-config --cflags sdl gl` $^ -lm `pkg-config --libs sdl gl` -lpng -lSDL_mixer

clean:
	rm -f test *.o

.PHONY: clean