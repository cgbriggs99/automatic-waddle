/*
 * input.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: cgbri
 */

#include <stdio.h>
#include <stdlib.h>

#include "../../compchem.hpp"

using namespace compchem;

/*
 * Get input from a file and place it in out.
 */
void compchem::input(class Molecule **out, FILE *fp) {
	int num;
	fscanf(fp, "%d", &num);

	//Because C++ does not like having friends for some reason.
	Atom *atoms = (Atom *) calloc(num, sizeof(compchem::Atom));
	size_t offset = 0;

	for(int i = 0; i < num; i++) {
		double n, x, y, z;
		fscanf(fp, "%lf %lf %lf %lf", &n, &x, &y, &z);
		Atom *atom = new (atoms + offset) Atom((int) n, x, y, z);
		offset += 1;
	}

	*out = new compchem::Molecule(atoms, num);
	return;
}

/*
 * Read in Hessian data.
 */
void compchem::inputHessian(class Molecule **out, FILE *fp) {
	int check;
	fscanf(fp, "%d", &check);
	if (check != (*out)->getNumatoms()) {
		perror("Error: Invalid sizes.\n");
		exit(-1);
	}

	for (int i = 0; i < (*out)->getHessianSize(); i++) {
		for (int j = 0; j < (*out)->getNumatoms(); j++) {
			fscanf(fp, "%lf %lf %lf", &((*((*out)->getHessian()))(i, 3 * j)),
				&((*((*out)->getHessian()))(i, 3 * j + 1)),
				&((*((*out)->getHessian()))(i, 3 * j + 2)));
		}
	}
	(*out)->harmonics();
}

/*
 * Read data from various files for the SCF procedure.
 */
void compchem::inputSCF(class Molecule **out, FILE *mol, FILE *enuc, FILE *s, FILE *t,
		FILE *v, FILE *eri, FILE *mux, FILE *muy, FILE *muz, FILE *hessian) {
	compchem::input(&(*out), &(*mol));
	double e;
	fscanf(enuc, "%lf", &e);
	(*out)->setEnuc(e);

	int i, j, k, l;
	while (!feof(s)) {
		fscanf(s, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(s, "%lf", &((*((*out)->getOverlap()))(i, j)));
		(*((*out)->getOverlap()))(j, i) = (*((*out)->getOverlap()))(i, j);
	}

	while (!feof(t)) {
		fscanf(t, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(t, "%lf", &((*((*out)->getKinetic()))(i, j)));
		(*((*out)->getKinetic()))(j, i) = (*((*out)->getKinetic()))(i, j);
	}

	while (!feof(v)) {
		fscanf(v, "%d %d", &i, &j);
		i--;
		j--;
		fscanf(v, "%lf", &((*((*out)->getAttraction()))(i, j)));
		(*((*out)->getAttraction()))(j, i) = (*((*out)->getAttraction()))(i, j);
	}

	while (!feof(eri)) {
		fscanf(eri, "%d %d %d %d", &i, &j, &k, &l);
		fscanf(eri, "%lf", &((*((*out)->getTwoElectron()))(i - 1, j - 1, k - 1, l - 1)));
	}

	while(!feof(mux)) {
		fscanf(mux, "%d %d", &i, &j);
		fscanf(mux, "%lf", &((*((*out)->getMux()))(i - 1, j - 1)));
		(*((*out)->getMux()))(j - 1, i - 1) = (*((*out)->getMux()))(i - 1, j - 1);
	}
	while(!feof(muy)) {
		fscanf(muy, "%d %d", &i, &j);
		fscanf(muy, "%lf", &((*((*out)->getMuy()))(i - 1, j - 1)));
		(*((*out)->getMuy()))(j - 1, i - 1) = (*((*out)->getMuy()))(i - 1, j - 1);
	}
	while(!feof(muz)) {
		fscanf(muz, "%d %d", &i, &j);
		fscanf(muz, "%lf", &((*((*out)->getMuz()))(i - 1, j - 1)));
		(*((*out)->getMuz()))(j - 1, i - 1) = (*((*out)->getMuz()))(i - 1, j - 1);
	}

	int check;
	fscanf(hessian, "%d", &check);
	if (check != (*out)->getNumatoms()) {
		perror("Error: Invalid sizes.\n");
		exit(-1);
	}

	for (int i = 0; i < (*out)->getHessianSize(); i++) {
		for (int j = 0; j < (*out)->getNumatoms(); j++) {
			fscanf(hessian, "%lf %lf %lf", &((*((*out)->getHessian()))(i, 3 * j)),
					&((*((*out)->getHessian()))(i, 3 * j + 1)),
					&((*((*out)->getHessian()))(i, 3 * j + 2)));
		}
	}
}
