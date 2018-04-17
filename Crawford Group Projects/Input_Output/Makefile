include ../defs.mk

input_output:
	gcc $(CFLAGS) -c -o input.o input.c
	gcc $(CFLAGS) -c -o output.o output.c
	ld -r -o ../io.o input.o output.o