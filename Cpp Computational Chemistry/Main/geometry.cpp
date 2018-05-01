/*
 * geometry.cpp
 *
 *  Created on: Apr 30, 2018
 *      Author: cgbri
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../compchem.hpp"

using namespace compchem;

int main(int argc, char **argv) {
	linkDLLs();
	compchem::Molecule *mol;
	initialize(&mol, argc, argv);
	mol->computeDists();
	mol->computeAngles();
	mol->computePlaneAngles();
	mol->computeTorsionAngles();
	mol->translateCOM();
	mol->momentsOfInertia();
	mol->rotations();
	printDists(mol);
	printAngles(mol);
	printMoments(mol);
	delete mol;
	unlinkDLLs();
	return (0);
}


