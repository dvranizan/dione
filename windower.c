#ifdef __GNUC__
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include <assert.h>

#include "windower.h"
#include "globals.h"
#include "objects.h"
#include "logging.h"

static GSList *gWindowList;
static int window_id_count;

void init_windower() {
	SDL_Rect orig = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	
	gWindowList = NULL;
	window_id_count = -1;
	/* create a max size window */
	(void)windower_create_window(orig);
}
int windower_create_window(SDL_Rect loc) {
	dioneWindow* new_window = malloc(sizeof(dioneWindow));
	print_message(MSG_VERBOSE_NOTE, "[windower] Creating window...", MSG_FLAG_NONE);
	new_window->id = ++window_id_count;
	new_window->flags = FLAGS_WINDOW_NOFLAGS;
	new_window->loc = loc;
	new_window->objects = NULL;
	/* add window to global list */
	gWindowList = g_slist_prepend(gWindowList, new_window);
	kernel_init_window(new_window);
	return new_window->id;
}

static GSList* windower_remove_window(dioneWindow *win) {
	GSList *ret;
	assert (win->id != 0); //can never remove the top window
	print_message(MSG_VERBOSE_NOTE, "[windower] Removing window...", MSG_FLAG_NONE);
	if ((ret = g_slist_remove(gWindowList, win))) {
		/* do some cleanup */
		//SDL_DestroyTexture(win->texture);
		/* TODO: proper removal of objects */
		g_tree_destroy(win->objects);
		free(win);
	}
	return ret;
}

/* create a new window 'inside' (accumulate offsets) current window */
/* if x, y, h, w > 0, use them, else determine w/ enums */
int windower_push_window(SDL_Rect size, WINDOW_STACK_TYPE winType, WINDOW_SPAN_TYPE winSpan) {
	/* STUBBIN HARD */
}

dioneWindow* windower_find_window(int win_id) {
	GSList *iter = gWindowList;
	while (iter) {
		if (((dioneWindow*)(iter->data))->id == win_id) {
			return iter->data;
		}
		iter = iter->next;
	}
	return NULL;
}

GSList* windower_get_windows() {
	return gWindowList;
}

SDL_bool windower_update_windows() {
	GSList *iter = gWindowList;
	dioneWindow *target = NULL;
	while (iter) {
		target = ((dioneWindow*)(iter->data));
		iter = iter->next;
		if (target->flags & FLAGS_WINDOW_REMOVE) {
			/* TODO: We iterate twice here, bad */
			gWindowList = windower_remove_window(target);
		}
	}
	/* TODO: smarter return values */
	return SDL_TRUE;	
}
