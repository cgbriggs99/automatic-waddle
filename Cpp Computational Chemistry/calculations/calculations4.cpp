/*
 * calculations4.cpp
 *
 *  Created on: Apr 30, 2018
 *      Author: Connor
 */

#include "../compchem.hpp"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

using namespace std;
using namespace compchem;
using namespace array;

void Molecule::computeMOTEI() {
	for (int i = 0, ijkl = 0; i < this->orbitals; i++) {
		for (int j = 0; j <= i; j++) {
			for (int k = 0; k <= i; k++) {
				for (int l = 0; l <= ((i == k)? j: k); l++, ijkl++) {
					(*(this->mo_two_electron))(i, j, k, l) = 0;
					for (int m = 0; m < this->orbitals; m++) {
						for (int n = 0; n < this->orbitals; n++) {
							for (int o = 0; o < this->orbitals; o++) {
								for (int p = 0; p < this->orbitals; p++) {
									this->mo_two_electron->getArray()[ijkl] +=
											(*(this->density))(m, i)
													* (*(this->density))(n, j)
													* (*(this->density))(o, k)
													* (*(this->density))(p, l)
													* (*(this->two_electron))(m,
															n, o, p);
								}
							}
						}
					}
				}
			}
		}
	}
}

void Molecule::computeMP2() {
	printArray("Energies", this->molecular_energies);
	printArray("Orbitals", this->molecular_orbitals);

	int fd = open("mo_two_electron.dat", O_CREAT | O_RDWR, 0777);
	close(fd);
	FILE *fp = fopen("mo_two_electron.dat", "r+");

	for(int i = 0; i < this->orbitals; i++) {
		for(int j = 0; j <= i; j++) {
			for(int k = 0; k <= i; k++) {
				for(int l = 0; l <= ((i == k)? j: k); l++) {
					fprintf(fp, "%d\t%d\t%d\t%d\t%.7f\n", i, j, k, l, (double) (*(this->mo_two_electron))(i, j, k, l));
				}
			}
		}
	}

	fclose(fp);

	this->mp2_correction = 0;
	for (int i = 0; i < this->occupied; i++) {
		for (int j = 0; j < this->occupied; j++) {
			for (int a = this->occupied; a < this->orbitals; a++) {
				for (int b = this->occupied; b < this->orbitals; b++) {
					this->mp2_correction += ((*(this->mo_two_electron))(i, a, j,
							b)
							* (2 * (*(this->mo_two_electron))(i, a, j, b)
									- (*(this->mo_two_electron))(i, b, j, a)))
							/ ((*(this->molecular_energies))(i, i)
									+ (*(this->molecular_energies))(j, j)
									- (*(this->molecular_energies))(a, a)
									- (*(this->molecular_energies))(b, b));

				}
			}
		}
	}
	this->mp2_energy = this->scf_energy + this->mp2_correction;
}
