#include "SDL2/SDL.h"
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

void registerObject(dioneObject *obj) {
	print_message(MSG_VERBOSE_NOTE, "Registering obj...", MSG_FLAG_NONE);
	renderObjects[currentObjCount++] = obj;
}

static clear_screen() {
	SDL_SetRenderDrawColor(global_renderer, 0, 0, 0, 255);
	SDL_RenderClear(global_renderer);
}

static render_objects() {
	int x;
	for (x = 0; x < currentObjCount; x++) {
		/* update phase -
		   hit every object and mutate appropriately */
		updateObject(renderObjects[x]);
		/* draw phase -
		   if objects are drawable, draw . does not affect game world */
		drawObject(renderObjects[x]);
	}
	/* the terminal is special, so i just jam it here.  I expect all the gui to go here */
	render_terminal();
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
				/* pass the key to the list of items that care about keypress */
				switch (e.key.keysym.sym) {
				case SDLK_BACKQUOTE:
				/* special case the console */
					toggle_console();
					break;
				case SDLK_w:
				case SDLK_s:
				case SDLK_a:
				case SDLK_d:
					/* player WASD bro! */
					updatePlayerDirection = e.key.keysym.sym;
				break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				/* pass the event to the list of items that care about mousepress */
				break;
			}
		}
		/* ok events have been processed, DRAW TIME */
		clear_screen();
		/* first the render phase */
		render_objects();
		/* flip that mofo */
		flip_screen();
		/* control the FPS like a mofo */
		fps_break();
	}
}
