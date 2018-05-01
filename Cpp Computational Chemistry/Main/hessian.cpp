/*
 * hessian.cpp
 *
 *  Created on: Apr 30, 2018
 *      Author: Connor
 */

#include <stdio.h>
#include <stdlib.h>
#include "../compchem.hpp"

using namespace compchem;
using namespace array;
using namespace std;

int main(int argc, char **argv) {
	linkDLLs();
	Molecule *mol;
	initialize(&mol, argc, argv);
	mol->harmonics();
	printFreqs(mol);
	printRotor(mol);
	delete mol;
	unlinkDLLs();
	return (0);
}


