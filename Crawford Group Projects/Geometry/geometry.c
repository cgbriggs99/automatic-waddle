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
#include "../linker.h"


int main(int argc, char **argv) {
	linkDLLs();
	FILE *fp;
	if(argc < 2) {
		fp = stdin;
	} else {
		fp = fopen(argv[1], "r");
	}

	molecule_t *mol;
	input(&mol, fp);
	printDists(mol);
	printAngles(mol);
	printMoments(mol);
	printRotor(mol);
	fclose(fp);
	deleteMolecule(mol);
	unlinkDLLs();
	return (0);
}

