#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include "bezier.h"

static SDL_Point multiplyVector(SDL_Point p, float u) {
	SDL_Point temp = { p.x * u, p.y * u };
	return temp;
}

static SDL_Point addVector(SDL_Point p, SDL_Point p1) {
	SDL_Point temp = { p.x + p1.x, p.y + p1.y };
	return temp;
}

SDL_Point calculateBezierPoint(float t, SDL_Point p0, SDL_Point p1, SDL_Point p2, SDL_Point p3) {
	SDL_Point ret = { 0, 0 };
	float u = 1 - t;
	float tt = t*t;
	float uu = u*u;
	float uuu = uu * u;
	float ttt = tt * t;

	SDL_Point p = multiplyVector(p0, uuu); //first term
	p = addVector(p, multiplyVector(p1, 3 * uu * t)); //second term
	p = addVector(p, multiplyVector(p2, 3 * u * tt)); //third term
	p = addVector(p, multiplyVector(p3, ttt)); //fourth term

	return p;
}