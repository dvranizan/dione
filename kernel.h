#include "SDL2/SDL.h"

#include "objects.h"

#ifndef _KERNEL_H
#define _KERNEL_H

typedef enum _EVENT_TYPE {
	EVENT_KEYPRESS
} EVENT_TYPE;

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
extern void kernel_register_object(dioneObject *obj);
extern SDL_bool kernel_remove_object(dioneObject *obj);

extern void init_kernel();
#endif
