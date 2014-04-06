#include "SDL2/SDL.h"

#include "logging.h"
#include "font.h"
#include "globals.h"


msg_node *msg_root = NULL;
static SDL_Color default_color = {0,0,0};
static SDL_Color highlight_color = {0,0,0};
static int msg_count = 0;
static msg_verbosity global_verbosity = MSG_VERBOSE_WARNING;
static int console_hidden = 0;

void toggle_console() {
	console_hidden = (console_hidden) ? 0 : !console_hidden;
}

static msg_node *new_msg_node() {
	msg_node *current = msg_root;
	
	if (msg_count >= MAX_MESSAGES) {
		/* remove root */
		current = msg_root->next;
		free(msg_root);
		msg_root = current;
	} else {
		++msg_count;
	}

	if (!current) {
		/* first msg */
		msg_root = (msg_node*)malloc(sizeof(msg_node));
		msg_root->next = NULL;
		return msg_root;
	}
	while (current && current->next) {
		current = current->next;
	}
	current->next = (msg_node*)malloc(sizeof(msg_node));
	current->next->next = NULL;
	return current->next;
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
	/* crank out a initial "welcome to whatever" msg */
	print_message(MSG_VERBOSE_NOTE, "Welcome to DIONE!", MSG_FLAG_NO_SUPPRESS|MSG_FLAG_HIGHLIGHT);
	
}

void render_terminal() {
	int msg_index;
	msg_node *current_msg = msg_root;
	SDL_Rect msg_pos;
	SDL_Rect msg_back;

	if (console_hidden) {
		return;
	}

	msg_pos.y = 0;
	msg_pos.x = 0;
	/* draw an opaque box for term */
	msg_back.x = 0;
	msg_back.y = 0;
	msg_back.w = SCREEN_WIDTH;
	msg_back.h = SCREEN_HEIGHT/2;
	SDL_SetRenderDrawColor(global_renderer, 0, 0, 0, 0);
	SDL_RenderFillRect(global_renderer, &msg_back);

	/*spin to first message we want to print*/
	/* TODO: this is hardset at 25 but we prob want to be able to scroll */
	for (msg_index = 0; current_msg && msg_count - msg_index > 25; msg_index++) {
		current_msg = current_msg->next;
	}
	
	while (current_msg) {
		int h, w;
		/*render text*/
		SDL_QueryTexture(current_msg->msg_meat, NULL, NULL, &msg_pos.w, &msg_pos.h);
		SDL_RenderCopy(global_renderer, current_msg->msg_meat, NULL, &msg_pos);
		/*increase pos*/
		msg_pos.y += msg_pos.h;
		current_msg = current_msg->next;
	}
}
