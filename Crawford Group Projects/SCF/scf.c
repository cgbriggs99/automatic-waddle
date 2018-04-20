/*
 * main3.c
 *
 *  Created on: Sep 15, 2017
 *      Author: connor
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../molecule.h"
#include "../calculations.h"
#include "../input.h"
#include "../output.h"
#include "../linker.h"

int main(int argc, char **argv) {
	linkDLLs();
	FILE *mol, *enuc, *s, *t, *v, *eri, *mux, *muy, *muz;
	molecule_t *molec;
	double **fmo;

	if(argc < 9) {
		perror("Please use files. It gets messy when you don't.");
		return (0);
	}
	mol = fopen(argv[1], "r");
	enuc = fopen(argv[2], "r");
	s = fopen(argv[3], "r");
	t = fopen(argv[4], "r");
	v = fopen(argv[5], "r");
	eri = fopen(argv[6], "r");
	mux = fopen(argv[7], "r");
	muy = fopen(argv[8], "r");
	muz = fopen(argv[9], "r");



	inputSCF(&molec, mol, enuc, s, t, v, eri, mux, muy, muz);

	findFockPrint(molec);

	calculateCharges(molec);

	printArray("\nDensity Matrix\n", molec->density, molec->orbitals, molec->orbitals);

	outputSFC(molec);

	deleteMolecule(molec);

	fclose(mol);
	fclose(enuc);
	fclose(s);
	fclose(v);
	fclose(eri);
	fclose(mux);
	fclose(muy);
	fclose(muz);

	unlinkDLLs();

	return (0);
}
