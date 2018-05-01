/*
 * main.cpp
 *
 *  Created on: Apr 30, 2018
 *      Author: cgbri
 */

#ifndef MAIN_CPP__
#define MAIN_CPP__

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include "../compchem.hpp"

using namespace compchem;

void compchem::initialize(class Molecule **mol, int argc, char **argv) {
	if(argc == 1) {
		if(!strcmp(argv[1], "-h")) {
			puts("Enter the files in the order:");
			puts("geometry nuclear-repulsion-energy overlap");
			puts("potential-energy kinetic-energy");
			puts("electron-repulsion dipole-x dipole-y dipole-z hessian");
			exit(0);
		}
	} else if(argc < 10) {
		perror("Please use files. Use -h to find the file order.");
		exit(-1);
	}

	FILE *geom, *enuc, *s, *t, *v, *eri, *mux, *muy, *muz, *hessian;
	geom = fopen(argv[1], "r");
	enuc = fopen(argv[2], "r");
	s = fopen(argv[3], "r");
	t = fopen(argv[4], "r");
	v = fopen(argv[5], "r");
	eri = fopen(argv[6], "r");
	mux = fopen(argv[7], "r");
	muy = fopen(argv[8], "r");
	muz = fopen(argv[9], "r");
	hessian = fopen(argv[10], "r");
	inputSCF(mol, geom, enuc, s, t, v, eri, mux, muy, muz, hessian);
	fclose(geom);
	fclose(enuc);
	fclose(s);
	fclose(t);
	fclose(v);
	fclose(eri);
	fclose(mux);
	fclose(muy);
	fclose(muz);
	fclose(hessian);
}
#endif

