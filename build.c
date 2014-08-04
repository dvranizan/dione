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

static void buildObject(dioneObject *obj, OBJECT_TYPE t, char *n, SDL_Rect loc) {
	obj->type = t;
	obj->id = get_unique_id();
	obj->name = n ? n : "<noname>";
	obj->l = loc;
	obj->texture = NULL;
}

waveObject* buildWave(int num) {
	int line_space = SCREEN_HEIGHT / 6;
	SDL_Rect l = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	waveObject *new_obj = malloc(sizeof(waveObject));
	buildObject((dioneObject*)new_obj, OBJ_WAVE, NULL, l);
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
	buildObject((dioneObject*)new_obj, OBJ_PEOPLE, "human", l);
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
	buildObject((dioneObject*)new_obj, OBJ_CUSTOM, "customobj", l);
	new_obj->updateFunc = updateFunc;
	new_obj->drawFunc = drawFunc;
	new_obj->listenFunc = listenFunc;
	return new_obj;
}

lineObject* buildLine(SDL_Color c, int w, int x1, int y1, int x2, int y2) {
	SDL_Rect l = { x1, y1, x2, y2 };
	lineObject *line = malloc(sizeof(lineObject));
	buildObject((dioneObject*)line, OBJ_LINE, "line", l);
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
	buildObject((dioneObject*)poly, OBJ_POLY, "poly", l);
	poly->color = c;
	poly->fill = polyFill;
	poly->pointList = pointList;

	return poly;
}

void destroyObject(dioneObject *obj) {
	if (obj) {
		free(obj);
	}
}
