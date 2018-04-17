include ../paths.mk
include ../defs.mk

all:
	$(CC) -g3 -ggdb -c -o alloc.o alloc.c
	$(CC) -g3 -ggdb -c -o free.o free.c
	$(CC) -g3 -ggdb -c -o contig.o contig.c
	$(CC) -shared -o ../memory.dll contig.o alloc.o free.o