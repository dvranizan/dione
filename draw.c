#include "SDL2/SDL.h"
#include <assert.h>

#include "globals.h"
#include "objects.h"
#include "logging.h"
#include "camera.h"

static void drawWave(waveObject* obj, SDL_Rect *drawable) {
	/* TODO: THIS DOES NOT HANDLE ZOOM YET */
	SDL_SetRenderDrawColor(global_renderer, obj->color.r, obj->color.g, obj->color.b, 255);
	SDL_RenderDrawPoints(global_renderer, obj->line, SCREEN_WIDTH);
}

static void drawPerson(humanObject* obj, SDL_Rect *drawable) {
	SDL_SetRenderDrawColor(global_renderer, obj->color.r, obj->color.g, obj->color.b, 255);
	SDL_RenderDrawRect(global_renderer, drawable);
}

static void drawLine(waveObject* obj, SDL_Rect *drawable) {
	/* STUB */
}

/* this is the main pump that will call all the specific functions */
/* first we see if we even need to draw by checking if it is
   in the camera, via cameraGetLoc()
   cameraGetLoc() returns a drawable rect (if camera is zoomed it needs to translate)
   which draw can use.

   DRAW SHOULD NEVER AFFECT GAME WORLD
*/
void drawObject(dioneObject *obj) {
	SDL_Rect drawable;
	SDL_bool in_camera;

	in_camera = cameraGetLoc(&(obj->l), &drawable);
	if (in_camera == SDL_FALSE) {
		return;
	}
	/* drawable is now in pixels not world units.
	   We can just draw it like it is! */
	switch (TYPEOF(obj)) {
	case OBJ_LINES:
		drawLine((waveObject*)obj, &drawable);
		break;
	case OBJ_PEOPLE:
		drawPerson((humanObject*)obj, &drawable);
		break;
	case OBJ_WAVE:
		drawWave((waveObject*)obj, &drawable);
		break;
	default:
		/* I never want to be here */
		print_message(MSG_VERBOSE_ERROR, "I have no idea how to handle this!", MSG_FLAG_NONE);
	}
};

