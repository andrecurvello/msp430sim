
#include <unistd.h>
#include <stdio.h>

#include "cpu.h"
#include "util.h"
#include "bin.h"

int main(int argc, char **argv) {

	char *binfile = NULL;
	char  c;
	int   o;
	int   s;
	int   e;
     
	opterr = 0;
     
	while ((o = getopt (argc, argv, "i:")) != -1) {
		switch (o) {
			case 'i':
		 		binfile = optarg;
		 	break;
	   }
	}
 
	if(!binfile) {
		printf("ERROR no input file (-i) given.\n");
		return -1;
	}

	context_t ctx; 

	printf("\n\n*** MSP430SIM ***\n\n");
	printf("- s\tstep\n");
	printf("- r\treset\n");
	printf("- q\tquit\n\n");
	printf("Loading bin file from: %s\n", binfile);

	if(bin_load(binfile, &ctx, LOAD_ADDR) == -1) {
		printf("ERROR opening file: %s\n", binfile);
		return -1;
	}

	cpu_reset(&ctx);

	do {
		scanf("%c", &c);

		switch(c) {
			case 's':
				util_printreg(&ctx);	
				util_printmem(&ctx, -8, 8); 

				if(cpu_process_instruction(&ctx) != 0) {
					printf("\nERROR illigal instruction\n");
				}
				else {
					printf("\nOK\n");
				}

				break;
			case 'r':
				bin_load(binfile, &ctx, LOAD_ADDR);
				cpu_reset(&ctx);

				printf("RESET\n");
				break;
			case 'm':
				printf("\nfrom address (hex): ");
				scanf("%04x", &s);
				printf("to   address (hex): ");
				scanf("%04x", &e);
				util_printmem(&ctx, s, e);
				break;
			case 'q':
				printf("QUIT\n");
				break;
		}
	}
	while(c != 'q');

	return 0;
}
