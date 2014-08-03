#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

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

void destroyObject(dioneObject *obj) {
	if (obj) {
		free(obj);
	}
}
