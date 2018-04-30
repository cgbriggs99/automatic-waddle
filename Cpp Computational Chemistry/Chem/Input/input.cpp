/*
 * input.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: cgbri
 */

#include "../../input.hpp"
#include "../../arrays.hpp"
#include "../../molecule.hpp"

using namespace compchem;

/*
 * Get input from a file and place it in out.
 */
void input(compchem::Molecule **out, FILE *fp) {
	int num;
	fscanf(fp, "%d", &num);

	Atom *atoms = new Atom[num];

	for (int i = 0; i < num; i++) {
		double x;
		atoms[i].charge = 0;
		fscanf(fp, "%lf %lf %lf %lf", &x, &(atoms[i].pos[0]),
		       &(atoms[i].pos[1]), &(atoms[i].pos[2]));
		atoms[i].num = x;
		atoms[i].mass = amu(atoms[i].num);
	}
	*out = new Molecule(atoms, num);
}

/*
 * Read in Hessian data.
 */
void inputHessian(compchem::Molecule **out, FILE *fp) {
	int check;
	fscanf(fp, "%d", &check);
	if (check != (*out)->numatoms) {
		perror("Error: Invalid sizes.\n");
		exit(-1);
	}

	for (int i = 0; i < (*out)->hessian_size; i++) {
		for (int j = 0; j < (*out)->numatoms; j++) {
			fscanf(fp, "%lf %lf %lf", &((*out)->hessian[i][3 * j]),
				&((*out)->hessian[i][3 * j + 1]),
				&((*out)->hessian[i][3 * j + 2]));
		}
	}
	(*out)->harmonics();
}

/*
 * Read data from various files for the SCF procedure.
 */
void inputSCF(compchem::Molecule **out, FILE *mol, FILE *enuc, FILE *s, FILE *t,
		FILE *v, FILE *eri, FILE *mux, FILE *muy, FILE *muz) {
	input(out, mol);
	fscanf(enuc, "%lf", &((*out)->enuc));

	int i, j, k, l;
	while (!feof(s)) {
		fscanf(s, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(s, "%lf", &((*((*out)->overlap))(i, j)));
		(*((*out)->overlap))(j, i) = (*((*out)->overlap))(i, j);
	}

	while (!feof(t)) {
		fscanf(t, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(t, "%lf", &((*((*out)->kinetic))(i, j)));
		(*((*out)->kinetic))(j, i) = (*((*out)->kinetic))(i, j);
	}

	while (!feof(v)) {
		fscanf(v, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(v, "%lf", &((*((*out)->attraction))(i, j)));
		(*((*out)->attraction))(j, i) = (*((*out)->attraction))(i, j);
	}

	while (!feof(eri)) {
		fscanf(eri, "%d %d %d %d", &i, &j, &k, &l);
		fscanf(eri, "%lf", &((*((*out)->two_electron))(i - 1, j - 1, k - 1, l - 1)));
	}

	while(!feof(mux)) {
		fscanf(mux, "%d %d", &i, &j);
		fscanf(mux, "%lf", &((*((*out)->mux))(i - 1, j - 1)));
		(*((*out)->mux))(j - 1, i - 1) = (*((*out)->mux))(i - 1, j - 1);
	}
	while(!feof(muy)) {
		fscanf(muy, "%d %d", &i, &j);
		fscanf(muy, "%lf", &((*((*out)->muy))(i - 1, j - 1)));
		(*((*out)->muy))(j - 1, i - 1) = (*((*out)->muy))(i - 1, j - 1);
	}
	while(!feof(muz)) {
		fscanf(muz, "%d %d", &i, &j);
		fscanf(muz, "%lf", &((*out)->muz[i - 1][j - 1]));
		(*((*out)->muz))(j - 1, i - 1) = (*((*out)->muz))(i - 1, j - 1);
	}
}
