#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#ifndef _BEZIER_H
#define _BEZIER_H

extern SDL_Point calculateBezierPoint(float t, SDL_Point p0, SDL_Point p1, SDL_Point p2, SDL_Point p3);

#endif
