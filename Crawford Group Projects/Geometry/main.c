/*
 * main.c
 *
 *  Created on: Sep 12, 2017
 *      Author: connor
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "../input.h"
#include "../output.h"
#include "../calculations.h"
#include "../molecule.h"


int main(int argc, char **argv) {
	puts("db0");
	fflush(stdout);
	FILE *fp;
	if(argc < 2) {
		fp = stdin;
	} else {
		fp = fopen(argv[1], "r");
	}

	puts("db1");
	fflush(stdout);
	molecule_t *mol;
	input(&mol, fp);
	printDists(mol);
	printAngles(mol);
	printMoments(mol);
	printRotor(mol);
	fclose(fp);
	deleteMolecule(mol);
	puts("db2");
	fflush(stdout);
	return (0);
}

