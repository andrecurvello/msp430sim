#ifndef _CPU_H
#define _CPU_H

#include <stdint.h>

#define LOW_ENDIAN	1

#define MAX_REG		16
#define MAX_MEM		2048	

#define	REG_PC		0
#define	REG_SP		1
#define	REG_SR		2
#define	REG_CG1		3
#define	REG_CG2		4

typedef uint16_t 	maddr_t;
typedef uint8_t 	raddr_t;
typedef uint8_t 	byte_t;

typedef struct {
#ifdef LOW_ENDIAN
	byte_t	l;
	byte_t	h;
#else
	byte_t	h;
	byte_t	l;
#endif
} word_t;

typedef struct {
	word_t 	reg[MAX_REG];
	byte_t	mem[MAX_MEM];
} context_t;

void cpu_reset(context_t *c);

void cpu_wregw(context_t *c, raddr_t r, word_t v);

word_t *cpu_rregw(context_t *c, raddr_t r); 

void cpu_wregb(context_t *c, raddr_t r, byte_t v);

byte_t *cpu_rregb(context_t *c, raddr_t r);

void cpu_wmemw(context_t *c, maddr_t a, word_t v);

word_t *cpu_rmemw(context_t *c, maddr_t a);

void cpu_wmemb(context_t *c, maddr_t a, byte_t v);

byte_t *cpu_rmemb(context_t *c, maddr_t a);

int cpu_process_instruction(context_t *c);

int cpu_disasm_instruction(context_t *c, maddr_t a, char *buf);

#endif
