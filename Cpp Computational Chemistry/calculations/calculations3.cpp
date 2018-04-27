/*
 * calculations3.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: Connor
 */

#include "../output.hpp"
#include "../arrays.hpp"
#include "../input.hpp"
#include "../molecule.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;
using namespace compchem;
using namespace array;

void Molecule::computeSCF() {

	double *im = (double *) malloc(this->electrons * sizeof(double));
	double curr_scf = 0, last_scf = 0;
	int *pivs = (int *) calloc(this->electrons, sizeof(int));
	int first = 1;

	do {
		last_scf = curr_scf;
		if (first) {
			LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', this->electrons,
					this->overlap->getArray(), this->electrons,
					this->orthogonal->getArray(), im, NULL, 1,
					this->orthogonal_eigvs->getArray(), this->electrons);
			for (int i = 0; i < this->electrons; i++) {
				(*(this->orthogonal))(i, i) = sqrt((*(this->orthogonal))(0, i));
				if (i != 0) {
					(*(this->orthogonal))(0, i) = 0;
				}
			}

			//Create the orthogonalization matrix.
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
					this->electrons, this->electrons, this->electrons, 1,
					this->orthogonal_eigvs->getArray(), this->electrons,
					this->orthogonal_t, this->electrons, 0, this->orthogonal,
					this->electrons);
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasTrans,
					this->electrons, this->electrons, this->electrons, 1,
					this->orthogonal_t->getArray(), this->electrons,
					this->orthogonal_eigvs->getArray(), this->electrons, 0,
					this->orthogonal->getArray(), this->electrons);

			//Create the initial fock matrix.
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
					this->electrons, this->electrons, this->electrons, 1,
					this->orthogonal->getArray(), this->electrons,
					this->hamiltonian->getArray(), this->electrons, 0,
					this->orthogonal_eigvs->getArray(), this->electrons);

			cblas_dcopy(this->electrons * this->electrons,
					this->orthogonal->getArray(), 1,
					this->orthogonal_t->getArray(), 1);

			LAPACKE_dgetrf(LAPACK_ROW_MAJOR, this->electrons, this->electrons,
					this->orthogonal_t->getArray(), this->electrons, pivs);
			LAPACKE_dgetri(LAPACK_ROW_MAJOR, this->electrons,
					this->orthogonal_t->getArray(), this->electrons, pivs);

			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
					this->electrons, this->electrons, this->electrons, 1,
					this->orthogonal_eigvs->getArray(), this->electrons,
					this->orthogonal_t->getArray(), this->electrons, 0,
					this->fock->getArray(), this->electrons);
			first = 0;
		} else {
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
					this->electrons, this->electrons, this->electrons, 1,
					this->orthogonal->getArray(), this->electrons,
					this->fock->getArray(), this->electrons, 0,
					this->orthogonal_eigvs->getArray(), this->electrons);

			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
					this->electrons, this->electrons, this->electrons, 1,
					this->orthogonal_eigvs->getArray(), this->electrons,
					this->orthogonal_t->getArray(), this->electrons, 0,
					this->fock->getArray(), this->electrons);
		}

		//Diagonalize the fock matrix.
		LAPACKE_dgeev(LAPACK_ROW_MAJOR, 'N', 'V', this->electrons,
				this->fock->getArray(), this->electrons,
				this->molecular_energies->getArray(), im, NULL, 1,
				this->molecular_orbitals->getArray(), this->electrons);

		//Change the basis of the orbitals.
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, this->electrons,
				this->electrons, this->electrons, 1,
				this->orthogonal_t->getArray(), this->electrons,
				this->molecular_orbitals->getArray(), this->electrons, 0,
				this->molecular_energies->getArray(), this->electrons);

		//Calculate the density matrix.
		for (int i = 0; i < this->electrons; i++) {
			for (int j = 0; j < this->electrons; j++) {
				(*(this->density))(i, j) = 0;
				for (int m = 0; m < this->occupied; m++) {
					(*(this->density))(i, j) += (*(this->molecular_energies))(i,
							m) * (*(this->molecular_energies))(j, m);
				}
			}
		}

		curr_scf = 0;

		for (int i = 0; i < this->electrons; i++) {
			for (int j = 0; j < this->electrons; j++) {
				curr_scf +=
						(*(this->density))(i, j)
								* ((*(this->fock))(i, j)
										+ (*(this->hamiltonian))(i, j));
			}
		}

		curr_scf += this->enuc;

		for (int i = 0; i < this->electrons; i++) {
			for (int j = 0; j < this->electrons; j++) {
				(*(this->fock))(i, j) = (*(this->hamiltonian))(i, j);
				for (int k = 0; k < this->electrons; k++) {
					for (int l = 0; l < this->electrons; l++) {
						(*(this->fock))(i, j) += (*(this->density))(k, l)
								* (2 * (*(this->two_electron))(i, j, k, l)
										- (*(this->two_electron))(i, k, j, l));
					}
				}
			}
		}
	} while (fabs(curr_scf - last_scf) > this->scf_eps);
}

