#include "objects.h"

void obj_set_update_frequency(dioneObject* obj, int freq) {
	obj->update_frequency = freq;
	return;
}