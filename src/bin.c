#include <stdio.h>
#include "bin.h"

int bin_load(char *file, context_t *c) {

	FILE *f;

	f = fopen(file, "rb");

	if (f) {
	    fread((unsigned char *)c->mem, MAX_MEM , 1, f);
	}
	else {
		return -1;
	}

	return 0;
}
