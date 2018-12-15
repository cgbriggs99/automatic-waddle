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

void Molecule::computeMOTEI() {
	for(int p = 0; p < this->orbitals; p++) {
		for(int q = 0; q <= p; q++) {
			for(int r = 0; r <= p; r++) {
				for(int s = 0; s <= ((p == r)? q: r); s++) {
					this->mo_two_electron->subscript(p, q, r, s) = 0;
					for(int mu = 0; mu < this->orbitals; mu++) {
						for(int nu = 0; nu < this->orbitals; nu++) {
							for(int lam = 0; lam < this->orbitals; lam++) {
								for(int sig = 0; sig < this->orbitals; sig++) {
									this->mo_two_electron->subscript(p, q, r, s) +=
									        this->molecular_orbitals->subscript(
									            mu, p)
									            * this->molecular_orbitals
									                ->subscript(nu, q)
									            * this->molecular_orbitals
									                ->subscript(lam, r)
									            * this->molecular_orbitals
									                ->subscript(sig, s)
									            * this->two_electron->subscript(
									                mu, nu, lam, sig);
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
	this->mp2_correction = 0;
	for(int i = 0; i < this->occupied; i++) {
		for(int j = 0; j < this->occupied; j++) {
			for(int a = this->occupied; a < this->orbitals; a++) {
				for(int b = this->occupied; b < this->orbitals; b++) {
					this->mp2_correction += (this->mo_two_electron->subscript(i,
					    a, j, b)
					    * (2 * this->mo_two_electron->subscript(i, a, j, b)
					        - this->mo_two_electron->subscript(i, b, j, a)))
					    / (this->molecular_energies->subscript(i, i)
					        + this->molecular_energies->subscript(j, j)
					        - this->molecular_energies->subscript(a, a)
					        - this->molecular_energies->subscript(b, b));

				}
			}
		}
	}
	this->mp2_energy = this->scf_energy + this->mp2_correction;
}
