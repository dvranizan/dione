#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "objects.h"

#ifndef _KERNEL_H
#define _KERNEL_H

typedef enum _EVENT_TYPE {
	EVENT_KEYPRESS,
	EVENT_PROC_SPAWN
} EVENT_TYPE;

typedef enum _INPUT_CAPTURE_TYPE {
	INPUT_CAPTURE_NONE,
	INPUT_CAPTURE_NORMAL,
	INPUT_CAPTURE_BLOCK,
} INPUT_CAPTURE_TYPE;

typedef struct _event {
	EVENT_TYPE type;
	int event_register_time;
	int event_start_time;
} dioneEvent;

typedef struct {
	dioneEvent ev_base;
	SDL_Keysym key;
	Uint32 key_type;
} dioneEventKey;

typedef struct {
	dioneEvent ev_base;
	void (*spawnFunc)(dioneObject*);
} dioneEventSpawn;

extern SDL_bool kernel_addSelected(dioneObject *obj);
extern SDL_bool kernel_clearSelected(dioneObject *obj);
extern SDL_bool kernel_clearSelectedAll();

extern SDL_bool kernel_push_input_grab(dioneObject *obj);
extern SDL_bool kernel_pop_input_grab(dioneObject *obj);
extern SDL_bool kernel_register_input_grab(dioneObject *obj);
extern SDL_bool kernel_clear_input_grab(dioneObject *obj);
extern SDL_bool kernel_ultimate_input_grab(dioneObject *obj);
extern SDL_bool kernel_ultimate_input_clear(dioneObject *obj);

extern void kernel_queueEvent(dioneEvent *ev);
extern void kernel_queueKeyboardEvent(Uint32 evType, SDL_Keysym key);
extern void kernel_exec_events();

extern void kernel_handle_objects();
extern void kernel_register_object_simple(dioneObject *obj);
extern void kernel_register_object(dioneObject *obj, int winID, INPUT_CAPTURE_TYPE captureType);
extern SDL_bool kernel_remove_object(dioneObject *obj);

extern SDL_bool kernel_remove_listener(dioneObject *obj);
extern void kernel_add_listener(dioneObject *obj, INPUT_CAPTURE_TYPE captureType);

extern SDL_bool kernel_remove_window(int winID);

extern void init_kernel();
#endif
