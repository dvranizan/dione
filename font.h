#include "SDL2/SDL.h"
#include "SDL_ttf.h"

#include "globals.h"

extern int init_font();
extern SDL_Texture *render_font(char *string, SDL_Color color);
extern int fontGetCharHeight();
extern int fontGetCharWidth();
extern int fontGetStrSize(const char *string, int *w, int *h);
