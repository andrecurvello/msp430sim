#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"

void cpu_init(context_t *c) {
	
	memset(c->reg, 0, MAX_REG * (sizeof(word_t) / sizeof(uint8_t)));
	memset(c->mem, 0, MAX_MEM * (sizeof(byte_t) / sizeof(uint8_t)));
}

void cpu_reset(context_t *c) {
		
	// set PC to address from reset vector
	word_t *rv = cpu_rmemw(c, 0xfffe);
	cpu_wregw(c, REG_PC, *rv);
}

void cpu_wregw(context_t *c, raddr_t r, word_t v) {

	memcpy(&c->reg[r], &v, sizeof(word_t));
}

word_t *cpu_rregw(context_t *c, raddr_t r) {

	return &c->reg[r];
}

void cpu_wregb(context_t *c, raddr_t r, byte_t v) {

	word_t w;

	w.l = v; 
	w.h = 0x00;

	memcpy(&c->reg[r], &w, sizeof(word_t));
}

byte_t *cpu_rregb(context_t *c, raddr_t r) {

	return &c->reg[r].l;
}

void cpu_wmemw(context_t *c, maddr_t a, word_t v) {

	word_t *p = (word_t *)(&c->mem[a]);
	*p = v;
} 

word_t *cpu_rmemw(context_t *c, maddr_t a) {

	word_t *p = (word_t *)(&c->mem[a]);

	return p;
} 

void cpu_wmemb(context_t *c, maddr_t a, byte_t v) {

	byte_t *p = (byte_t *)(&c->mem[a]);
	*p = v;
} 

byte_t *cpu_rmemb(context_t *c, maddr_t a) {

	byte_t *p = (byte_t *)(&c->mem[a]);

	return p;
} 

int cpu_process_instruction(context_t *c) {

	int op;
	int ad; 
	int bw;
	int as;
	int sr;
	int dr;

	word_t *sv;
	word_t *dv;

	word_t *pc = cpu_rregw(c, REG_PC); 
	maddr_t  a = *((maddr_t *)(pc));
	
	word_t *inst = cpu_rmemw(c, a);
	(*(maddr_t *)(pc)) += sizeof(word_t);

	if((inst->h & 0x11110000) == 0x11110000) {
		// printf("One-operanLoadingd instruction\n");
	}
	else if((inst->h & 0x11100000) == 0x11100000) {
		// printf("Condition instruction\n");
	}
	else {
		op =  inst->h >> 4;
		sr =  inst->h       & 0b0001111;
		ad =  inst->l >> 7;
		bw = (inst->l >> 6) & 0b00000001;
		as = (inst->l >> 4) & 0b00000011;
		dr =  inst->l       & 0b0001111;
		
		// select opcode
		switch(op) {
			case 0b0100: 
				// Addressing mode: register mode (e.g.: mov.w R10, R11)
				if(as == 0) {
					if(ad == 0) {
						// Byte or word operation
						if(bw) cpu_wregb(c, dr, *cpu_rregb(c, sr)); else cpu_wregw(c, dr, *cpu_rregw(c, sr));
					}
				}	
				// Addressiong mode: immediate mode (e.g. mov.w #0xaffe, R12, mov.w #0xcafe, &0x0120)
				else if(as == 3) {
					// read source value from next word
					a  = *((maddr_t *)(pc));
					sv = cpu_rmemw(c, a);
					(*(maddr_t *)(pc)) += sizeof(word_t);

					// to register
					if(ad == 0) {
						// Byte or word operation
						if(bw) cpu_wregb(c, dr, sv->l); else cpu_wregw(c, dr, *sv);
					}
					else if(ad == 1) {
						dv = cpu_rmemw(c, a + sizeof(word_t));
						(*(maddr_t *)(pc)) += sizeof(word_t);
						if(bw) cpu_wmemb(c, *((maddr_t *)(dv)), sv->l); else cpu_wmemw(c, *((maddr_t *)(dv)), *sv);
					}
				}

				break;
			default:
				return -1;
		}
	}

	return 0;
}

