#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include <stdbool.h>

#include "globals.h"
#include "logging.h"
#include "font.h"
#include "loader.h"
#include "draw.h"
#include "camera.h"
#include "update.h"

static dioneObject *renderObjects[MAX_OBJECTS];

static int currentObjCount = 0;


static clear_screen() {
	SDL_SetRenderDrawColor(global_renderer, 0, 0, 0, 255);
	SDL_RenderClear(global_renderer);
}

static flip_screen() {
	SDL_RenderPresent(global_renderer);
}

int worldLoop() {
	SDL_Event e;
	bool done = false;

	/* init everything */
	init_font();
	init_message_system();
	init_kernel();

	/* setup camera */
	cameraInit();
	/*this is ultimately not how I want to do it but whatever */
	loadLevel_1();

	print_message(MSG_VERBOSE_NOTE, "Entering main loop...", MSG_FLAG_NONE);
	while (!done) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				/* user is trying to close the window, implode */
				done = true;
				break;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				/* pass the key even to kernel */
				kernel_queueKeyboardEvent(e.type, e.key.keysym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				/* pass the event to the list of items that care about mousepress */
				break;
			}
		}
		/* crank event wheel */
		kernel_exec_events();
		/* ok events have been processed, DRAW TIME */
		clear_screen();
		/* first the render phase (update, draw)*/
		kernel_handle_objects();
		/* flip that mofo */
		flip_screen();
		/* control the FPS like a mofo */
		fps_break();
	}
}
