#ifndef _UTIL_H
#define _UTIL_H

#include "cpu.h"

void util_printreg(context_t *c);

void util_printmem(context_t *c, int s, int e);

#endif
