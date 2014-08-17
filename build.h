#include "objects.h"
#include "glib.h"

#ifndef _BUILD_H
#define _BUILD_H
extern waveObject* buildWave(int num);
extern humanObject* buildHuman();
extern customObject* buildCustom(void(*updateFunc)(dioneObject*), 
								 void(*drawFunc)(dioneObject*), 
								 void(*listenFunc)(dioneObject*, void*));
extern lineObject* buildLine(SDL_Color c, int w, int x1, int y1, int x2, int y2);
extern polyObject* buildPoly(SDL_Color c, int fill, GList* polyList);
extern bezierObject* buildBezier(SDL_Color c, 
								 SDL_Point p0, SDL_Point p1, SDL_Point p2, SDL_Point p3);
extern void destroyObject(dioneObject *obj);
#endif
