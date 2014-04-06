#inclube "SDL2/SDL.h"
#include "globals.h"

extern void registerAddGameObject(dioneObject *obj);
extern void registerAddHUDObject(dioneObject *obj);

extern void registerAddToGameObject(dioneObject *mainObj, dioneObject *new);
extern void registerAddToHUDObject(dioneObject *mainObj, dioneObject *new);

extern SDL_bool registerRemoveGameObject(int id);
extern SDL_bool registerRemoveHUDObject(int id);

extern dioneObject** getGameList();
extern dioneObject** getHUDList();
