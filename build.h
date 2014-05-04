#include "objects.h"

#ifndef _BUILD_H
#define _BUILD_H
extern waveObject* buildWave(int num);
extern humanObject* buildHuman();
extern customObject* buildCustom(void(*updateFunc)(dioneObject*), 
								 void(*drawFunc)(dioneObject*), 
								 void(*listenFunc)(dioneObject*, void*));
extern void destroyObject(dioneObject *obj);
#endif
