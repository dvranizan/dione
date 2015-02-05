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
#include "bezier.h"

static int gDrawOffset_x = 0;
static int gDrawOffset_y = 0;

static void drawWave(waveObject* obj, SDL_Rect *drawable) {

}

static void drawPerson(humanObject* obj, SDL_Rect *drawable) {

}

/* this is as low as it gets. slams pixels onto surface */
static void drawBetweenPoints(SDL_Point* p1, SDL_Point* p2, SDL_Color c, int w) {
	double divDiff;
	bool yLonger = false;
	int incrementVal;
	int shortLen;
	int longLen;
	int i;

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
		for (i = 0; i != longLen; i += incrementVal) {
			int x = p1->x + (int)((double)i / divDiff) + gDrawOffset_x;
			int y = p1->y + i + gDrawOffset_y;
			SDL_RenderDrawPoint(global_renderer, x, y);
		}
	}
	else {
		for (i = 0; i != longLen; i += incrementVal) {
			int x = p1->x + i + gDrawOffset_x;
			int y = p1->y + (int)((double)i / divDiff) + gDrawOffset_y;
			SDL_RenderDrawPoint(global_renderer, x, y);
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

/* draw bezier in line segments instead of pixles */
static void drawBezier(bezierObject *obj, SDL_Rect *drawable) {
	SDL_Point q0 = calculateBezierPoint(0, obj->p0, obj->p1, obj->p2, obj->p3);
	SDL_Point q1;
	int i;

	for (i = 1; i <= 20; i++) {
		float t = i / (float)20;
		q1 = calculateBezierPoint(t, obj->p0, obj->p1, obj->p2, obj->p3);
		drawBetweenPoints(&q0, &q1, obj->color, 1);
		q0 = q1;
	}

}

static void drawTexture(textureObject *obj, SDL_Rect *drawable) {
	SDL_Rect source = ((dioneObject*)obj)->l;
	SDL_Rect target = ((dioneObject*)obj)->l;
	/* skip to proper source frame */
	source.x = source.x * obj->current_frame;
	source.y = 0;
	/* shift for window */
	target.x += gDrawOffset_x;
	target.y += gDrawOffset_y;
	SDL_RenderCopy(global_renderer, ((dioneObject*)obj)->texture, &source, &target);
}

/* low level routine to draw texture on screen, given texture and 
   offsets, acc global offsets */
static void draw_texture(SDL_Texture *tex, const SDL_Rect* source, SDL_Rect target) {
	target.x += gDrawOffset_x;
	target.y += gDrawOffset_y;
	SDL_RenderCopy(global_renderer, tex, source, &target);
}

static void drawText(textObject *obj, SDL_Rect *drawable) {
	draw_texture(((dioneObject*)obj)->texture, NULL, ((dioneObject*)obj)->l);
}

/* set offset for window for low level draw */
void draw_set_offset(SDL_Rect loc) {
	gDrawOffset_x = loc.x;
	gDrawOffset_y = loc.y;
}

void draw_window(dioneWindow *win) {
	/* STUB - nothing to do for window yet */
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
	case OBJ_BEZIER:
		drawBezier((bezierObject*)obj, &drawable);
		break;
	case OBJ_TEXTURE:
		drawTexture((textureObject*)obj, &drawable);
		break;
	case OBJ_TEXT:
		drawText((textObject*)obj, &drawable);
		break;
	default:
		/* I never want to be here */
		print_message(MSG_VERBOSE_ERROR, MSG_FLAG_NONE, "[draw] I have no idea how to handle this obj: %x!", obj);
	}
}

