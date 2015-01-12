#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include <assert.h>

#include "fps.h"
#include "build.h"
#include "globals.h"
#include "logging.h"
#include "objects.h"
#include "font.h"
#include "windower.h"

static int fps_hidden = 1;
static textObject *fps_txt = NULL;
static SDL_Rect fps_loc;
static double fps_current_frame_ticks;
static char *fps_string;
static int fps_window_id;
SDL_Color white = {255,255,255};

void fps_break() {
	/* calculate how long that frame took and store time passed in globals */
	int current_ticks = SDL_GetTicks();
	int tick_wait = (current_ticks - fps_last_ticks) < fps_ticks_per_frame ? 
		(fps_ticks_per_frame - (current_ticks - fps_last_ticks)) : 0;
	/* block for 60fps*/
	SDL_Delay(tick_wait);
	/* store total frame ticks for FPS display */
	fps_current_frame_ticks = (current_ticks - fps_last_ticks) + tick_wait;
	fps_last_ticks = current_ticks;
	update_fps(NULL);
	return;
}

int fps_get_ticks() {
	/* for now just return SDL ticks.
	   More logic re: game time will probably need to be put here */
	return SDL_GetTicks();
}

void register_fps() {
	assert(!fps_txt);
	fps_txt = buildText(white, &fps_string);
	/* TODO - WINDOW MANAGER */
	fps_loc.x = SCREEN_WIDTH - 40;
	fps_loc.y = 0;
	fps_loc.h = 20;
	fps_loc.w = 20;
	fps_string = malloc(sizeof(char)*8); //"[   ] fps"
	sprintf(fps_string,"    fps");
}

void toggle_fps() {
	assert(fps_txt);
	fps_hidden = (fps_hidden) ? 0 : !fps_hidden;

	if (!fps_hidden) {
		/* time to draw, register with kernel */
		//create window
		fps_window_id = windower_create_window(fps_loc);
		//add string
		kernel_register_object((dioneObject*)fps_txt, fps_window_id, INPUT_CAPTURE_NONE);
	} else {
		/* user turned it off, nuke window */
		(void)kernel_remove_window(fps_window_id);
	}
}

void update_fps(dioneObject *obj) {
	snprintf(fps_string, 3, "%d", (int)((1.0/fps_current_frame_ticks)*1000));
}

void listen_fps(dioneObject *obj, void *data) {
	/* STUB, no input for fps window */
}


void draw_fps(dioneObject *obj) {
}
