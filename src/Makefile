OBJS 	= cpu.o bin.o util.o main.o
CC	= gcc 
DEBUG 	= -g
CFLAGS 	= -Wall -c $(DEBUG) 
LFLAGS 	= -Wall $(DEBUG)

all: msp430sim

msp430sim : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o msp430sim

cpu.o : cpu.h cpu.c 
	$(CC) $(CFLAGS) cpu.c

bin.o : bin.h bin.c 
	$(CC) $(CFLAGS) bin.c

util.o : util.h util.c 
	$(CC) $(CFLAGS) util.c

main.o : main.c 
	$(CC) $(CFLAGS) main.c

clean:
	rm *.o *~ mspemu