int cpu_disasm_instruction(context_t *c, maddr_t a, char *buf) {

	int op;
	int ad; 
	int bw;
	int as;
	int sr;
	int dr;

	word_t *sv;
	word_t *dv;
	word_t *inst = cpu_rmemw(c, a);

	buf[0] = 0;

	if((inst->h & 0x11110000) == 0x11110000) {
		// printf("One-operanLoadingd instruction\n");
	}
	else if((inst->h & 0x11100000) == 0x11100000) {
		// printf("Condition instruction\n");
	}
	else {
		op =  inst->h >> 4;
		sr =  inst->h       & 0b0001111;
		ad =  inst->l >> 7;
		bw = (inst->l >> 6) & 0b00000001;
		as = (inst->l >> 4) & 0b00000011;
		dr =  inst->l       & 0b0001111;
		
		// printf("\nop=%02x, sr=%02x, ad=%02x, bw=%02x, as=%02x, dr=%02x\n", op, sr, ad, bw, as, dr);

		sprintf(&buf[strlen(buf)], "[op=%02x, sr=%02x, ad=%02x, bw=%02x, as=%02x, dr=%02x]\t", op, sr, ad, bw, as, dr);

		// select opcode
		switch(op) {
			case 0b0100: 
				strcat(buf, "mov.");
				// Byte or word operation
				if(bw) strcat(buf, "b"); else strcat(buf, "w");

				// Addressing mode: register (e.g.: mov.w R10, R11)
				if(as == 0) { 
					// to register
					if(ad == 0) {
						sprintf(&buf[strlen(buf)], " R%d, R%d", sr, dr);
					}
					// to indexed location 
					else if(ad == 1) {
						// read destination value from next word
						dv = cpu_rmemw(c, a + 2 * sizeof(word_t));
						sprintf(&buf[strlen(buf)], " R%d, 0x%02x%02x(R%d)", sr, dv->h, dv->l, dr);
					}
				}
				// Addressing mode: indexed (e.g.: mov.w 2(R5), R4) 
				else if(as == 1) {
					
					// read source value from next word
					sv = cpu_rmemw(c, a + 1 * sizeof(word_t));
					
					// to register
					if(ad == 0)  {
						sprintf(&buf[strlen(buf)], " 0x%02x%02x(R%d), R%d", sv->h, sv->l, sr, dr); 
					}
					// to location
					else if(ad == 1) {
						// read destination value from next word
						dv = cpu_rmemw(c, a + 2 * sizeof(word_t));

						if(dr == 2) {
							sprintf(&buf[strlen(buf)], " 0x%02x%02x(R%d), &0x%02x%02x", 
								sv->h, sv->l, sr, dv->h, dv->l); 
						}
						else {
							sprintf(&buf[strlen(buf)], " 0x%02x%02x(R%d), 0x%02x%02x(R%d)", 
								sv->h, sv->l, sr, dv->h, dv->l, dr); 
						}
					}
				}
				// Addressiong mode: immediate (e.g. mov.w #0xaffe, R12, mov.w #0xcafe, &0x0120)
				else if(as == 3) {

					// read source value from next word
					sv = cpu_rmemw(c, a + 1 * sizeof(word_t));

					// to register
					if(ad == 0)  {
						if(bw == 1)
							sprintf(&buf[strlen(buf)], " #0x%02x, R%d", sv->l, dr); 
						else
							sprintf(&buf[strlen(buf)], " #0x%02x%02x, R%d", sv->h, sv->l, dr); 
					}
					// to abs location
					else if(ad == 1) {

						// read destination value from next word
						dv = cpu_rmemw(c, a + 2 * sizeof(word_t));

						if(bw == 1)
							sprintf(&buf[strlen(buf)], " #0x%02x, &0x%02x%02x", sv->l, dv->h, dv->l); 
						else
							sprintf(&buf[strlen(buf)], " #0x%02x%02x, &0x%02x%02x", sv->h, sv->l, dv->h, dv->l); 
					}
				}
				break;
			default:
				return -1;
		}
	}
	

	return 0;
}

