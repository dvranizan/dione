#include "build.h"
#include "world.h"
#include "kernel.h"

void loadLevel_1() {
	int x;
	waveObject *obj;
	humanObject *player;
	/* build and register them */
	for (x = 0; x < 5; x++) {
		obj = buildWave(x);
		kernel_register_object((dioneObject*)obj);
	}
	/* player is almost always last so it is on top */
	player = buildHuman();
	kernel_register_object((dioneObject*)player);
}
