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

static void drawWave(waveObject* obj, SDL_Rect *drawable) {
	/* TODO: THIS DOES NOT HANDLE ZOOM YET */
	SDL_SetRenderDrawColor(global_renderer, obj->color.r, obj->color.g, obj->color.b, 255);
	SDL_RenderDrawPoints(global_renderer, obj->line, SCREEN_WIDTH);
}

static void drawPerson(humanObject* obj, SDL_Rect *drawable) {
	SDL_SetRenderDrawColor(global_renderer, obj->color.r, obj->color.g, obj->color.b, 255);
	SDL_RenderDrawRect(global_renderer, drawable);
}

static void drawBetweenPoints(SDL_Point* p1, SDL_Point* p2, SDL_Color c, int w) {
	double divDiff;
	bool yLonger = false;
	int incrementVal;
	int shortLen;
	int longLen;

	if (!p1 || !p2) {
		return;
	}

	shortLen = p2->y - p1->y;
	longLen = p2->x - p1->x;

	//determine long
	if (abs(shortLen)>abs(longLen)) {
		int swap = shortLen;
		shortLen = longLen;
		longLen = swap;
		yLonger = true;
	}

	if (longLen<0) incrementVal = -1;
	else incrementVal = 1;

	if (shortLen == 0) divDiff = longLen;
	else divDiff = (double)longLen / (double)shortLen;

	//ok time to draw
	SDL_SetRenderDrawColor(global_renderer, c.r, c.g, c.b, 255);
	if (yLonger) {
		for (int i = 0; i != longLen; i += incrementVal) {
			SDL_RenderDrawPoint(global_renderer, p1->x + (int)((double)i / divDiff), p1->y + i);
		}
	}
	else {
		for (int i = 0; i != longLen; i += incrementVal) {
			SDL_RenderDrawPoint(global_renderer, p1->x + i, p1->y + (int)((double)i / divDiff));
		}
	}
}

static void drawLine(lineObject* l, SDL_Rect *drawable) {
	SDL_Point p1 = { l->x1, l->y1 };
	SDL_Point p2 = { l->x2, l->y2 };
	drawBetweenPoints(&p1, &p2, l->color, l->w);
}

static void drawPoly(polyObject* obj, SDL_Rect *drawable) {
	GList* l;
	SDL_Point *p1 = NULL;
	SDL_Point *p2 = NULL;

	if (g_list_length(obj->pointList) < 2) {
		/* nothing to draw */
		return;
	}

	for (l = obj->pointList; l != NULL; l = l->next) {
		/* shift points and draw */
		p1 = p2;
		p2 = (SDL_Point*)(l->data);
		drawBetweenPoints(p1, p2, obj->color, 1);
	}
	/* connect final point */
	p1 = (SDL_Point*)(obj->pointList->data);
	drawBetweenPoints(p1, p2, obj->color, 1);
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
	case OBJ_LINE:
		drawLine((lineObject*)obj, &drawable);
		break;
	case OBJ_PEOPLE:
		drawPerson((humanObject*)obj, &drawable);
		break;
	case OBJ_WAVE:
		drawWave((waveObject*)obj, &drawable);
		break;
	case OBJ_CUSTOM:
		(((customObject*)obj)->drawFunc)(obj);
		break;
	case OBJ_POLY:
		drawPoly((polyObject*)obj, &drawable);
		break;
	default:
		/* I never want to be here */
		print_message(MSG_VERBOSE_ERROR, "[draw] I have no idea how to handle this!", MSG_FLAG_NONE);
	}
}

