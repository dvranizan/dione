#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include <assert.h>

#include "globals.h"
#include "objects.h"
#include "logging.h"
#include "camera.h"
#include "font.h"

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

static void updatePoly(polyObject *obj) {
}

static void updateBezier(bezierObject *obj) {
}

static void updateTexture(textureObject *obj) {
	/* TODO: advance frames */
	assert(obj->current_frame <= obj->num_frames);
	((dioneObject*)obj)->l.x = ((dioneObject*)obj)->l.w * obj->current_frame;
}

static void updateText(textObject *obj) {
	int w, h;
	dioneObject *base = (dioneObject*)obj;
	char *text = *(obj->text);
	/* nuke old surface if necessary */
	if (base->texture) {
		SDL_DestroyTexture(base->texture);
	}

	base->texture = render_font(text, obj->color);
	SDL_QueryTexture(base->texture, NULL, NULL, &w, &h);
	base->l.w = w;
	base->l.h = h;
}

/* this processes all game events and updates items appropriately */
void updateObject(dioneObject *obj) {
	if (!TEST_OBJ_NEEDS_UPDATE(obj)) {
		return;
	}

	CLEAR_OBJ_NEEDS_UPDATE(obj);

	switch (TYPEOF(obj)) {
	case OBJ_LINE:
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
	case OBJ_POLY:
		updatePoly((polyObject*)obj);
		break;
	case OBJ_BEZIER:
		updateBezier((bezierObject*)obj);
		break;
	case OBJ_TEXTURE:
		updateTexture((textureObject*)obj);
		break;
	case OBJ_TEXT:
		updateText((textObject*)obj);
		break;
	default:
		/* I never want to be here */
		print_message(MSG_VERBOSE_ERROR, MSG_FLAG_NONE, "[UPDATE] I have no idea how to handle this! ptr: %x", obj);

	}
}
