#include "console.h"
#include "fps.h"

/* string input assumed all lower case */
void console_exec_string(char *str) {
	char *str_tmp = str;
	if (strncmp(str_tmp, "show ", 5) == 0) {
		/* shift */
		str_tmp += 5;
		if (strncmp(str_tmp, "fps", 3) == 0) {
			toggle_fps();
		}
	}
}
