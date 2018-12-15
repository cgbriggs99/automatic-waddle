/*
 * output.cpp
 *
 *  Created on: Apr 27, 2018
 *      Author: cgbri
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include "../../compchem.hpp"

using namespace compchem;

void compchem::printDists(Molecule &molecule) {
	puts("Distances");
	for(int i = 0; i < molecule.getNumatoms(); i++) {
		for(int j = 0; j < i; j++) {
			printf("%d\t%d\t%8.5f\n", i, j, molecule.getDistances()(i, j));
		}
	}
	fflush(stdout);
}

void compchem::printAngles(Molecule &molecule) {
	puts("\nBond angles");
	for(int i = 0; i < molecule.getNumatoms(); i++) {
		for(int j = 0; j < molecule.getNumatoms(); j++) {
			for(int k = 0; k < molecule.getNumatoms(); k++) {
				if(i == j || i == k || j == k) {
					continue;
				}
				if(molecule.getDistances()(i, k) > 4.0
				    || molecule.getDistances()(j, k) > 4.0) {
					continue;
				}
				printf("%d\t%d\t%d\t%3.4f\n", i, j, k,
				    180 * molecule.getAngles()(i, j, k) / M_PI);
			}
		}
	}

	puts("\nPlane angles");
	for(int i = 0; i < molecule.getNumatoms(); i++) {
		for(int j = 0; j < i; j++) {
			for(int k = 0; k < j; k++) {
				for(int l = 0; l < k; l++) {
					if(i == j || i == k || i == l || j == k || j == l
					    || k == l) {
						continue;
					}
					if(molecule.getDistances()(i, k) > 4.0
					    || molecule.getDistances()(j, k) > 4.0
					    || molecule.getDistances()(k, l) > 4.0) {
						continue;
					}
					printf("%d\t%d\t%d\t%d\t%3.4f\n", i, j, k, l,
					    180 * molecule.getPlaneAngles()(i, j, k, l) / M_PI);
				}
			}
		}
	}

	puts("\nTorsion angles");
	for(int i = 0; i < molecule.getNumatoms(); i++) {
		for(int j = 0; j < i; j++) {
			for(int k = 0; k < j; k++) {
				for(int l = 0; l < k; l++) {
					if(i == j || i == k || i == l || j == k || j == l
					    || k == l) {
						continue;
					}
					if(molecule.getDistances()(i, j) > 4.0
					    || molecule.getDistances()(j, k) > 4.0
					    || molecule.getDistances()(k, l) > 4.0) {
						continue;
					}
					printf("%d\t%d\t%d\t%d\t%3.4f\n", i, j, k, l,
					    180 * molecule.getTorsionAngles()(i, j, k, l) / M_PI);
				}
			}
		}
	}
	fflush(stdout);
}

void compchem::printMoments(Molecule &molecule) {
	puts("\nCenter of Mass");
	printf("%f %f %f\n", molecule.getCenterOfMass()(0),
	    molecule.getCenterOfMass()(1), molecule.getCenterOfMass()(2));

	puts("\nMoments of Inertia");
	for(int i = 0; i < 3; i++) {
		printf("%5.5f %5.5f %5.5f\n", molecule.getInertialMoments()(i, 0),
		    molecule.getInertialMoments()(i, 1),
		    molecule.getInertialMoments()(i, 2));
	}

	puts("\nPrimary moments (amu bohr^2)");
	printf("%5.5f %5.5f %5.5f\n", molecule.getPrincipleMoments()(0),
	    molecule.getPrincipleMoments()(1), molecule.getPrincipleMoments()(2));
	puts("\nPrimary moments (amu \u00e5^2)");
	printf("%5.5f %5.5f %5.5f\n", molecule.getPrincipleMoments()(0) * .2800285,
	    molecule.getPrincipleMoments()(1) * .2800285,
	    molecule.getPrincipleMoments()(2) * .2800285);
	puts("\nPrimary moments (g cm^2)");
	printf("%5.5e %5.5e %5.5e\n",
	    1.660468e-24 * molecule.getPrincipleMoments()(0)
	        * .00000000000000002800285,
	    1.660468e-24 * molecule.getPrincipleMoments()(1)
	        * .00000000000000002800285,
	    1.660468e-24 * molecule.getPrincipleMoments()(2)
	        * .00000000000000002800285);

	puts("\nRotational Constants (1/cm)");
	printf("%f %f %f\n", molecule.getRotationalConstants()(0),
	    molecule.getRotationalConstants()(1),
	    molecule.getRotationalConstants()(2));

	puts("\nRotational Constants (MHz)");
	printf("%f %f %f\n", 29979.2458 * molecule.getRotationalConstants()(0),
	    29979.2458 * molecule.getRotationalConstants()(1),
	    29979.2458 * molecule.getRotationalConstants()(2));
	fflush(stdout);
}

void compchem::printRotor(Molecule &molecule) {
	puts("\nRotor type");
	switch(molecule.getRotor()) {
	case Molecule::SPHERICAL_TOP:
		puts("Spherical top");
		break;
	case Molecule::LINEAR:
		puts("Linear");
		break;
	case Molecule::SYMMETRIC:
		puts("Symmetric");
		break;
	default:
		puts("Asymmetric");
	}
	fflush(stdout);
}

void compchem::printEigenvalues(Molecule &molecule) {
	puts("\nHessian eigenvalues.");
	for(int i = molecule.getHessianSize() - 1; i >= 0; i--) {
		printf("%d\t%f\n", i, molecule.getHessianEigs()(i));
	}
	fflush(stdout);
}

void compchem::printFreqs(Molecule &molecule) {
	puts("\nHarmonics.");
	for(int i = molecule.getHessianSize() - 1; i >= 0; i--) {
		printf("%d\t%f\n", i,
		    sqrt(molecule.getHessianEigs()(i)) * 5140.697669352);
	}
	fflush(stdout);
}

void compchem::printSCF(compchem::Molecule &molecule) {
	puts("\nFock matrix");
	for(int i = 0; i < molecule.getOrbitals(); i++) {
		for(int j = 0; j < molecule.getOrbitals(); j++) {
			printf("%f\t", molecule.getFock()(i, j));
		}
		printf("\n");
	}
	printf("\n");

	printArray("\nMux\n", molecule.getMux());
	printArray("\nMuy\n", molecule.getMuy());
	printArray("\nMuz\n", molecule.getMuz());

	printf("\nDipole moment\n");
	printf("%f\t%f\t%f\n", molecule.getDipole()(0), molecule.getDipole()(1),
	    molecule.getDipole()(2));
	printf("\nTotal dipole moment\n");
	printf("%f\n",
	    hypot(hypot(molecule.getDipole()(0), molecule.getDipole()(1)),
	        molecule.getDipole()(2)));

	for(int i = 0; i < molecule.getNumatoms(); i++) {
		printf("Charge on atom %d: %f\n", i,
		    molecule.getAtoms()[i].getTrueCharge());
	}
	fflush(stdout);
}

void compchem::printArray(std::string title,
                          compchem::FortranArray<double> &array) {
	puts("");
	puts(title.c_str());
	for(int i = 0; i < array.getShape()[0]; i++) {
		for(int j = 0; j < array.getShape()[1]; j++) {
			printf("%f\t", array(i, j));
		}
		printf("\n");
	}
	fflush(stdout);
}

void compchem::printMP2(Molecule &molecule) {
	printf("SCF Energy\t%f\n", molecule.getScfEnergy());
	printf("\nMP2 Energy\t%f\n", molecule.getMp2Correction());
	printf("\nTotal Energy\t%f\n", molecule.getMp2Energy());
	fflush(stdout);
}

