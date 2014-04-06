#include "SDL2/SDL.h"
#include "objects.h"

extern loc CAMERA_OUT_OF_SHOT;

extern void cameraInit();

extern void cameraPan(double factor);

extern SDL_bool cameraGetLoc(SDL_Rect *l, SDL_Rect *worldLoc);
