/*
 * main.cpp
 *
 *  Created on: Apr 30, 2018
 *      Author: cgbri
 */

#include "../main.hpp"

using namespace compchem;

void initialize(Molecule **mol, int argc, char **argv) {
	if(argc == 1) {
		if(!strcmp(argv[1], "-h")) {
			puts("Enter the files in the order:");
			puts("geometry nuclear-repulsion-energy overlap");
			puts("potential-energy kinetic-energy");
			puts("electron-repulsion dipole-x dipole-y dipole-z");
			exit(0);
		}
	} else if(argc != 9) {
		perror("Please use files. Use -h to find the file order.");
		exit(-1);
	}
	FILE *geom, *eri, *s, *t, *v, *eri, *mux, *muy, *muz;
	geom = fopen(argv[1], "r");
	eri = fopen(argv[2], "r");
	s = fopen(argv[3], "r");
	t = fopen(argv[4], "r");
	v = fopen(argv[5], "r");
	eri = fopen(argv[6], "r");
	mux = fopen(argv[7], "r");
	muy = fopen(argv[8], "r");
	muz = fopen(argv[9], "r");
}


