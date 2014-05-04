#include "SDL2/SDL.h"
#include <assert.h>

#include "globals.h"
#include "objects.h"
#include "logging.h"
#include "camera.h"

SDL_Keycode updatePlayerDirection = SDLK_0;
static double updatePlayerLatSpeed = 10.0; /* pixels per second */

static void updateWave(waveObject *obj) {
	int x;
	dioneObject *b = (dioneObject*)obj;
	int time_diff = SDL_GetTicks() - obj->birth_ticks;
	double angular_frequency = 2 * PI * obj->frequency;
	SDL_Point *line = obj->line;

	for (x = 0; x < obj->num_points; x++) {
		line[x].y = obj->baseline + (obj->amplitude *
							  sin((angular_frequency * time_diff) + obj->phase));
		line[x].x = x;

		assert(line[x].y >= 0);
		assert(line[x].x >= 0);
	}
}

static void updateLine(waveObject *obj) {
}

static void updatePerson(humanObject *obj) {
	switch (updatePlayerDirection) {
	case SDLK_w:
		/* snap up*/
		break;
	case SDLK_s:
		/* snap down */
		break;
	case SDLK_a:
		/* moving left */
		break;
	case SDLK_d:
		/* moving right */
		break;
	}
}

/* this processes all game events and updates items appropriately */
void updateObject(dioneObject *obj) {
	switch (TYPEOF(obj)) {
	case OBJ_LINES:
		updateLine((waveObject*)obj);
		break;
	case OBJ_PEOPLE:
		updatePerson((humanObject*)obj);
		break;
	case OBJ_WAVE:
		updateWave((waveObject*)obj);
		break;
	case OBJ_CUSTOM:
		(((customObject*)obj)->updateFunc)(obj);
		break;
	default:
		/* I never want to be here */
		print_message(MSG_VERBOSE_ERROR, "[UPDATE] I have no idea how to handle this!", MSG_FLAG_NONE);

	}
}
