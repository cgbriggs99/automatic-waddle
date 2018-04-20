/*
 * output.c
 *
 *  Created on: Sep 12, 2017
 *      Author: connor
 */

#include <string.h>
#include <math.h>
#include <stdio.h>
#include "../molecule.h"
#include "../output.h"

void printDists(const molecule_t *molecule) {
	puts("Distances");
	for (int i = 0; i < molecule->num_atoms; i++) {
		for (int j = 0; j < i; j++) {
			printf("%d\t%d\t%8.5f\n", i, j, molecule->distances[i][j]);
		}
	}
}

void printAngles(const molecule_t *molecule) {
	puts("\nBond angles");
	for (int i = 0; i < molecule->num_atoms; i++) {
		for (int j = 0; j < i; j++) {
			for (int k = 0; k < j; k++) {
				if (molecule->distances[i][k] > 4.0
						|| molecule->distances[j][k] > 4.0) {
					continue;
				}
				printf("%d\t%d\t%d\t%3.4f\n", i, j, k,
						180 * molecule->angles[i][j][k] / M_PI);
			}
		}
	}

	puts("\nPlane angles");
	for (int i = 0; i < molecule->num_atoms; i++) {
		for (int j = 0; j < i; j++) {
			for (int k = 0; k < j; k++) {
				for (int l = 0; l < k; l++) {
					if (molecule->distances[i][k] > 4.0
							|| molecule->distances[j][k] > 4.0
							|| molecule->distances[k][l] > 4.0) {
						continue;
					}
					printf("%d\t%d\t%d\t%d\t%3.4f\n", i, j, k, l,
							180 * molecule->plane_angles[i][j][k][l] / M_PI);
				}
			}
		}
	}

	puts("\nTorsion angles");
	for (int i = 0; i < molecule->num_atoms; i++) {
		for (int j = 0; j < i; j++) {
			for (int k = 0; k < j; k++) {
				for (int l = 0; l < k; l++) {
					if (molecule->distances[i][j] > 4.0
							|| molecule->distances[j][k] > 4.0
							|| molecule->distances[k][l] > 4.0) {
						continue;
					}
					printf("%d\t%d\t%d\t%d\t%3.4f\n", i, j, k, l,
							180 * molecule->torsion_angles[i][j][k][l] / M_PI);
				}
			}
		}
	}
}

void printMoments(const molecule_t *molecule) {
	puts("\nCenter of Mass");
	printf("%f %f %f\n", molecule->center_of_mass[0], molecule->center_of_mass[1], molecule->center_of_mass[2]);

	puts("\nMoments of Inertia");
	for (int i = 0; i < 3; i++) {
		printf("%5.5f %5.5f %5.5f\n", molecule->inertial_moments[i][0],
				molecule->inertial_moments[i][1], molecule->inertial_moments[i][2]);
	}

	puts("\nPrimary moments (amu bohr^2)");
	printf("%5.5f %5.5f %5.5f\n", molecule->principal_moments[0], molecule->principal_moments[1], molecule->principal_moments[2]);
	puts("\nPrimary moments (amu \u00e5^2)");
	printf("%5.5f %5.5f %5.5f\n", molecule->principal_moments[0] * .2800285, molecule->principal_moments[1] * .2800285,
			molecule->principal_moments[2] * .2800285);
	puts("\nPrimary moments (g cm^2)");
	printf("%5.5e %5.5e %5.5e\n", 1.660468e-24 * molecule->principal_moments[0] * .00000000000000002800285,
			1.660468e-24 * molecule->principal_moments[1] * .00000000000000002800285,
			1.660468e-24 * molecule->principal_moments[2] * .00000000000000002800285);

	puts("\nRotational Constants (1/cm)");
	printf("%f %f %f\n", molecule->rotational_constants[0], molecule->rotational_constants[1],
			molecule->rotational_constants[2]);

	puts("\nRotational Constants (MHz)");
	printf("%f %f %f\n", 29979.2458 * molecule->rotational_constants[0], 29979.2458 * molecule->rotational_constants[1],
			29979.2458 * molecule->rotational_constants[2]);

}

void printRotor(const molecule_t *molecule) {
	puts("\nRotor type");
	switch (molecule->rotor) {
	case SPHERICAL_TOP:
		puts("Spherical top");
		break;
	case LINEAR:
		puts("Linear");
		break;
	case SYMMETRIC:
		puts("Symmetric");
		break;
	default:
		puts("Asymmetric");
	}
}

void printEigenvalues(const molecule_t *molecule) {
	puts("\nHessian eigenvalues.");
	for(int i = molecule->num_hessian - 1; i >= 0; i--) {
		printf("%d\t%f\n", i, molecule->hessian_eigs[i]);
	}
}

void printFreqs(const molecule_t *molecule) {
	puts("\nHarmonics.");
	for(int i = molecule->num_hessian - 1; i >= 0; i--) {
		printf("%d\t%f\n", i, sqrt(molecule->hessian_eigs[i]) * 5140.697669352);
	}
}

void outputSFC(molecule_t *molecule) {
	puts("\nFock matrix");
	for(int i = 0; i < molecule->orbitals; i++) {
		for(int j = 0; j < molecule->orbitals; j++) {
			printf("%f\t", molecule->fock[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printArray("\nMux\n", molecule->mux, molecule->orbitals, molecule->orbitals);
	printArray("\nMuy\n", molecule->muy, molecule->orbitals, molecule->orbitals);
	printArray("\nMuz\n", molecule->muz, molecule->orbitals, molecule->orbitals);

	printf("\nDipole moment\n");
	printf("%f\t%f\t%f\n", molecule->dipole[0], molecule->dipole[1],
			molecule->dipole[2]);
	printf("\nTotal dipole moment\n");
	printf("%f\n", hypot(hypot(molecule->dipole[0], molecule->dipole[1]),
			molecule->dipole[2]));

	for(int i = 0; i < molecule->num_atoms; i++) {
		printf("Charge on atom %d: %f\n", i, molecule->atoms[i].true_charge);
	}
}

void printArray(char *title, double **array, int r, int c) {
	puts(title);
	for(int i = 0; i < r; i++) {
		for(int j = 0; j < c; j++) {
			printf("%f\t", array[i][j]);
		}
		printf("\n");
	}
	fflush(stdout);
}

void printMP2(const molecule_t *molecule) {
	printf("SCF Energy\t%f\n", molecule->scf_energy);
	printf("\nMP2 Energy\t%f\n", molecule->mp2_energy);
	printf("\nTotal Energy\t%f\n", molecule->tot_mp2);
}
