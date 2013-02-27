#include <stdio.h>
#include "util.h"

void util_printreg(context_t *c) {

	int i; 

	printf("\n\nRegister Dump\n-------------\n");

	for(i = 0; i < MAX_REG; i++) {
		printf("R%02d |", i);
	}

	printf("\n");

	for(i = 0; i < MAX_REG; i++) {
		printf("%04x|", *(uint16_t *)(&c->reg[i]));
	}

	printf("\n");
}

void util_printmem(context_t *c, int s, int e) {

	char buf[64];
	word_t *w;

	int i;
	int ds;
	int de;

	word_t *pc; 
	maddr_t a = 0;
	
	if(s < 0) {
		pc = cpu_rregw(c, REG_PC); 
		a  = *((maddr_t *)(pc));
		ds = (int)(a) + s;
		if(ds < 0) ds = 0;
		de = a + e;
		if(de > MAX_MEM) de = MAX_MEM;
	}

	printf("\n\nMemory Dump (%04x..%04x)\n-------------------------\n", ds,de);

	for(i = ds; i <= de; i += sizeof(word_t)) {
		w = cpu_rmemw(c, i); 

		if(i == a) printf("-> "); else printf("   ");
		cpu_disasm_instruction(c, i, buf);
		printf("%04x|%02x%02x\t%s\n", i, w->h, w->l, buf);  	
	} 
}
