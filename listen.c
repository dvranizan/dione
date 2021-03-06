#include "listen.h"
#include "objects.h"
#include "logging.h"

void listen_handler(dioneObject *obj, void *data) {
	switch (TYPEOF(obj)) {
	case OBJ_LINE:
	case OBJ_PEOPLE:
	case OBJ_WAVE:
		/*unimplimented*/
		break;
	case OBJ_CUSTOM:
		(((customObject*)obj)->listenFunc)(obj, data);
		break;
	default:
		/* I never want to be here */
		print_message(MSG_VERBOSE_ERROR, MSG_FLAG_NONE, "[listener] I have no idea how to handle this! ptr: %x", obj);
	}
}
