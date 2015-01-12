#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include "objects.h"
#include "windower.h"

extern void drawObject(dioneObject *obj);
extern void draw_set_offset(SDL_Rect loc);
extern void draw_window(dioneWindow *win);
