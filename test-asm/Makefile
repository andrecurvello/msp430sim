AS		= naken430asm 
ASFLAGS	= -d -e 
OBJCP	= msp430-objcopy

all: mov

mov: mov.elf mov.bin mov.hex
 
mov.elf: mov.asm 
	$(AS) $(ASFLAGS) mov.asm -o mov.elf 

mov.bin: mov.elf
	$(OBJCP) -Obinary mov.elf mov.bin

mov.hex: mov.elf
	$(OBJCP) -Oihex mov.elf mov.hex
	
clean:
	rm *.elf 
	rm *.hex 
	rm *.bin 
	rm *~ 
