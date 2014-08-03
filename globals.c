#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "globals.h"

int MAX_MESSAGES = 1000;
SDL_Renderer *global_renderer = NULL;

int fps_ticks_per_frame = 16; /* 16ms per frame is 60FPS or 1000/60 */
int fps_last_ticks = 0;

int SCREEN_WIDTH = 1440;
int SCREEN_HEIGHT = 900;

/* these are in game units. */
int WORLD_WIDTH = 1440;
int WORLD_HEIGHT = 900;
