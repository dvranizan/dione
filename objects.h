#include "SDL2/SDL.h"
#include <stdbool.h>
/* see http://www.python.org/dev/peps/pep-3123/ for my rational here... */
#ifndef _OBJECTS_H
#define _OBJECTS_H

typedef enum _WINDOW_LOC {
	WINDOW_LOC_TOP_LEFT,
	WINDOW_LOC_TOP_MID,
	WINDOW_LOC_TOP_RIGHT,
	WINDOW_LOC_MID_LEFT,
	WINDOW_LOC_MID_MID,
	WINDOW_LOC_MID_RIGHT,
	WINDOW_LOC_BOT_LEFT,
	WINDOW_LOC_BOT_MID,
	WINDOW_LOC_BOT_RIGHT 
} WINDOW_LOC;

typedef enum _WINDOW_BORDER_STYLE {
	WINDOW_BORDER_NONE,
	WINDOW_BORDER_BOLD 
} WINDOW_BORDER_STYLE;

typedef enum _OBJECT_TYPE {
	OBJ_LINES,
	OBJ_PEOPLE,
	OBJ_WAVE
} OBJECT_TYPE;

/* 
   The loc that is saved in the objects is the world map location.
   Where it is drawn is calculated via the camera routines
*/

typedef struct _loc {
	int x;
	int y;
	int h;
	int w;
} loc;

typedef struct _object {
	OBJECT_TYPE type;
	int id;
	char *name;
	SDL_Rect l;
	SDL_Texture *texture;
} dioneObject;

typedef struct {
	dioneObject ob_base;
	SDL_Color color;
	int life;
	SDL_bool selected;
} humanObject;

typedef struct {
	dioneObject ob_base;
	char *text;
	WINDOW_BORDER_STYLE bstyle;
} textWindowObject;

typedef struct {
	dioneObject ob_base;
	SDL_Color color;
	double amplitude;
	double frequency;
	double phase;
	int birth_ticks;
	SDL_Point *line;
	int num_points;
	int baseline;
} waveObject;

#define TYPEOF(obj) (((dioneObject*)obj)->type)

#endif