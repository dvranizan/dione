#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include <stdbool.h>

#include "camera.h"
#include "globals.h"

/* the 'cameraLocation' is not pixel units but world units. */
static SDL_Rect cameraLocation;
static double cameraZoom;

/* init the camera to show the entire game world 
   zoom is the ratio of SCREEN_WIDTH to cameraLocation.w,
   it is used to calc box zooms in cameraTranslateLoc
*/
void cameraInit() {
	cameraZoom = 1; /* this is default */
	cameraLocation.x = 0;
	cameraLocation.y = 0;
	cameraLocation.h = SCREEN_HEIGHT;
	cameraLocation.w = SCREEN_WIDTH;
}

/* this function assumes worldLoc is ENTIRELY CONTAINED WITHIN cameraLocation.
   This is guarenteed by SDL_IntersectRect() */
static void cameraTranslateLoc(SDL_Rect* worldLoc) {
	worldLoc->x = (worldLoc->x - cameraLocation.x) * cameraZoom;
	worldLoc->y = (worldLoc->y - cameraLocation.y) * cameraZoom;
	worldLoc->h = (worldLoc->h) * cameraZoom;
	worldLoc->w = (worldLoc->w) * cameraZoom;
}

SDL_bool cameraGetLoc(SDL_Rect* objLoc, SDL_Rect* worldIntersect) {
	/* determine if it is in the shot */
	if (SDL_IntersectRect(objLoc, &cameraLocation, worldIntersect) == SDL_FALSE) {
		/* no need to translate to pixels, just return */
		return SDL_FALSE;
	}
	/* worldIntersect is currently in world units.  Translate to drawable units */
	cameraTranslateLoc(worldIntersect);

	return SDL_TRUE;
}
