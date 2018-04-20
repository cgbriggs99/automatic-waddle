/* main2.c
 * Copyright (C) ${year} Connor Briggs
 */
/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
/*
 * I can be reached at cgbriggs99@gmail.com if you have any questions,
 * concerns, or bugs.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "../calculations.h"
#include "../molecule.h"
#include "../output.h"
#include "../input.h"

int main(int argc, char **argv) {
	FILE *mol, *hes;

	molecule_t *molecule;
	if(argc < 2) {
		input(&molecule, stdin);
		inputHessian(&molecule, stdin);
	} else if(argc == 2){
		mol = fopen(argv[1], "r");
		hes = stdin;
		input(&molecule, mol);
		inputHessian(&molecule, hes);
	} else {
		mol = fopen(argv[1], "r");
		hes = fopen(argv[2], "r");
		input(&molecule, mol);
		inputHessian(&molecule, hes);
	}


	printEigenvalues(molecule);
	printFreqs(molecule);
	deleteMolecule(molecule);
	if(argc >= 2) {
		fclose(mol);
	}
	if(argc == 2) {
		fclose(hes);
	}
	return (0);
}
