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
void compchem::input(compchem::Molecule **out, FILE *fp) {
	int num;
	int ignore = fscanf(fp, "%d", &num);

	std::vector<Atom> atoms;

	for(int i = 0; i < num; i++) {
		double n, x, y, z;
		ignore = fscanf(fp, "%lf %lf %lf %lf", &n, &x, &y, &z);
		atoms.push_back(Atom(n, x, y, z));
	}

	*out = new compchem::Molecule(atoms, num);
	return;
}

/*
 * Read in Hessian data.
 */
void compchem::inputHessian(compchem::Molecule **out, FILE *fp) {
	int check;
	int ignore = fscanf(fp, "%d", &check);
	if(check != (*out)->getNumatoms()) {
		perror("Error: Invalid sizes.\n");
		exit(-1);
	}

	for(int i = 0; i < (*out)->getHessianSize(); i++) {
		for(int j = 0; j < (*out)->getNumatoms(); j++) {
			double a, b, c;
			ignore = fscanf(fp, "%lf %lf %lf", &a, &b, &c);
			(*out)->getHessian()(i, 3 * j) = a;
			(*out)->getHessian()(i, 3 * j + 1) = b;
			(*out)->getHessian()(i, 3 * j + 2) = c;
		}
	}
	(*out)->harmonics();
}

/*
 * Read data from various files for the SCF procedure.
 */
void compchem::inputSCF(compchem::Molecule **out, FILE *mol, FILE *enuc, FILE *s,
                        FILE *t, FILE *v, FILE *eri, FILE *mux, FILE *muy,
                        FILE *muz, FILE *hessian) {
	compchem::input(&(*out), &(*mol));
	double e;
	int ignore = fscanf(enuc, "%lf", &e);
	(*out)->setEnuc(e);

	int i, j, k, l;
	while(!feof(s)) {
		ignore = fscanf(s, "%d %d", &i, &j);
		i--;
		j--;
		double a;
		ignore = fscanf(s, "%lf", &a);
		(*out)->getOverlap()(i, j) = a;
		(*out)->getOverlap()(j, i) = (*out)->getOverlap()(i, j);
	}

	while(!feof(t)) {
		ignore = fscanf(t, "%d %d", &i, &j);
		i--;
		j--;
		double a;
		ignore = fscanf(t, "%lf", &a);
		(*out)->getKinetic()(i, j) = a;
		(*out)->getKinetic()(j, i) = (*out)->getKinetic()(i, j);
	}

	while(!feof(v)) {
		ignore = fscanf(v, "%d %d", &i, &j);
		i--;
		j--;
		double a;
		ignore = fscanf(v, "%lf", &a);
		(*out)->getAttraction()(i, j) = a;
		(*out)->getAttraction()(j, i) = (*out)->getAttraction()(i, j);
	}

	while(!feof(eri)) {
		ignore = fscanf(eri, "%d %d %d %d", &i, &j, &k, &l);
		double a;
		ignore = fscanf(eri, "%lf", &a);
		    (*out)->getTwoElectron()(i - 1, j - 1, k - 1, l - 1) = a;
	}

	while(!feof(mux)) {
		ignore = fscanf(mux, "%d %d", &i, &j);
		double a;
		ignore = fscanf(mux, "%lf", &a);
		(*out)->getMux()(i - 1, j - 1) = a;
		(*out)->getMux()(j - 1, i - 1) = (*out)->getMux()(i - 1, j - 1);
	}
	while(!feof(muy)) {
		ignore = fscanf(muy, "%d %d", &i, &j);
		double a;
		ignore = fscanf(muy, "%lf", &a);
		(*out)->getMuy()(i - 1, j - 1) = a;
		(*out)->getMuy()(j - 1, i - 1) = (*out)->getMuy()(i - 1, j - 1);
	}
	while(!feof(muz)) {
		ignore = fscanf(muz, "%d %d", &i, &j);
		double a;
		ignore = fscanf(muz, "%lf", &a);
		(*out)->getMuz()(i - 1, j - 1) = a;
		(*out)->getMuz()(j - 1, i - 1) = (*out)->getMuz()(i - 1, j - 1);
	}

	int check;
	ignore = fscanf(hessian, "%d", &check);
	if(check != (*out)->getNumatoms()) {
		perror("Error: Invalid sizes.\n");
		exit(-1);
	}

	for(int i = 0; i < (*out)->getHessianSize(); i++) {
		for(int j = 0; j < (*out)->getNumatoms(); j++) {
			double a, b, c;
			ignore = fscanf(hessian, "%lf %lf %lf", &a, &b, &c);
			(*out)->getHessian()(i, 3 * j) = a;
			(*out)->getHessian()(i, 3 * j + 1) = b;
			(*out)->getHessian()(i, 3 * j + 2) = c;
		}
	}
}
