typedef enum {
	MSG_VERBOSE_NOTE,
	MSG_VERBOSE_WARNING,
	MSG_VERBOSE_ERROR,
	MSG_VERBOSE_FATAL
} msg_verbosity;

typedef struct _msg_node {
	SDL_Texture *msg_meat;
	struct _msg_node *next;
} msg_node;

#define MSG_FLAG_NONE        0
#define MSG_FLAG_NO_SUPPRESS 1 << 0 
#define MSG_FLAG_HIGHLIGHT   1 << 1

extern void init_message_system();
extern void set_verbosity(msg_verbosity level);
extern void print_message(msg_verbosity level, char *msg, int msg_flags);
extern void render_terminal();
extern void toggle_console();
