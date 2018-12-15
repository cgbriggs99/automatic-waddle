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
#include <lapacke.h>
#include <cblas.h>

using namespace std;
using namespace compchem;

static int comparedp(const void *a, const void *b) {
	if (**((double **) a) > **((double **) b)) {
		return (1);
	} else if (**((double **) a) < **((double **) b)) {
		return (-1);
	} else {
		return (0);
	}
}

void compchem::Molecule::computeSCF(bool print) {

//	double *im = (double *) malloc(this->orbitals * sizeof(double));
	FortranArray<double> im(this->orbitals);
	double curr_scf = 0, last_scf = 0;
	double curr_rms = 0, last_rms = 0;
	double energy = 0;
	int *pivs = (int *) calloc(this->orbitals, sizeof(int));
	int first = 1;
	double **adds = (double **) malloc(this->orbitals * sizeof(double *));
	compchem::FortranArray<double> temp(
			Dimension(this->orbitals, this->orbitals));
//	double *last_dens = (double *) calloc(this->orbitals * this->orbitals,
//	    sizeof(double));
	FortranArray<double> last_dens(Dimension(this->orbitals, this->orbitals));
	size_t whole_mat = this->orbitals * this->orbitals * sizeof(double);
	int count = 0;

//	for(int i = 0; i < this->orbitals; i++) {
//		for(int j = 0; j < this->orbitals; j++) {
//			this->hamiltonian(i, j) = this->kinetic(i, j)
//			    + (*(this->attraction))(i, j);
//		}
//	}

	(*(this->hamiltonian)) = *(this->kinetic) + *(this->attraction);

	do {
		last_scf = curr_scf;
		last_rms = curr_rms;
		if (first) {

			//Find the diagonalization of the orthogonalization matrix.
			compchem::eigenval_compute(false, true, *(this->overlap),
					this->orthogonal, nullptr, nullptr, this->orthogonal_eigvs);

			for (int i = 0; i < this->orbitals; i++) {
				this->orthogonal->subscript(i, i) = 1
						/ sqrt(this->orthogonal->subscript(0, i));
				if (i != 0) {
					this->orthogonal->subscript(0, i) = 0;
				}
			}

			//Create the orthogonalization matrix.
			*(this->orthogonal) = compchem::matmult(*(this->orthogonal_eigvs),
					*(this->orthogonal));
			*(this->orthogonal) = compchem::matmult(*(this->orthogonal),
					this->orthogonal_eigvs->transpose());

			//Create the initial fock matrix.
			*(this->fock) = compchem::matmult(
					this->orthogonal_eigvs->transpose(), *(this->hamiltonian));
			*(this->fock) = compchem::matmult(*(this->fock),
					*(this->orthogonal_eigvs));

			if (print) {
				printArray((char *) "Overlap", *this->overlap);
				printArray((char *) "Kinetic Energy", *this->kinetic);
				printArray((char *) "Nuclear Attraction", *this->attraction);
				printArray((char *) "Hamiltonian", *this->hamiltonian);
				printArray((char *) "Orthogonalization", *this->orthogonal);
				printArray((char *) "Initial F' Matrix", temp);
			}
			*this->fock = temp;
//			memmove(this->fock->getArray(), temp, whole_mat);
		} else {
			*(this->fock) = compchem::matmult(
					this->orthogonal_eigvs->transpose(), *(this->fock));
			*(this->fock) = compchem::matmult(*(this->fock),
					*(this->orthogonal_eigvs));
		}

		//Diagonalize the fock matrix.
		compchem::eigenval_compute(false, true, *(this->fock),
				this->molecular_energies, nullptr, nullptr,
				this->molecular_orbitals);

		//Sort values.
		for (int i = 0; i < this->orbitals; i++) {
			adds[i] = &(this->molecular_energies->subscript(i));
		}
		qsort(adds, this->orbitals, sizeof(double *), comparedp);
		this->molecular_energies->sort();

		//Sift through data.
		for (int i = 0; i < this->orbitals; i++) {
			union {
				const void *pointer;
				unsigned long int integer;
			} addsp, arrp;
			addsp.pointer = adds[i];
			arrp.pointer = (const void *) this->molecular_energies->getData();
			unsigned long off = (addsp.integer - arrp.integer)
					/ sizeof(double *);
			for (int j = 0; j < this->orbitals; j++) {
				temp(j * this->orbitals + i) =
						this->molecular_orbitals->subscript(j, off);
			}
		}
//		memmove(this->molecular_orbitals->getArray(), temp, whole_mat);
		*this->molecular_orbitals = temp;
		//Change the basis of the orbitals.
		*(this->orthogonal_t) = compchem::matmult(*(this->orthogonal), *(this->molecular_orbitals));
		if (print && first) {
			printArray((char *) "Initial C", *this->orthogonal_t);
		}

//		memmove(this->molecular_orbitals->getArray(),
//		    this->orthogonal_t->getArray(), whole_mat);

		*this->molecular_orbitals = *this->orthogonal_t;

		//Calculate the density matrix.
		for (int i = 0; i < this->orbitals; i++) {
			for (int j = 0; j < this->orbitals; j++) {
				this->density->subscript(i, j) = 0;
				for (int m = 0; m < this->occupied; m++) {
					this->density->subscript(i, j) +=
							this->orthogonal_t->subscript(i, m)
									* this->orthogonal_t->subscript(j, m);
				}
			}
		}

		if (print && first) {
			printArray((char *) "Initial Density", *this->density);
		}

		curr_scf = 0;

		for (int i = 0; i < this->orbitals; i++) {
			for (int j = 0; j < this->orbitals; j++) {
				if (first) {
					curr_scf += this->density->subscript(i, j)
							* (2 * this->hamiltonian->subscript(i, j));
				} else {
					curr_scf += this->density->subscript(i, j)
							* (this->fock->subscript(i, j)
									+ this->hamiltonian->subscript(i, j));
				}
			}
		}

		energy = curr_scf + this->enuc;

		*this->fock = *this->hamiltonian;

		for (int i = 0; i < this->orbitals; i++) {
			for (int j = 0; j < this->orbitals; j++) {
				for (int k = 0; k < this->orbitals; k++) {
					for (int l = 0; l < this->orbitals; l++) {
						this->fock->subscript(i, j) += this->density->subscript(
								k, l)
								* (2 * this->two_electron->subscript(i, j, k, l)
										- this->two_electron->subscript(i, k, j,
												l));
					}
				}
			}
		}

		curr_rms = 0;
		for (int i = 0; i < this->orbitals; i++) {
			for (int j = 0; j < this->orbitals; j++) {
				double hold = (this->density->subscript(i, j) - last_dens(i, j));
				curr_rms += hold * hold;
			}
		}
		curr_rms = sqrt(curr_rms);

		if (first && print) {
			printArray((char *) "Second Fock matrix", *this->fock);
			printf("Iter\tSCF Energy\tTotal Energy\tDelta E\t\tDelta RMS\n");
		}

		if (print) {
			printf("%d\t%.7f\t%.7f\t%.7f\t%.7f\n", count, curr_scf, energy,
					curr_scf - last_scf, curr_rms - last_rms);
		}

		first = 0;
		count++;
		last_dens = *this->density;
	} while (fabs(curr_scf - last_scf) > this->scf_eps
			&& fabs(curr_rms - last_rms) > this->scf_eps);
	this->scf_energy = curr_scf + this->enuc;

	this->dipole->subscript(0) = 0;
	this->dipole->subscript(1) = 0;
	this->dipole->subscript(2) = 0;

	if (print) {
		printArray((char *) "Density", *this->density);
	}

	for (int i = 0; i < this->orbitals; i++) {
		this->molecular_energies->subscript(i, i) =
				this->molecular_energies->subscript(0, i);
		if (i != 0) {
			this->molecular_energies->subscript(0, i) = 0;
		}
	}

	for (int i = 0; i < this->orbitals; i++) {
		for (int j = 0; j < this->orbitals; j++) {
			this->dipole->subscript(0) += 2 * this->density->subscript(i, j)
					* this->mux->subscript(i, j);
			this->dipole->subscript(1) += 2 * this->density->subscript(i, j)
					* this->muy->subscript(i, j);
			this->dipole->subscript(2) += 2 * this->density->subscript(i, j)
					* this->muz->subscript(i, j);
		}
	}

	for (int i = 0; i < this->numatoms; i++) {
		*this->dipole += this->atoms[i].getPos() * (double) this->atoms[i].num;
	}

	temp = compchem::matmult(*(this->density), *(this->overlap));

	int curr = 0;

	for (int i = 0; i < this->numatoms; i++) {
		this->atoms[i].true_charge = this->atoms[i].num;
		for (int j = 0; j < compchem::orbitals(this->atoms[i].num); j++) {
			this->atoms[i].true_charge -= 2 * temp(curr, curr);
			curr++;
		}
	}
	free(pivs);
	free(adds);

}

#endif
