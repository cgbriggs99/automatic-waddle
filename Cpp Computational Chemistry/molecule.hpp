/*
 * molecule.hpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#ifndef MOLECULE_HPP_
#define MOLECULE_HPP_

#include "arrays.hpp"
#include "input.hpp"

namespace compchem {

using namespace array;

class Atom {
	friend class Molecule;
private:
	double pos[3];
	double mass;
	double true_charge;
	int num;
	int charge;
	int id;
public:
	Atom(int num, double x, double y, double z);
};

class Molecule {
public:
	typedef int filedesc;
	filedesc blocks;
	char *memory;

	//Counts.
	int occupied;
	int electrons;
	int orbitals;

	//Atom data.
	Atom *atoms;
	int numatoms;
	double total_mass;

	//Geometry data.
	Array<double> *distances;
	Array<double> *angles;
	Array<double> *plane_angles;
	Array<double> *torsion_angles;
	double inertial_moments[3][3];
	double principle_moments[3];
	double rotational_constants[3];
	double center_of_mass[3];
	enum {
		SPHERICAL_TOP, LINEAR, SYMMETRIC, ASYMMETRIC
	} rotor;

	//Harmonic data.
	Array<double> *hessian;
	int hessian_size;
	Array<double> *hessian_eigs;

	//Energies.
	double enuc;
	double scf_energy;
	Array<double> *molecular_energies;
	double mp2_correction;
	double mp2_energy;

	//Integrals.
	Array<double> *overlap;
	Array<double> *kinetic;
	Array<double> *attraction;
	Array<double> *hamiltonian;
	Array<double> *two_electron;
	Array<double> *mo_two_electron;
	Array<double> *spin_two_electron;

	//Important matrices.
	Array<double> *orthogonal;
	Array<double> *orthogonal_t;
	Array<double> *orthogonal_eigvs;
	Array<double> *fock;
	Array<double> *density;
	Array<double> *molecular_orbitals;
	Array<double> *mux, *muy, *muz;
	double dipole[3];
	Array<double> *spin_fock;

	//CCSD intermediates;
	Array<double> *t1_amplitudes;
	Array<double> *t2_amplitudes;
	Array<double> *inter_f;
	Array<double> *inter_w;
	Array<double> *tpe1;
	Array<double> *tpe2;

	Molecule(Atom *atoms, int num);
	~Molecule();
};

extern int TEI(int mu, int nu, int lam, int sig);

extern double amu(int z);

extern int valence(int z);

extern int orbitals(int z);

}


#endif /* MOLECULE_HPP_ */
