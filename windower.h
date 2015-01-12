#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "objects.h"

#ifndef _WINDOWER_H
#define _WINDOWER_H

typedef enum _WINDOW_STACK_TYPE {
	WINDOW_STACK_DEFAULT,
	WINDOW_STACK_ON_TOP,
	WINDOW_STACK_ON_BOT
} WINDOW_STACK_TYPE;

typedef enum _WINDOW_SPAN_TYPE {
	WINDOW_SPAN_DEFAULT
} WINDOW_SPAN_TYPE;

extern void init_windower();

extern int windower_create_window(SDL_Rect loc);

/* create new window inside (accumulate offsets) current window */
extern int windower_push_window(SDL_Rect size, WINDOW_STACK_TYPE winType, WINDOW_SPAN_TYPE winSpan);
extern dioneWindow* windower_find_window(int win_id);
/* traverse to parent window, do not delete */
extern int windower_pop_window();

extern GSList* windower_get_windows();

extern SDL_bool windower_update_windows();
#endif
