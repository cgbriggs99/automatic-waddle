/*
 * molecule.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#include "../../molecule.hpp"

#ifdef __ELF__
#include <sys/mman.h>
#else
#include <windows.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

//Replace with later C++ headers.
#include "../../input.h"
#include "../../output.h"
#include "../../linker.h"
#include "../../calculations.h"

using namespace std;
using namespace compchem;

static int index_(int i, int j) {
	if(i >= j) {
		return ((i * (i + 1)) / 2 + j);
	} else {
		return ((j * (j + 1)) / 2 + i);
	}
}

int TEI(int mu, int nu, int lam, int sig) {
	int ij, kl;

	ij = index_(mu, nu);
	kl = index_(lam, sig);

	return (index_(ij, kl));
}


double amu(int z) {
	/*
	 * Atomic mass data retrieved from the Israel Science and Technology Directory.
	 * Due to copyright restrictions, this code is for educational use only.
	 * Reproduced under "fair use."
	 *
	 * List of Elements of the Periodic Table - Sorted by Atomic number. (n.d.).
	 * 	Retrieved September 12, 2017, from https://www.science.co.il/elements/
	 */
	double amus[] = { 1.0079, 4.0026, 6.941, 9.0122, 10.811, 12.0107, 14.0067,
	                  15.9994, 18.9984, 20.1797, 22.9897, 24.305, 26.9815,
	                  28.0855, 30.9738, 32.065, 35.453, 39.948, 39.0983, 40.078,
	                  44.9559, 47.867, 50.9415, 51.9961, 54.938, 55.845,
	                  58.9332, 58.6934, 63.546, 65.39, 69.723, 72.64, 74.9216,
	                  78.96, 79.904, 83.8, 85.4678, 87.62, 88.9059, 91.224,
	                  92.9064, 95.94, 98, 101.07, 102.9055, 106.42, 107.8682,
	                  112.411, 114.818, 118.71, 121.76, 127.6, 126.9045,
	                  131.293, 132.9055, 137.327, 138.9055, 140.116, 140.9077,
	                  144.24, 145, 150.36, 151.964, 157.25, 158.9253, 162.5,
	                  164.9303, 167.259, 168.9342, 173.04, 174.967, 178.49,
	                  180.9479, 183.84, 186.207, 190.23, 192.217, 195.078,
	                  196.9665, 200.59, 204.3833, 207.2, 208.9804, 209, 210,
	                  222, 223, 226, 227, 232.0381, 231.0359, 238.0289, 237,
	                  244, 243, 247, 247, 251, 252, 257, 258, 259, 262, 261,
	                  262, 266, 264, 277, 268 };
	return (amus[z - 1]);
}

int valence(int z) {
	int i = 1, s = z;
	;
	while(s > (i / 2) * (i / 2) * 2) {
		s -= (i / 2) * (i / 2) * 2;
	}
	return (s);
}

int orbitals(int z) {
	//Deal with exceptions.
	switch(z) {
	case 64:
	case 71:
		return (14);
	case 91:
	case 92:
	case 93:
	case 97:
	case 103:
		return (18);
	}

	if(z == 1 || z == 2) {
		return (1);
	}
	if(z >= 3 && z <= 10) {
		return (5);
	}
	if(z >= 11 && z <= 18) {
		return (9);
	}
	if(z == 19 || z == 20) {
		return (10);
	}
	if(z >= 21 && z <= 30) {
		return (15);
	}
	if(z >= 31 && z <= 36) {
		return (8);
	}
	if(z == 37 || z == 38) {
		return (9);
	}
	if(z >= 39 && z <= 48) {
		return (10);
	}
	if(z >= 49 && z <= 54) {
		return (11);
	}
	if(z == 55 || z == 56) {
		return (12);
	}
	if(z >= 57 && z <= 71) {
		return (13);
	}
	if(z >= 72 && z <= 80) {
		return (14);
	}
	if(z >= 81 && z <= 86) {
		return (15);
	}
	if(z == 87 || z == 88) {
		return (16);
	}
	if(z >= 89 && z <= 103) {
		return (17);
	}
	return (0);
}
