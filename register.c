#include "globals.h"
#include "objects.h"

/*
  There are (as of now) two lists of objects: game objects and hud objects.
  HUD ignores the camera and absolute pixels.
  Game objects behave w/ camera (zoom etc...)

  To be fast we make a 2D array of dioneObjects size [MAX_OBJECTS][MAX_OBJECTS].
  The first dimension is the main object, ie: terminal drop down
  The 2nd dimension is the peices that build it up, ie: txt lines of terminal, blinking cursor, etc
*/

static int currentGameObjCount = 0;
static dioneObject *renderGameObjects[MAX_OBJECTS][MAX_OBJECTS];
static int currentHUDObjCount = 0;
static dioneObject *renderHUDObjects[MAX_OBJECTS][MAX_OBJECTS];

void registerAddGameObject(dioneObject *obj) {
	assert(currentGameObjCount < MAX_OBJECTS);
	renderGameObjects[currentGameObjCount][0] = obj;
	renderGameObjects[currentGameObjCount][1] = NULL;
	currentGameObjCount += 1;
}

void registerAddHUDObject(dioneObject *obj) {
	assert(currentHUDObjCount < MAX_OBJECTS);
	renderHUDObjects[currentHUDObjCount][0] = obj;
	renderHUDObjects[currentHUDObjCount][1] = NULL;
	currentHUDObjCount += 1;
}

void registerAddToGameObject(dioneObject *mainObj, dioneObject *new) {
	/* find first index */
	int index = 0;
	int subindex = 0;
	while (index < MAX_OBJECTS) {
		if (renderGameObjects[index][0] == mainObj) {
			/* found you! */
			break;
		}
		index++;
	}
	assert(index != MAX_OBJECTS); /* asked for one that doesn't exist, freak out */
	while (renderGameObjects[index][subindex]) {
		subindex++;
	}
	assert (subindex < MAX_OBJECTS - 1);
	renderGameObjects[index][subindex] = new;
	renderGameObjects[index][subindex+1] = NULL;
}

void registerAddToHUDObject(int obj_id, dioneObject *new) {
	/* find first index */
	int index = 0;
	int subindex = 0;
	while (index < currentGameObjCount) {
		if ((renderHUDObjects[index][0])->obj_id == obj_id) {
			/* found you! */
			break;
		}
		index++;
	}
	assert(index != currentGameObjCount); /* asked for one that doesn't exist, freak out */
	while (renderHUDObjects[index][subindex]) {
		subindex++;
	}
	assert (subindex < MAX_OBJECTS - 1);
	renderHUDObjects[index][subindex] = new;
	renderHUDObjects[index][subindex+1] = NULL;
}

SDL_bool registerRemoveGameObject(int id) {
	int index = 0;
	while ((renderGameObjects[index][0])->id != id && index < currentGameObjCount) {
		index += 1;
	}
	if (index == currentGameObjCount) {
		return SDL_FALSE;
	} else {
		/* rip this badboy out */
		while (index < currentGameObjCount) {
			renderGameObjects[index][0] == NULL
