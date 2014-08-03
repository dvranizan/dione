#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif
#include <glib.h>
#include <assert.h>

#include "kernel.h"
#include "globals.h"
#include "objects.h"
#include "fps.h"
#include "logging.h"
#include "listen.h"
static dioneObject *gSelectedObjects[KERNEL_MAX_SELECTED_OBJECTS];
static GHashTable *gQueuedEvents;
static GHashTable *gRenderObjects;
static GSList *gInputObjects;
static int gSelectedObjects_index = 0;

void kernel_register_object_simple(dioneObject *obj) {
	kernel_register_object(obj, WINDOW_STACK_DEFAULT, INPUT_CAPTURE_NONE);
}

void kernel_register_object(dioneObject *obj, 
							WINDOW_STACK_TYPE winType, INPUT_CAPTURE_TYPE captureType) {
	/* register OBJ with kernel */
	print_message(MSG_VERBOSE_NOTE, "Registering obj...", MSG_FLAG_NONE);
	g_hash_table_add(gRenderObjects, obj);
	/* if it listens to input, register it */
	if (captureType > INPUT_CAPTURE_NONE) {
		gInputObjects = g_slist_prepend(gInputObjects, obj);
	}
}

SDL_bool kernel_remove_object(dioneObject *obj) {
	if (g_hash_table_remove(gRenderObjects, obj)) {
		return SDL_TRUE;
	} else {
		return SDL_FALSE;
	}
}

SDL_bool kernel_remove_listener(dioneObject *obj) {
	gInputObjects = g_slist_remove(gInputObjects, obj);
	return SDL_TRUE;
}

void kernel_add_listener(dioneObject *obj, INPUT_CAPTURE_TYPE captureType) {
	if (captureType > INPUT_CAPTURE_NONE) {
		gInputObjects = g_slist_prepend(gInputObjects, obj);
	}
}

/* called by world loop, deal with all registered objects */
void kernel_handle_objects() {
	int x;
	GHashTableIter iter;
	gpointer key, value;

	g_hash_table_iter_init(&iter, gRenderObjects);
	while (g_hash_table_iter_next(&iter, &key, &value)) {
		dioneObject *dObj = (dioneObject*)value;
		/* update phase -
		   hit every object and mutate appropriately */
		updateObject(dObj);
		/* draw phase -
		   if objects are drawable, draw . does not affect game world */
		drawObject(dObj);
	}
}

/*
  setup dynamic kernel objects and get things rolling...
*/
void init_kernel() {
	/* use default hash func for now, will probably need to look at this for
	   performance
	*/
	gQueuedEvents = g_hash_table_new(NULL, NULL);
	gRenderObjects = g_hash_table_new(NULL, NULL);
	gInputObjects = NULL;

	/* built in hooks */
	register_console();
}

/*
  kernel_init_event() should init all parts of the base dioneEvent struct
*/
static void kernel_init_event(dioneEvent *ev, EVENT_TYPE t, int evTime) {
	ev->type = t;
	ev->event_start_time = evTime;
	ev->event_register_time = fps_get_ticks();
}

SDL_bool kernel_addSelected(dioneObject *obj) {
}

/* 
   broadcast keystroke to any obj listening.
*/
static void kernel_broadcast_key_event(dioneEventKey *key) {
	GSList *iter = gInputObjects;
	while(iter) {
		listen_handler((dioneObject*)(iter->data), key);
		iter = g_slist_next(iter);
	}
	
}
/*
  event has been flagged to exec, eventually falls here.
  for now this is a big case, but probably a good idea to break
  it up into functions.  meh.
*/
static void kernel_exec_event(dioneEvent *ev) {
	switch (ev->type) {
	case EVENT_KEYPRESS: {
		dioneEventKey *key = (dioneEventKey*)ev;
		print_message(MSG_VERBOSE_NOTE, "Key pressed!", MSG_FLAG_NONE);
		switch (key->key.sym) {
		case SDLK_w:
		case SDLK_s:
		case SDLK_a:
		case SDLK_d:
			/* player WASD bro! */
			break;
		default:
			break;	
		}
		/* all special high level kernel keyjacking done.
		   send key event to all obj's registered to listen */
		kernel_broadcast_key_event(key);
		break;
	}
	default:
		/* we should never get here */
		print_message(MSG_VERBOSE_ERROR, "Trying to handle an event I don't understand!", MSG_FLAG_NONE);
	}
}

/*
  iterate over all queued events and if current time >= start time, execute
  INCOMPLETE LIKE WOAH
*/
void kernel_exec_events() {
	GHashTableIter iter;
	gpointer key, value;
	int current_time = fps_get_ticks();
	
	g_hash_table_iter_init(&iter, gQueuedEvents);
	while (g_hash_table_iter_next(&iter, &key, &value)) {
		dioneEventKey *dEv = (dioneEventKey*)value;
		if (dEv->ev_base.event_start_time <= current_time) {
			/* execute proc */
			kernel_exec_event((dioneEvent*)dEv);
			/* cleanup */
			g_hash_table_iter_remove(&iter);
		}
	}
}

void kernel_queueEvent(dioneEvent *ev) {
	/* add event to event wheel */
	g_hash_table_add(gQueuedEvents, ev);
}

void kernel_queueKeyboardEvent(Uint32 evType, SDL_Keysym key) {
	dioneEventKey *dEv = malloc(sizeof(dioneEventKey));

	/* only two SDL events handled here */
	assert(evType == SDL_KEYDOWN || evType == SDL_KEYUP);

	kernel_init_event((dioneEvent*)dEv, EVENT_KEYPRESS, fps_get_ticks());
	dEv->key = key;
	dEv->key_type = evType;
	/* throw onto wheel */
	kernel_queueEvent((dioneEvent*)dEv);
}
