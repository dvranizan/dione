SDL_FLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_ttf
GCC_FLAGS = -g `pkg-config --cflags glib-2.0`
LINKER_FLAGS = -lm `pkg-config --libs glib-2.0` 
FILES = globals.c font.c logging.c fps.c world.c main.c objects.c draw.c loader.c build.c update.c camera.c kernel.c listen.c console.c bezier.c 

all:
	gcc $(GCC_FLAGS) $(SDL_FLAGS) $(FILES) -o main $(LINKER_FLAGS) $(SDL_LIBS) 

clean:
	rm -rf *.o main
