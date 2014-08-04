#include "build.h"
#include "world.h"
#include "kernel.h"

void loadLevel_1() {
	int x;
	SDL_Color blue = { 0, 0, 255 };
	waveObject *obj;
	humanObject *player;
	lineObject *line;
	polyObject *poly;
	GList *polylist = NULL;
	SDL_Point *pt = NULL;
	/* testing out the lines */
	line = buildLine(blue, 1, 10, 10, 350, 235);
	kernel_register_object_simple((dioneObject*)line);
	/* build and register them */
	for (x = 0; x < 5; x++) {
		obj = buildWave(x);
		kernel_register_object_simple((dioneObject*)obj);
	}
	/* build a poly */
	pt = malloc(sizeof(SDL_Point));
	pt->x = 35;
	pt->y = 35;
	polylist = g_list_prepend(polylist, pt);
	pt = malloc(sizeof(SDL_Point));
	pt->x = 35;
	pt->y = 60;
	polylist = g_list_prepend(polylist, pt);
	pt = malloc(sizeof(SDL_Point));
	pt->x = 60;
	pt->y = 60;
	polylist = g_list_prepend(polylist, pt);
	pt = malloc(sizeof(SDL_Point));
	pt->x = 75;
	pt->y = 60;
	polylist = g_list_prepend(polylist, pt);
	poly = buildPoly(blue, 0, polylist);
	kernel_register_object_simple((dioneObject*)poly);
	/* player is almost always last so it is on top */
	player = buildHuman();
	kernel_register_object_simple((dioneObject*)player);
}
