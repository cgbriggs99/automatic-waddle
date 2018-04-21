/*
 * input.c
 *
 *  Created on: Sep 12, 2017
 *      Author: connor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../molecule.h"
#include "../input.h"
#include "../calculations.h"

void input(molecule_t **out, FILE *in) {
	int num;
	if(in == stdin) {
		int l = scanf("%d", &num);
		printf("%d", l);
		fflush(stdout);
	} else {
		fscanf(in, "%d", &num);
	}

	atom_t *atoms = calloc(num, sizeof(atom_t));

	for (int i = 0; i < num; i++) {
		double x;
		atoms[i].charge = 0;
		atoms[i].id = i;
		if(in == stdin) {
			scanf("%lf %lf %lf %lf", &x, &(atoms[i].pos[0]),
				&(atoms[i].pos[1]), &(atoms[i].pos[2]));
		} else {
			fscanf(in, "%lf %lf %lf %lf", &x, &(atoms[i].pos[0]),
				&(atoms[i].pos[1]), &(atoms[i].pos[2]));
		}
		atoms[i].z = x;
		atoms[i].mass = amu(atoms[i].z);
	}
	*out = newMolecule(atoms, num);
	free(atoms);
}

void inputHessian(molecule_t **out, FILE *in) {
	int check;
	if (in == stdin) {
		scanf("%d", &check);
	} else {
		fscanf(in, "%d", &check);
	}
	if (check != (*out)->num_atoms) {
		perror("Error: Invalid sizes.\n");
		exit(-1);
	}

	for (int i = 0; i < (*out)->num_hessian; i++) {
		for (int j = 0; j < (*out)->num_atoms; j++) {
			fscanf(in, "%lf %lf %lf", &((*out)->hessian[i][3 * j]),
					&((*out)->hessian[i][3 * j + 1]),
					&((*out)->hessian[i][3 * j + 2]));
		}
	}
	weightHessian(*out);
	eigensHessian(*out);
}

void inputSCF(molecule_t **out, FILE *mol, FILE *enuc, FILE *s, FILE *t,
		FILE *v, FILE *eri, FILE *mux, FILE *muy, FILE *muz) {
	input(out, mol);
	fscanf(enuc, "%lf", &((*out)->enuc));

	int i, j, k, l;
	while (!feof(s)) {
		fscanf(s, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(s, "%lf", &((*out)->overlap[i][j]));
		(*out)->overlap[j][i] = (*out)->overlap[i][j];
	}

	while (!feof(t)) {
		fscanf(t, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(t, "%lf", &((*out)->kin_energy[i][j]));
		(*out)->kin_energy[j][i] = (*out)->kin_energy[i][j];
	}

	while (!feof(v)) {
		fscanf(v, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(v, "%lf", &((*out)->attraction[i][j]));
		(*out)->attraction[j][i] = (*out)->attraction[i][j];
	}

	while (!feof(eri)) {
		fscanf(eri, "%d %d %d %d", &i, &j, &k, &l);
		fscanf(eri, "%lf", &((*out)->two_electron[TEI(i - 1, j - 1, k - 1, l - 1)]));
	}

	while(!feof(mux)) {
		fscanf(mux, "%d %d", &i, &j);
		fscanf(mux, "%lf", &((*out)->mux[i - 1][j - 1]));
		(*out)->mux[j - 1][i - 1] = (*out)->mux[i - 1][j - 1];
	}
	while(!feof(muy)) {
		fscanf(muy, "%d %d", &i, &j);
		fscanf(muy, "%lf", &((*out)->muy[i - 1][j - 1]));
		(*out)->muy[j - 1][i - 1] = (*out)->muy[i - 1][j - 1];
	}
	while(!feof(muz)) {
		fscanf(muz, "%d %d", &i, &j);
		fscanf(muz, "%lf", &((*out)->muz[i - 1][j - 1]));
		(*out)->muz[j - 1][i - 1] = (*out)->muz[i - 1][j - 1];
	}


}
