#include <stdio.h>
#include "bin.h"

int bin_load(char *file, context_t *c, maddr_t a) {

	FILE *f;

	maddr_t max = MAX_MEM - a + sizeof(word_t); 

	unsigned char *mem = (unsigned char *)(&c->mem[a]);

	f = fopen(file, "rb");

	if (f) {
	    return fread(mem, max, 1, f);
	}

	return -1;
}
