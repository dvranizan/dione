#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

extern int MAX_MESSAGES;
extern SDL_Renderer *global_renderer;

/*for FPS*/
extern int fps_last_ticks;
extern int fps_ticks_per_frame;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern int WORLD_WIDTH;
extern int WORLD_HEIGHT;

#define SDL_COLORWHEEL_WHITE (SDL_Color){255, 255, 255}
#define SDL_COLORWHEEL_BLACK (SDL_Color){0, 0, 0} 

#define MAX_OBJECTS 1000

#define KERNEL_MAX_SELECTED_OBJECTS 1000
#define KERNEL_MAX_EVENT_QUEUE 1000

#define PI 3.14159265

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#endif
