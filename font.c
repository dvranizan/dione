#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include "SDL_ttf.h"
#include <assert.h>

#include "globals.h"

static TTF_Font *global_font = NULL;

int init_font() {
	TTF_Init();
#ifdef __GNUC__
	global_font = TTF_OpenFont("fonts/FreeMono.ttf", 24);
#else
	global_font = TTF_OpenFont("C:\\Users\\david\\Source\\Repos\\dione\\fonts\\FreeMono.ttf", 24);
#endif
	assert(global_font);
}

SDL_Texture *render_font(char *string, SDL_Color color) {
	SDL_Surface *surf = TTF_RenderText_Solid(global_font, string, color);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(global_renderer, surf);

	SDL_FreeSurface(surf);
	
	return tex;
}

/*
  returns max height of loaded font
*/
int fontGetCharHeight() {
	assert(global_font);
	return TTF_FontHeight(global_font);
}

int fontGetCharWidth() {
	assert(0); //TODO
}

void fontGetStrSize(const char *string, int *w, int *h) {
	assert(global_font);
	TTF_SizeText(global_font, string, w, h);
}
