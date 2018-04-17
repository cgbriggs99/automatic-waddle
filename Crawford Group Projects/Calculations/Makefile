include ../defs.mk

calcuations:
	gcc $(CFLAGS) -c -o calculations.o calculations.c
	gcc $(CFLAGS) -c -o calculations2.o calculations2.c
	gcc $(CFLAGS) -c -o calculations3.o calculations3.c
	gcc $(CFLAGS) -c -o calculations4.o calculations4.c
	ld -r -o ../calculations.o calculations.o calculations2.o calculations3.o
	
export calculations