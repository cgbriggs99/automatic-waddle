/*
 * molecule.c
 *
 *  Created on: Sep 12, 2017
 *      Author: connor
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef __ELF__
#include <sys/mman.h>
#else
#include <windows.h>
#endif

#include "../molecule.h"
#include "../calculations.h"
#include "../../Free/mem_custom.h"

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

static void allocateOrbitals(molecule_t *molecule) {
	int orbs = 0;
	for(int i = 0; i < molecule->num_atoms; i++) {
		orbs += orbitals(molecule->atoms[i].z);
	}
	molecule->orbitals = orbs;

	molecule->kin_energy = calloc(orbs, sizeof(double *));
	molecule->overlap = calloc(orbs, sizeof(double *));
	molecule->attraction = calloc(orbs, sizeof(double *));
	molecule->hamiltonian = calloc(orbs, sizeof(double *));
	molecule->symmetric = calloc(orbs, sizeof(double *));
	molecule->symm_vecs = calloc(orbs, sizeof(double *));
	molecule->fock = calloc(orbs, sizeof(double *));
	molecule->density = calloc(orbs, sizeof(double *));
	molecule->ccsd_energy = 0;

	if(orbs > 30) {
#ifndef __WIN32
		molecule->files.two_electron = open("./two_electron.bin",
				O_RDWR | O_CREAT | O_TRUNC, 0777);
		char zero = 0;
		for(int i = 0; i < TEI(orbs, orbs, orbs, orbs) + 1; i++) {
			write(molecule->files.two_electron, &zero, sizeof(char));
		}
		lseek(molecule->files.two_electron, 0, SEEK_SET);
		molecule->two_electron = mmap(NULL, sizeof(double) * (TEI(orbs, orbs, orbs, orbs) + 1),
				PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED,
				molecule->files.two_electron, 0);
#else
		molecule->two_electron = VirtualAlloc(NULL,
		    sizeof(double) * (TEI(orbs, orbs, orbs, orbs) + 1),
		    MEM_COMMIT | MEM_RESERVE,
		    PAGE_READWRITE);
		molecule->files.two_electron = 0;

#endif
	} else {
		molecule->two_electron = calloc(TEI(orbs, orbs, orbs, orbs) + 1,
		    sizeof(double));
		molecule->files.two_electron = -1;
	}

	molecule->molecular_orbitals = calloc(orbs, sizeof(double *));
	molecule->molecular_eigs = calloc(orbs, sizeof(double *));
	molecule->symm_trans = calloc(orbs, sizeof(double *));
	molecule->mux = calloc(orbs, sizeof(double *));
	molecule->muy = calloc(orbs, sizeof(double *));
	molecule->muz = calloc(orbs, sizeof(double *));
	molecule->mo_two_electron = calloc(TEI(orbs, orbs, orbs, orbs) + 1,
	    sizeof(double));
	molecule->spin_two_electron = calloc(2 * orbs, sizeof(double ***));
	molecule->t1_amplitude = calloc(2 * orbs, sizeof(double *));
	molecule->t2_amplitude = calloc(2 * orbs, sizeof(double ***));
	molecule->spin_fock = calloc(2 * orbs, sizeof(double *));
	molecule->inter_f = calloc(2 * orbs, sizeof(double *));
	molecule->inter_w = calloc(2 * orbs, sizeof(double ***));
	molecule->tpe1 = calloc(2 * orbs, sizeof(double ***));
	molecule->tpe2 = calloc(2 * orbs, sizeof(double ***));

	for(int i = 0; i < orbs; i++) {
		molecule->kin_energy[i] = calloc(orbs, sizeof(double));
		molecule->overlap[i] = calloc(orbs, sizeof(double));
		molecule->attraction[i] = calloc(orbs, sizeof(double));
		molecule->hamiltonian[i] = calloc(orbs, sizeof(double));
		molecule->symmetric[i] = calloc(orbs, sizeof(double));
		molecule->symm_vecs[i] = calloc(orbs, sizeof(double));
		molecule->fock[i] = calloc(orbs, sizeof(double));
		molecule->density[i] = calloc(orbs, sizeof(double));
		molecule->molecular_orbitals[i] = calloc(orbs, sizeof(double));
		molecule->molecular_eigs[i] = calloc(orbs, sizeof(double));
		molecule->symm_trans[i] = calloc(orbs, sizeof(double));
		molecule->mux[i] = calloc(orbs, sizeof(double));
		molecule->muy[i] = calloc(orbs, sizeof(double));
		molecule->muz[i] = calloc(orbs, sizeof(double));
	}

	for(int i = 0; i < 2 * orbs; i++) {
		molecule->tpe1[i] = calloc(2 * orbs, sizeof(double **));
		molecule->tpe2[i] = calloc(2 * orbs, sizeof(double **));
		molecule->spin_two_electron[i] = calloc(2 * orbs, sizeof(double **));
		molecule->t1_amplitude[i] = calloc(2 * orbs, sizeof(double));
		molecule->spin_fock[i] = calloc(2 * orbs, sizeof(double));
		molecule->t2_amplitude[i] = calloc(2 * orbs, sizeof(double **));
		molecule->inter_f[i] = calloc(2 * orbs, sizeof(double));
		molecule->inter_w[i] = calloc(2 * orbs, sizeof(double **));
		for(int j = 0; j < 2 * orbs; j++) {
			molecule->tpe1[i][j] = calloc(2 * orbs, sizeof(double *));
			molecule->tpe2[i][j] = calloc(2 * orbs, sizeof(double *));
			molecule->spin_two_electron[i][j] = calloc(2 * orbs,
			    sizeof(double *));
			molecule->t2_amplitude[i][j] = calloc(2 * orbs, sizeof(double *));
			molecule->inter_w[i][j] = calloc(2 * orbs, sizeof(double *));
			for(int k = 0; k < 2 * orbs; k++) {
				molecule->tpe1[i][j][k] = calloc(2 * orbs, sizeof(double));
				molecule->tpe2[i][j][k] = calloc(2 * orbs, sizeof(double));
				molecule->spin_two_electron[i][j][k] = calloc(2 * orbs,
				    sizeof(double));
				molecule->t2_amplitude[i][j][k] = calloc(2 * orbs,
				    sizeof(double));
				molecule->inter_w[i][j][k] = calloc(2 * orbs, sizeof(double));
			}
		}
	}
}

molecule_t *newMolecule(const atom_t *atoms, int num) {
	molecule_t *out;
	out = calloc(1, sizeof(molecule_t));

	out->num_atoms = num;
	out->atoms = calloc(num, sizeof(atom_t));
	out->angles = calloc(num, sizeof(double **));
//	out->bonds = calloc(numBonds, sizeof(bond_t));
	out->distances = calloc(num, sizeof(double *));
	out->plane_angles = calloc(num, sizeof(double ***));
	out->torsion_angles = calloc(num, sizeof(double ***));
	out->hessian = calloc(3 * num, sizeof(double *));
	out->hessian_eigs = calloc(3 * num, sizeof(double));

	out->num_hessian = 3 * num;
	out->electrons = 0;

//	out->num_bonds = numBonds;

	for(int i = 0; i < num; i++) {
		out->atoms[i].pos[0] = atoms[i].pos[0];
		out->atoms[i].pos[1] = atoms[i].pos[1];
		out->atoms[i].pos[2] = atoms[i].pos[2];
		out->atoms[i].id = i;
		out->atoms[i].charge = atoms[i].charge;
		out->atoms[i].mass = atoms[i].mass;
		out->atoms[i].z = atoms[i].z;
		out->electrons += out->atoms[i].z - out->atoms[i].charge; //It's a minus because electrons are negative.
	}

	for(int i = 0; i < num; i++) {
		out->plane_angles[i] = calloc(num, sizeof(double **));
		out->torsion_angles[i] = calloc(num, sizeof(double **));
		for(int j = 0; j < num; j++) {
			out->plane_angles[i][j] = calloc(num, sizeof(double *));
			out->torsion_angles[i][j] = calloc(num, sizeof(double *));
			for(int k = 0; k < num; k++) {
				out->plane_angles[i][j][k] = calloc(num, sizeof(double));
				out->torsion_angles[i][j][k] = calloc(num, sizeof(double));
			}
		}
	}

	for(int i = 0; i < num; i++) {
		out->distances[i] = calloc(num, sizeof(double));
	}

	for(int i = 0; i < num; i++) {
		out->angles[i] = calloc(num, sizeof(double *));
		for(int j = 0; j < num; j++) {
			out->angles[i][j] = calloc(num, sizeof(double));
		}
	}

	for(int i = 0; i < 3 * num; i++) {
		out->hessian[i] = calloc(3 * num, sizeof(double));
	}

	out->mass = 0;

	for(int i = 0; i < num; i++) {
		out->mass += atoms[i].mass;
	}

	allocateOrbitals(out);

	distances(out);
	angles(out);
	planeAngles(out);
	torsionAngles(out);
	centerOfMass(out);
	center(out);
	inertialMoments(out);
	principalMoments(out);
	rotor(out);
	rotationalConstants(out);

	return (out);

}

void deleteMolecule(molecule_t *molecule) {
	free(molecule->hessian_eigs);
	if(molecule->files.two_electron < 0) {
		free(molecule->two_electron);
	} else {
#ifndef __WIN32
		close(molecule->files.two_electron);
		free(molecule->two_electron);
#else
		VirtualFree(molecule->two_electron, 0, MEM_RELEASE);
#endif
	}
	free(molecule->mo_two_electron);

	for(int i = 0; i < molecule->orbitals; i++) {
		free(molecule->density[i]);
		free(molecule->symm_vecs[i]);
		free(molecule->symmetric[i]);
		free(molecule->fock[i]);
		free(molecule->attraction[i]);
		free(molecule->hamiltonian[i]);
		free(molecule->kin_energy[i]);
		free(molecule->overlap[i]);
		free(molecule->molecular_orbitals[i]);
		free(molecule->molecular_eigs[i]);
		free(molecule->symm_trans[i]);
		free(molecule->mux[i]);
		free(molecule->muy[i]);
		free(molecule->muz[i]);
	}

	for(int i = 0; i < molecule->num_hessian; i++) {
		free(molecule->hessian[i]);
	}

	for(int i = 0; i < molecule->num_atoms; i++) {
		free(molecule->distances[i]);
		for(int j = 0; j < molecule->num_atoms; j++) {
			free(molecule->angles[i][j]);
			for(int k = 0; k < molecule->num_atoms; k++) {
				free(molecule->plane_angles[i][j][k]);
				free(molecule->torsion_angles[i][j][k]);
			}
			free(molecule->plane_angles[i][j]);
			free(molecule->torsion_angles[i][j]);
		}
		free(molecule->angles[i]);
		free(molecule->plane_angles[i]);
		free(molecule->torsion_angles[i]);
	}

	for(int i = 0; i < 2 * molecule->orbitals; i++) {
		for(int j = 0; j < 2 * molecule->orbitals; j++) {
			for(int k = 0; k < 2 * molecule->orbitals; k++) {
				free(molecule->spin_two_electron[i][j][k]);
				free(molecule->t2_amplitude[i][j][k]);
				free(molecule->inter_w[i][j][k]);
				free(molecule->tpe1[i][j][k]);
				free(molecule->tpe2[i][j][k]);
			}
			free(molecule->spin_two_electron[i][j]);
			free(molecule->t2_amplitude[i][j]);
			free(molecule->inter_w[i][j]);
			free(molecule->tpe1[i][j]);
			free(molecule->tpe2[i][j]);
		}
		free(molecule->spin_two_electron[i]);
		free(molecule->spin_fock[i]);
		free(molecule->t1_amplitude[i]);
		free(molecule->t2_amplitude[i]);
		free(molecule->inter_f[i]);
		free(molecule->inter_w[i]);
		free(molecule->tpe1[i]);
		free(molecule->tpe2[i]);
	}
	free(molecule->atoms);
	free(molecule->tpe1);
	free(molecule->tpe2);
	free(molecule->inter_f);
	free(molecule->inter_w);
	free(molecule->spin_fock);
	free(molecule->t1_amplitude);
	free(molecule->t2_amplitude);
	free(molecule->spin_two_electron);
	free(molecule->mux);
	free(molecule->muy);
	free(molecule->muz);
	free(molecule->symm_trans);
	free(molecule->symm_vecs);
	free(molecule->symmetric);
	free(molecule->fock);
	free(molecule->overlap);
	free(molecule->kin_energy);
	free(molecule->hamiltonian);
	free(molecule->attraction);
	free(molecule->distances);
	free(molecule->hessian);
	free(molecule->angles);
	free(molecule->plane_angles);
	free(molecule->torsion_angles);
	free(molecule->density);
	free(molecule->molecular_orbitals);
	free(molecule->molecular_eigs);
	free(molecule);
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
