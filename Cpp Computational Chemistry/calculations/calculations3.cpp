/*
 * calculations3.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: Connor
 */

#ifndef CALCULATIONS3_CPP
#define CALCULATIONS3_CPP

#include "../compchem.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace compchem;
using namespace array;

static int comparedp(const void *a, const void *b) {
	if(**((double **) a) > **((double **) b)) {
		return (1);
	} else if(**((double **) a) < **((double **) b)) {
		return (-1);
	} else {
		return (0);
	}
}

static int compared(const void *a, const void *b) {
	if(*((double *) a) > *((double *) b)) {
		return (1);
	} else if(*((double *) a) < *((double *) b)) {
		return (-1);
	} else {
		return (0);
	}
}

void compchem::Molecule::computeSCF(bool print) {

	double *im = (double *) malloc(this->orbitals * sizeof(double));
	double curr_scf = 0, last_scf = 0;
	double curr_rms = 0, last_rms = 0;
	double energy = 0;
	int *pivs = (int *) calloc(this->orbitals, sizeof(int));
	int first = 1;
	double **adds = (double **) malloc(this->orbitals * sizeof(double *));
	double *temp = (double *) calloc(this->orbitals * this->orbitals,
	    sizeof(double));
	double *last_dens = (double *) calloc(this->orbitals * this->orbitals,
	    sizeof(double));
	size_t whole_mat = this->orbitals * this->orbitals * sizeof(double);
	int count = 0;

	for(int i = 0; i < this->orbitals; i++) {
		for(int j = 0; j < this->orbitals; j++) {
			(*(this->hamiltonian))(i, j) = (*(this->kinetic))(i, j)
			    + (*(this->attraction))(i, j);
		}
	}

	do {
		last_scf = curr_scf;
		last_rms = curr_rms;
		if(first) {
			memmove(temp, this->overlap->getArray(), whole_mat);
			LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', this->orbitals, temp,
			    this->orbitals, this->orthogonal->getArray(), im, NULL,
			    this->orbitals, this->orthogonal_eigvs->getArray(),
			    this->orbitals);
			for(int i = 0; i < this->orbitals; i++) {
				(*(this->orthogonal))(i, i) = 1
				    / sqrt((*(this->orthogonal))(0, i));
				if(i != 0) {
					(*(this->orthogonal))(0, i) = 0;
				}
			}

			//Create the orthogonalization matrix.
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
			    this->orbitals, this->orbitals, this->orbitals, 1,
			    this->orthogonal_eigvs->getArray(), this->orbitals,
			    this->orthogonal->getArray(), this->orbitals, 0,
			    this->orthogonal_t->getArray(), this->orbitals);

			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans, this->orbitals,
			    this->orbitals, this->orbitals, 1,
			    this->orthogonal_t->getArray(), this->orbitals,
			    this->orthogonal_eigvs->getArray(), this->orbitals, 0,
			    this->orthogonal->getArray(), this->orbitals);

			//Create the initial fock matrix.
			cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, this->orbitals,
			    this->orbitals, this->orbitals, 1, this->orthogonal->getArray(),
			    this->orbitals, this->hamiltonian->getArray(), this->orbitals,
			    0, this->orthogonal_eigvs->getArray(), this->orbitals);

			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
			    this->orbitals, this->orbitals, this->orbitals, 1,
			    this->orthogonal_eigvs->getArray(), this->orbitals,
			    this->orthogonal->getArray(), this->orbitals, 0, temp,
			    this->orbitals);

			if(print) {
				printArray((char *) "Overlap", this->overlap);
				printArray((char *) "Kinetic Energy", this->kinetic);
				printArray((char *) "Nuclear Attraction", this->attraction);
				printArray((char *) "Hamiltonian", this->hamiltonian);
				printArray((char *) "Orthogonalization", this->orthogonal);
				printArray((char *) "Initial F' Matrix",
				    new Array<double>(temp, 2, this->orbitals, this->orbitals));
			}
			memmove(this->fock->getArray(), temp, whole_mat);
		} else {
			cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, this->orbitals,
			    this->orbitals, this->orbitals, 1, this->orthogonal->getArray(),
			    this->orbitals, this->fock->getArray(), this->orbitals, 0,
			    this->orthogonal_eigvs->getArray(), this->orbitals);

			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
			    this->orbitals, this->orbitals, this->orbitals, 1,
			    this->orthogonal_eigvs->getArray(), this->orbitals,
			    this->orthogonal->getArray(), this->orbitals, 0, temp,
			    this->orbitals);
		}

		//Diagonalize the fock matrix.
		//memmove(temp, this->fock->getArray(), whole_mat);
		LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', this->orbitals, temp,
		    this->orbitals, this->molecular_energies->getArray(), im, NULL,
		    this->orbitals, this->molecular_orbitals->getArray(),
		    this->orbitals);

		//Sort values.
		for(int i = 0; i < this->orbitals; i++) {
			adds[i] = &(this->molecular_energies->getArray()[i]);
		}
		qsort(adds, this->orbitals, sizeof(double *), comparedp);
		qsort(this->molecular_energies->getArray(), this->orbitals,
		    sizeof(double), compared);

		//Sift through data.
		for(int i = 0; i < this->orbitals; i++) {
			union {
				void *pointer;
				unsigned long int integer;
			} addsp, arrp;
			addsp.pointer = adds[i];
			arrp.pointer = this->molecular_energies->getArray();
			unsigned long off = (addsp.integer - arrp.integer)
			    / sizeof(double *);
			for(int j = 0; j < this->orbitals; j++) {
				temp[j * this->orbitals + i] = (*(this->molecular_orbitals))(j,
				    off);
			}
		}
		memmove(this->molecular_orbitals->getArray(), temp, whole_mat);

		//Change the basis of the orbitals.
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, this->orbitals,
		    this->orbitals, this->orbitals, 1, this->orthogonal->getArray(),
		    this->orbitals, this->molecular_orbitals->getArray(),
		    this->orbitals, 0, this->orthogonal_t->getArray(), this->orbitals);
		if(print && first) {
			printArray((char *) "Initial C", this->orthogonal_t);
		}

		memmove(this->molecular_orbitals->getArray(),
		    this->orthogonal_t->getArray(), whole_mat);

		//Calculate the density matrix.
		for(int i = 0; i < this->orbitals; i++) {
			for(int j = 0; j < this->orbitals; j++) {
				(*(this->density))(i, j) = 0;
				for(int m = 0; m < this->occupied; m++) {
					(*(this->density))(i, j) += (*(this->orthogonal_t))(i, m)
					    * (*(this->orthogonal_t))(j, m);
				}
			}
		}

		if(print && first) {
			printArray((char *) "Initial Density", this->density);
		}

		curr_scf = 0;

		for(int i = 0; i < this->orbitals; i++) {
			for(int j = 0; j < this->orbitals; j++) {
				if(first) {
					curr_scf += (*(this->density))(i, j)
					    * (2 * (*(this->hamiltonian))(i, j));
				} else {
					curr_scf +=
					        (*(this->density))(i, j)
					            * ((*(this->fock))(i, j)
					                + (*(this->hamiltonian))(i, j));
				}
			}
		}

		energy = curr_scf + this->enuc;

		for(int i = 0; i < this->orbitals; i++) {
			for(int j = 0; j < this->orbitals; j++) {
				(*(this->fock))(i, j) = (*(this->hamiltonian))(i, j);
				for(int k = 0; k < this->orbitals; k++) {
					for(int l = 0; l < this->orbitals; l++) {
						(*(this->fock))(i, j) += (*(this->density))(k, l)
						    * (2 * (*(this->two_electron))(i, j, k, l)
						        - (*(this->two_electron))(i, k, j, l));
					}
				}
			}
		}

		curr_rms = 0;
		for(int i = 0; i < this->orbitals; i++) {
			for(int j = 0; j < this->orbitals; j++) {
				double hold = ((*(this->density))(i, j)
				    - last_dens[i * this->orbitals + j]);
				curr_rms += hold * hold;
			}
		}
		curr_rms = sqrt(curr_rms);

		if(first && print) {
			printArray((char *) "Second Fock matrix", this->fock);
			printf("Iter\tSCF Energy\tTotal Energy\tDelta E\t\tDelta RMS\n");
		}

		if(print) {
			printf("%d\t%.7f\t%.7f\t%.7f\t%.7f\n", count, curr_scf, energy,
			    curr_scf - last_scf, curr_rms - last_rms);
		}

		first = 0;
		count++;
	} while(fabs(curr_scf - last_scf) > this->scf_eps
	    && fabs(curr_rms - last_rms) > this->scf_eps);
	this->scf_energy = curr_scf + this->enuc;

	this->dipole[0] = 0;
	this->dipole[1] = 0;
	this->dipole[2] = 0;

	if(print) {
		printArray((char *) "Density", this->density);
	}

	for(int i = 0; i < this->orbitals; i++) {
		(*(this->molecular_energies))(i, i) = (*(this->molecular_energies))(0,
		    i);
		if(i != 0) {
			(*(this->molecular_energies))(0, i) = 0;
		}
	}

	for(int i = 0; i < this->orbitals; i++) {
		for(int j = 0; j < this->orbitals; j++) {
			this->dipole[0] += 2 * (*(this->density))(i, j)
			    * (*(this->mux))(i, j);
			this->dipole[1] += 2 * (*(this->density))(i, j)
			    * (*(this->muy))(i, j);
			this->dipole[2] += 2 * (*(this->density))(i, j)
			    * (*(this->muz))(i, j);
		}
	}

	for(int i = 0; i < this->numatoms; i++) {
		this->dipole += this->atoms[i].pos * this->atoms[i].num;
	}

	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, this->orbitals,
	    this->orbitals, this->orbitals, 1, this->density->getArray(),
	    this->orbitals, this->overlap->getArray(), this->orbitals, 0, temp,
	    this->orbitals);

	int curr = 0;

	for(int i = 0; i < this->numatoms; i++) {
		this->atoms[i].true_charge = this->atoms[i].num;
		for(int j = 0; j < compchem::orbitals(this->atoms[i].num); j++) {
			this->atoms[i].true_charge -= 2
			    * temp[curr * this->orbitals + curr];
			curr++;
		}
	}

	free(temp);
	free(last_dens);
	free(pivs);
	free(im);
	free(adds);

}

#endif
