#include "SDL2/SDL.h"
#include <assert.h>
#include "glib.h"

#include "logging.h"
#include "font.h"
#include "globals.h"
#include "objects.h"
#include "build.h"
#include "kernel.h"
#include "console.h"

static GList *gMessageList = NULL;
static SDL_Color default_color = {0,0,0};
static SDL_Color highlight_color = {0,0,0};
static msg_verbosity global_verbosity = MSG_VERBOSE_WARNING;
static int console_hidden = 0;
static customObject *console_obj = NULL;

/* for console input buffer */
GString *logging_input_buffer = NULL;

#define CONSOLE_BUFFER_PROMPT "> "
#define CONSOLE_BUFFER_PROMPT_LENGTH strlen(CONSOLE_BUFFER_PROMPT) 

void register_console() {
	assert(!console_obj);
	console_obj = buildCustom(&update_terminal, &draw_terminal, &listen_terminal);
	kernel_add_listener((dioneObject*)console_obj, INPUT_CAPTURE_NORMAL);
}

void toggle_console() {
	assert(console_obj);
	console_hidden = (console_hidden) ? 0 : !console_hidden;

	if (!console_hidden) {
		/* register term w/ kernel to draw */
		kernel_register_object((dioneObject*)console_obj, WINDOW_STACK_ON_TOP, INPUT_CAPTURE_NONE);
		/* TODO - elevate listening to block input to other objects */
	} else {
		/* remove the terminal */
		(void)kernel_remove_object((dioneObject*)console_obj);
		//destroyObject((dioneObject*)console_obj);
		//console_obj = NULL;
	}
}

static msg_node *new_msg_node() {
	msg_node *current = (msg_node*)malloc(sizeof(msg_node));
	/* put at front, we iterate first to last */
	gMessageList = g_list_prepend(gMessageList, current);
	return current;
}

static void fill_msg_node(msg_node *msg, char *meat, int msg_flags) {
	SDL_Color c = (msg_flags & MSG_FLAG_HIGHLIGHT) ? highlight_color : default_color;
	msg->msg_meat = render_font(meat, c);
}

void print_message(msg_verbosity level, char *msg, int msg_flags) {
	msg_node *new_msg;
	
	if (msg_flags & MSG_FLAG_NO_SUPPRESS || level >= global_verbosity) {
	} else {
		/* this is not high enough priority to print */
		return;
	}
	new_msg = new_msg_node();
	fill_msg_node(new_msg, msg, msg_flags);
}

void set_verbosity(msg_verbosity level) {
	global_verbosity = level;
}

void init_message_system() {
	/* set msg color */
	default_color = (SDL_Color){255, 255, 255};
	highlight_color = (SDL_Color){255, 0, 0};
	console_hidden = 1; /*default hidden*/
	assert(!logging_input_buffer);
	logging_input_buffer = g_string_new(CONSOLE_BUFFER_PROMPT);
	/* crank out a initial "welcome to whatever" msg */
	print_message(MSG_VERBOSE_NOTE, "Welcome to DIONE!", MSG_FLAG_NO_SUPPRESS|MSG_FLAG_HIGHLIGHT);
	
}

void draw_terminal(dioneObject *obj) {
	GList *l = gMessageList;
	SDL_Rect msg_pos;
	SDL_Rect msg_back;

	if (console_hidden) {
		return;
	}

	msg_pos.x = 0;
	/* draw an opaque box for term */
	msg_back.x = 0;
	msg_back.y = 0;
	msg_back.w = SCREEN_WIDTH;
	msg_back.h = SCREEN_HEIGHT/2;
	msg_pos.y = msg_back.h;
	SDL_SetRenderDrawColor(global_renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(global_renderer, &msg_back);

	/* first draw input buffer */
	if (logging_input_buffer) {
		SDL_Texture *s = render_font(logging_input_buffer->str, default_color);
		SDL_QueryTexture(s, NULL, NULL, &msg_pos.w, &msg_pos.h);
		/* msg_pos starts at bottom of terminal, shift up and draw */
		msg_pos.y -= msg_pos.h;
		SDL_RenderCopy(global_renderer, s, NULL, &msg_pos);
		SDL_DestroyTexture(s);
	}

	/* iterate over messages as long as we have room */
	for (l = gMessageList; l != NULL; l = l->next) {
		int h, w;
		msg_node *node = (msg_node*)(l->data);
		/*render text*/
		SDL_QueryTexture(node->msg_meat, NULL, NULL, &msg_pos.w, &msg_pos.h);
		/* msg_pos starts at bottom of terminal, shift up and draw */
		msg_pos.y -= msg_pos.h;
		if (msg_pos.y < 0) {
			break;
		}
		SDL_RenderCopy(global_renderer, node->msg_meat, NULL, &msg_pos);
	}
}

void update_terminal(dioneObject *obj) {
	/* STUB */
}

void listen_terminal(dioneObject *obj, void *data) {
	dioneEventKey *key = data;
	assert(TYPEOF(obj) == OBJ_CUSTOM);

	/* only listen to keydowns */
	if (key->key_type != SDL_KEYDOWN)
		return;

	switch (key->key.sym) {
	case SDLK_BACKQUOTE:
		/* toggle the terminal */
		toggle_console();
		break;
	case SDLK_RETURN:
		/* echo the command */
		print_message(MSG_VERBOSE_ALL, 
					  logging_input_buffer->str+CONSOLE_BUFFER_PROMPT_LENGTH, 
					  MSG_FLAG_NO_SUPPRESS);
		/* exec the command */
		console_exec_string(logging_input_buffer->str + CONSOLE_BUFFER_PROMPT_LENGTH);
		/* clear the input buffer */
		g_string_set_size(logging_input_buffer, CONSOLE_BUFFER_PROMPT_LENGTH);
		break;
	case SDLK_BACKSPACE:
		/* truncate buffer by 1 */
		if (!logging_input_buffer || 
			strlen(logging_input_buffer->str) <= CONSOLE_BUFFER_PROMPT_LENGTH) {
			break;
		}
		logging_input_buffer = g_string_truncate(logging_input_buffer,
												 (strlen(logging_input_buffer->str) - 1));
		break;
	default:
		/* slam it into the buffer */
		if (console_hidden) {
			break;
		}
		if (!logging_input_buffer) {
			assert(0);
		}
		/* if ascii append */
		if (key->key.sym < 0x80 && key->key.sym > 0 ) {
			g_string_append_c(logging_input_buffer, g_ascii_tolower (key->key.sym));
		}
		break;
	}
}
