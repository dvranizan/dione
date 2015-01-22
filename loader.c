#include "build.h"
#include "world.h"
#include "kernel.h"

void loadLevel_1() {
	int x;
	SDL_Color blue = { 0, 0, 255 };
	SDL_Color red = { 255, 0, 0 };
	waveObject *obj;
	humanObject *player;
	lineObject *line;
	polyObject *poly;
	bezierObject *beizer;
	GList *polylist = NULL;
	SDL_Point *pt = NULL;
	SDL_Point p0, p1, p2, p3;
	textObject *txt;
	textObject *txt2;
	textObject *txt3;
	static char *test_txt;
	char * str = "This is a text string that will require some pretty intense word wrapping and I hope it works well.";

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
	poly = buildPoly(red, 0, polylist);
	kernel_register_object_simple((dioneObject*)poly);

	p0.x = 45;  p0.y = 45;
	p3.x = 700; p3.y = 700;
	p1.x = 1000; p1.y = 100;
	p2.x = 300; p2.y = 400;
	beizer = buildBezier(blue, p0, p1, p2, p3);
	kernel_register_object_simple((dioneObject*)beizer);

	test_txt = malloc(sizeof(char) * 5);
	sprintf(test_txt, "test");
	txt = buildText(blue, &test_txt);
	txt2 = buildText(red, &test_txt);
	txt2->ob_base.l.x = 400;
	txt2->ob_base.l.y = 400;
	txt3 = buildText(blue, &test_txt);
	txt3->ob_base.l.x = 200;
	txt3->ob_base.l.y = 200;
	kernel_register_object_simple((dioneObject*)txt);
	kernel_register_object_simple((dioneObject*)txt2);
	kernel_register_object_simple((dioneObject*)txt3);
	/* player is almost always last so it is on top */
	player = buildHuman();
	kernel_register_object_simple((dioneObject*)player);
}
