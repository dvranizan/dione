#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "world.h"
#include "globals.h"

int main (int argc, char** argv) {
	SDL_Window *win = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		printf("ERROR! %s\n", SDL_GetError());
		return 1;
	}

	win = SDL_CreateWindow("Dione.", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!win) {
		printf("ERROR! %s\n", SDL_GetError());
		return 1;
	}
	
	global_renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!global_renderer) {
		printf("ERROR! %s\n", SDL_GetError());
		return 1;
	}

	worldLoop();

	SDL_DestroyRenderer(global_renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
