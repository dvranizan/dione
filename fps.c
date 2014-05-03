#include "SDL2/SDL.h"

#include "globals.h"
#include "logging.h"

void fps_break() {
	/* calculate how long that frame took and store time passed in globals */
	int current_ticks = SDL_GetTicks();
	int tick_wait = (current_ticks - fps_last_ticks) < fps_ticks_per_frame ? 
		(fps_ticks_per_frame - (current_ticks - fps_last_ticks)) : 0;
	/* block for 60fps*/
	SDL_Delay(tick_wait);
	fps_last_ticks = current_ticks;
	return;
}

int fps_get_ticks() {
	/* for now just return SDL ticks.
	   More logic re: game time will probably need to be put here */
	return SDL_GetTicks();
}
