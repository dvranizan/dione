SDL_FLAGS = $(shell sdl2-config --cflags)
SDL_LIBS = $(shell sdl2-config --libs) -lSDL2_ttf
GCC_FLAGS = -g 
LINKER_FLAGS = -lm  
FILES = globals.c font.c logging.c fps.c world.c main.c objects.c draw.c loader.c build.c update.c camera.c 

all:
	gcc $(GCC_FLAGS) $(SDL_FLAGS) $(FILES) -o main $(LINKER_FLAGS) $(SDL_LIBS) 

clean:
	rm -rf *.o main
