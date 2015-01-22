#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "glib.h"
#include "globals.h"
#include "objects.h"

static int new_id=0;

static int get_unique_id() {
	return new_id++;
}

static void buildObject(dioneObject *obj, OBJECT_TYPE t, char *n, SDL_Rect loc, int d) {
	obj->type = t;
	obj->id = get_unique_id();
	obj->name = n ? n : "<noname>";
	obj->l = loc;
	obj->texture = NULL;
	obj->depth = d;
	obj->update_frequency = 0;
	obj->last_update = 0;
}

waveObject* buildWave(int num) {
	int line_space = SCREEN_HEIGHT / 6;
	SDL_Rect l = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	waveObject *new_obj = malloc(sizeof(waveObject));
	buildObject((dioneObject*)new_obj, OBJ_WAVE, NULL, l, 0);
	new_obj->color = SDL_COLORWHEEL_WHITE;
	new_obj->frequency = 0;
	new_obj->amplitude = 200;
	new_obj->phase = 0;
	new_obj->birth_ticks = SDL_GetTicks();
	new_obj->line = malloc(sizeof(SDL_Point)*SCREEN_WIDTH);
	new_obj->num_points = SCREEN_WIDTH;
	new_obj->baseline = line_space*(num+1);
	return new_obj;
}

humanObject* buildHuman() {
	SDL_Rect l = {50, 50, 10, 10};
	SDL_Color yellow = {255, 255, 0};
	humanObject *new_obj = malloc(sizeof(humanObject));
	buildObject((dioneObject*)new_obj, OBJ_PEOPLE, "human", l, 0);
	new_obj->color = yellow;
	new_obj->life = 100;
	new_obj->selected = SDL_FALSE;
	return new_obj;
}

customObject* buildCustom(void(*updateFunc)(dioneObject*), 
						  void(*drawFunc)(dioneObject*), 
						  void(*listenFunc)(dioneObject*, void*)) {
	SDL_Rect l = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	customObject *new_obj = malloc(sizeof(customObject));
	buildObject((dioneObject*)new_obj, OBJ_CUSTOM, "customobj", l, 0);
	new_obj->updateFunc = updateFunc;
	new_obj->drawFunc = drawFunc;
	new_obj->listenFunc = listenFunc;
	return new_obj;
}

lineObject* buildLine(SDL_Color c, int w, int x1, int y1, int x2, int y2) {
	SDL_Rect l = { x1, y1, x2, y2 };
	lineObject *line = malloc(sizeof(lineObject));
	buildObject((dioneObject*)line, OBJ_LINE, "line", l, 0);
	line->color = c;
	line->w = w;
	line->x1 = x1;
	line->x2 = x2;
	line->y1 = y1;
	line->y2 = y2;

	return line;
}

static void growRecttoFitPoint(SDL_Rect* rect, SDL_Point point) {
	SDL_Rect temp = { point.x, point.y, 0, 0 };
	if (SDL_HasIntersection(&temp, rect) == SDL_TRUE) {
		/* no need to grow */
		return;
	}
	if (point.x < rect->x) {
		rect->w += rect->x - point.x;
		rect->x = point.x;
	}
	else if (point.x >(rect->x + rect->w)) {
		rect->w = point.x - rect->x;
	}
	if (point.y < rect->y) {
		rect->h += rect->y - point.y;
		rect->y = point.y;
	}
	else if (point.y >(rect->y + rect->h)) {
		rect->h = point.y - rect->y;
	}
}

polyObject* buildPoly(SDL_Color c, int polyFill, GList* pointList) {
	SDL_Rect l = { 0, 0, 0, 0 }; //empty collision box, will grow when iterating line list
	polyObject *poly = malloc(sizeof(polyObject));
	GList *ll = pointList;

	if (pointList->data) {
		SDL_Point *firstPoint = pointList->data;
		l.x = firstPoint->x;
		l.y = firstPoint->y;
	}
	for (ll = pointList; ll != NULL; ll = ll->next) {
		SDL_Point *point = (SDL_Point*)(ll->data);
		growRecttoFitPoint(&l, *point);
	}
	buildObject((dioneObject*)poly, OBJ_POLY, "poly", l, 0);
	poly->color = c;
	poly->fill = polyFill;
	poly->pointList = pointList;

	return poly;
}

/* cubic bezier objects, vectors stored as SDL_Points */
bezierObject* buildBezier(SDL_Color c, SDL_Point p0, SDL_Point p1, SDL_Point p2, SDL_Point p3) {
	SDL_Rect l = { 0, 0, 0, 0 }; /* start empty, grow around vectors */
	bezierObject *bezier = malloc(sizeof(bezierObject));

	growRecttoFitPoint(&l, p0);
	growRecttoFitPoint(&l, p1);
	growRecttoFitPoint(&l, p2);
	growRecttoFitPoint(&l, p3);

	buildObject((dioneObject*)bezier, OBJ_BEZIER, "bezier", l, 0);
	bezier->color = c;
	bezier->p0 = p0;
	bezier->p1 = p1;
	bezier->p2 = p2;
	bezier->p3 = p3;

	return bezier;
}

textureObject* buildTexture(SDL_Color c, SDL_Texture *tex, SDL_Rect size, int num_frames, int speed) {
	/* todo: this needs to be given a path to data, not a SDL_texture */
	SDL_Rect l = { 0, 0, 0, 0};
	textureObject *texture = malloc(sizeof(textureObject));

	SDL_QueryTexture(tex, NULL, NULL, &l.w, &l.h);

	buildObject((dioneObject*)texture, OBJ_TEXTURE, "tex", l, 0);
	texture->num_frames = num_frames;
	texture->current_frame = 0;
	texture->frame_speed = speed;
	return texture;
}
	
textObject* buildText(SDL_Color c, char **t) {
	SDL_Rect l = {0, 0, 0, 0};
	textObject *text = malloc(sizeof(textObject));
	buildObject((dioneObject*)text, OBJ_TEXT, *t, l, 0);
	text->color = c;
	text->text = t;
	return text;
}

void destroyObject(dioneObject *obj) {
	if (obj) {
		free(obj);
	}
}
