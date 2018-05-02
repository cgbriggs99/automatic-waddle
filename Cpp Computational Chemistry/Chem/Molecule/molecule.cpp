/*
 * molecule_cpp.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#ifndef MOLECULE_CPP__
#define MOLECULE_CPP__

#ifdef __ELF__
#include <sys/mman.h>
#else
#include <windows.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

#include "../../compchem.hpp"

using namespace std;
using namespace compchem;

compchem::Atom::Atom(int num, double x, double y, double z): pos(3) {
	this->charge = 0;
	this->mass = compchem::amu(num);
	this->num = num;
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->true_charge = 0;
}

compchem::Molecule::Molecule(Atom *atoms, int num, double scf_eps) : center_of_mass(3), dipole(3), inertial_moments(2,3,3), principle_moments(1,3), rotational_constants(1,3) {
	long total_mem = 0;
	this->atoms = atoms;
	this->numatoms = num;
	this->hessian_size = 3 * num;

	this->electrons = 0;
	this->orbitals = 0;
	this->total_mass = 0;

	for (int i = 0; i < num; i++) {
		this->electrons += atoms[i].num;
		this->orbitals += compchem::orbitals(atoms[i].num);
		this->total_mass += atoms[i].mass;
	}

	this->scf_eps = scf_eps;

	total_mem = 0;
	total_mem += this->numatoms * this->numatoms;
	total_mem += this->numatoms * this->numatoms * this->numatoms;
	total_mem += this->numatoms * this->numatoms * this->numatoms * this->numatoms;
	total_mem += this->numatoms * this->numatoms * this->numatoms * this->numatoms;
	total_mem += 9 * this->numatoms * this->numatoms;
	total_mem += 3 * this->numatoms;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += TEI(this->orbitals, this->orbitals, this->orbitals, this->orbitals);
	total_mem += TEI(this->orbitals, this->orbitals, this->orbitals, this->orbitals);
	total_mem += 16 * this->orbitals * this->orbitals * this->orbitals
				* this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals;
	total_mem += 4 * this->orbitals * this->orbitals;
	total_mem += 4 * this->orbitals * this->orbitals;
	total_mem += 16 * this->orbitals * this->orbitals * this->orbitals
				* this->orbitals;
	total_mem += 4 * this->orbitals * this->orbitals;
	total_mem += 16 * this->orbitals * this->orbitals * this->orbitals
				* this->orbitals;
	total_mem += 4 * this->orbitals * this->orbitals;
	total_mem += 16 * this->orbitals * this->orbitals * this->orbitals
				* this->orbitals;
	total_mem += this->hessian_size;

	if (total_mem * sizeof(double) >= 2147483647L) {
#ifdef __ELF__

#else
		this->memory = (char *) VirtualAlloc(NULL, sizeof(double) * total_mem,
		MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		this->blocks = -1;
#endif
	} else {
		this->memory = (char *) malloc(total_mem * sizeof(double));
		this->blocks = -2;
	}

	double *_distances, *_angles, *_plane_angles, *_torsion_angles, *_hessian,
			*_hessian_eigs, *_molecular_energies, *_overlap, *_kinetic,
			*_attraction, *_hamiltonian, *_two_electron, *_mo_two_electron,
			*_spin_two_electron, *_orthogonal, *_orthogonal_t,
			*_orthogonal_eigvs, *_fock, *_density, *_molecular_orbitals, *_mux,
			*_muy, *_muz, *_spin_fock, *_t1_amplitudes, *_t2_amplitudes,
			*_inter_f, *_inter_w, *_tpe1, *_tpe2, *_vibrations;

	occupied = electrons / 2;
	long shift = 0;

	_distances = new (this->memory) double[this->numatoms * this->numatoms];
	shift += this->numatoms * this->numatoms * sizeof(double);
	_angles = new (this->memory + shift) double[this->numatoms * this->numatoms
			* this->numatoms];
	shift += this->numatoms * this->numatoms * this->numatoms * sizeof(double);
	_plane_angles = new (this->memory + shift) double[this->numatoms
			* this->numatoms * this->numatoms * this->numatoms];
	shift += this->numatoms * this->numatoms * this->numatoms * this->numatoms * sizeof(double);
	_torsion_angles = new (this->memory + shift) double[this->numatoms
			* this->numatoms * this->numatoms * this->numatoms];
	shift += this->numatoms * this->numatoms * this->numatoms * this->numatoms * sizeof(double);
	_hessian = new (this->memory + shift) double[9 * this->numatoms
			* this->numatoms];
	shift += 9 * this->numatoms * this->numatoms * sizeof(double);
	_hessian_eigs = new (this->memory + shift) double[3 * this->numatoms];
	shift += 3 * this->numatoms * sizeof(double);
	_molecular_energies = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_overlap =
			new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_kinetic =
			new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_attraction = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_hamiltonian = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);

	//TODO Redo the size of this so it is not using the full size.
	_two_electron = new (this->memory + shift) double[TEI(this->orbitals, this->orbitals, this->orbitals, this->orbitals)];
	shift += TEI(this->orbitals, this->orbitals, this->orbitals, this->orbitals) * sizeof(double);
	_mo_two_electron = new (this->memory + shift) double[TEI(this->orbitals, this->orbitals, this->orbitals, this->orbitals)];
	shift += TEI(this->orbitals, this->orbitals, this->orbitals, this->orbitals) * sizeof(double);
	_spin_two_electron = new (this->memory + shift) double[16 * this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += 16 * this->orbitals * this->orbitals * this->orbitals
			* this->orbitals * sizeof(double);
	_orthogonal = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_orthogonal_t = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_orthogonal_eigvs = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_fock = new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_density =
			new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_molecular_orbitals = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_mux = new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_muy = new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_muz = new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * sizeof(double);
	_spin_fock = new (this->memory + shift) double[4 * this->orbitals
			* this->orbitals];
	shift += 4 * this->orbitals * this->orbitals * sizeof(double);
	_t1_amplitudes = new (this->memory + shift) double[4 * this->orbitals
			* this->orbitals];
	shift += 4 * this->orbitals * this->orbitals * sizeof(double);
	_t2_amplitudes = new (this->memory + shift) double[16 * this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += 16 * this->orbitals * this->orbitals * this->orbitals
			* this->orbitals * sizeof(double);
	_inter_f = new (this->memory + shift) double[4 * this->orbitals
			* this->orbitals];
	shift += 4 * this->orbitals * this->orbitals * sizeof(double);
	_inter_w = new (this->memory + shift) double[16 * this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += 16 * this->orbitals * this->orbitals * this->orbitals
			* this->orbitals * sizeof(double);
	_tpe1 = new (this->memory + shift) double[4 * this->orbitals
			* this->orbitals];
	shift += 4 * this->orbitals * this->orbitals * sizeof(double);
	_tpe2 = new (this->memory + shift) double[16 * this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += 16 * this->orbitals * this->orbitals * this->orbitals
			* this->orbitals * sizeof(double);
	_vibrations = new (this->memory + shift) double[this->hessian_size * this->hessian_size];
	shift += this->hessian_size * sizeof(double);

	//Geometry data.
	distances = new array::Array<double>(_distances, 2, numatoms, numatoms);
	angles = new array::Array<double>(_angles, 3, numatoms, numatoms, numatoms);
	plane_angles = new array::Array<double>(_plane_angles, 4, numatoms, numatoms,
			numatoms, numatoms);
	torsion_angles = new array::Array<double>(_torsion_angles, 4, numatoms, numatoms,
			numatoms, numatoms);
	rotor = ASYMMETRIC;

	//Harmonic data.
	hessian = new Array<double>(_hessian, 2, 3 * numatoms, 3 * numatoms);
	hessian_size = 3 * numatoms;
	hessian_eigs = new Array<double>(_hessian_eigs, 1, 3 * numatoms);

	//Energies.
	enuc = 0;
	scf_energy = 0;
	molecular_energies = new Array<double>(_molecular_energies, 2, orbitals,
			orbitals);
	mp2_correction = 0;
	mp2_energy = 0;

	//Integrals.
	overlap = new Array<double>(_overlap, 2, orbitals, orbitals);
	kinetic = new Array<double>(_kinetic, 2, orbitals, orbitals);
	attraction = new Array<double>(_attraction, 2, orbitals, orbitals);
	hamiltonian = new Array<double>(_hamiltonian, 2, orbitals, orbitals);
	two_electron = new TEArray<double>(_two_electron, 4, orbitals, orbitals,
			orbitals, orbitals);
	mo_two_electron = new TEArray<double>(_mo_two_electron, 4, orbitals, orbitals,
			orbitals, orbitals);
	spin_two_electron = new Array<double>(_spin_two_electron, 4, 2 * orbitals,
			2 * orbitals, 2 * orbitals, 2 * orbitals);

	//Important matrices.
	orthogonal = new Array<double>(_orthogonal, 2, orbitals, orbitals);
	orthogonal_t = new Array<double>(_orthogonal_t, 2, orbitals, orbitals);
	orthogonal_eigvs = new Array<double>(_orthogonal_eigvs, 2, orbitals,
			orbitals);
	fock = new Array<double>(_fock, 2, orbitals, orbitals);
	density = new Array<double>(_density, 2, orbitals, orbitals);
	molecular_orbitals = new Array<double>(_molecular_orbitals, 2, orbitals,
			orbitals);
	mux = new Array<double>(_mux, 2, orbitals, orbitals);
	muy = new Array<double>(_muy, 2, orbitals, orbitals);
	muz = new Array<double>(_muz, 2, orbitals, orbitals);
	spin_fock = new Array<double>(_spin_fock, 2, 2 * orbitals, 2 * orbitals);

	//CCSD intermediates;
	t1_amplitudes = new Array<double>(_t1_amplitudes, 2, 2 * orbitals,
			2 * orbitals);
	t2_amplitudes = new Array<double>(_t2_amplitudes, 4, 2 * orbitals,
			2 * orbitals, 2 * orbitals, 2 * orbitals);
	inter_f = new Array<double>(_inter_f, 2, 2 * orbitals, 2 * orbitals);
	inter_w = new Array<double>(_inter_w, 4, 2 * orbitals, 2 * orbitals,
			2 * orbitals, 2 * orbitals);
	tpe1 = new Array<double>(_tpe1, 2, 2 * orbitals, 2 * orbitals);
	tpe2 = new Array<double>(_tpe2, 4, 2 * orbitals, 2 * orbitals, 2 * orbitals,
			2 * orbitals);

	vibrations = new Array<double>(_vibrations, 1, this->hessian_size);
}

Molecule::~Molecule() {
	puts("debug6");
	fflush(stdout);
	//Geometry data.
	delete distances;
	delete angles;
	delete plane_angles;
	delete torsion_angles;

	puts("debug9");
	fflush(stdout);

	//Harmonic data.
	delete hessian;
	delete hessian_eigs;

	puts("debug11");
	fflush(stdout);

	//Energies.
	delete molecular_energies;

	//Integrals.
	delete overlap;
	delete kinetic;
	delete attraction;
	delete hamiltonian;

	puts("debug14");
	fflush(stdout);

	delete two_electron;
	delete mo_two_electron;
	delete spin_two_electron;

	puts("debug12");
	fflush(stdout);

	//Important matrices.
	delete orthogonal;
	delete orthogonal_t;
	delete orthogonal_eigvs;
	delete fock;
	delete density;
	delete molecular_orbitals;
	delete mux;
	delete muy;
	delete muz;
	delete spin_fock;

	puts("debug13");
	fflush(stdout);

	//CCSD intermediates;
	delete t1_amplitudes;
	delete t2_amplitudes;
	delete inter_f;
	delete inter_w;
	delete tpe1;
	delete tpe2;

	puts("debug10");
	fflush(stdout);

	for(int i = 0; i < this->numatoms; i++) {
		delete (atoms + i);
	}
	puts("debug7");
	fflush(stdout);
	free(atoms);

	if(blocks == -2) {
		free(memory);
	} else if(blocks == -1) {
#ifndef __ELF__
		VirtualFree(memory, 0, MEM_RELEASE);
#endif
	} else {
#ifdef __ELF__
		//Close stuff.
#endif
	}
	puts("debug8");
	fflush(stdout);
}


#endif
