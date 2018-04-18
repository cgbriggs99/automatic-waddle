include ../defs.mk

calcuations:
	gcc -c -o calculations.o calculations.c $(CFLAGS)
	gcc -c -o calculations2.o calculations2.c $(CFLAGS)
	gcc -c -o calculations3.o calculations3.c $(CFLAGS)
	gcc -c -o calculations4.o calculations4.c $(CFLAGS)
	ld -r -o ../calculations.o calculations.o calculations2.o calculations3.o
	
export calculations