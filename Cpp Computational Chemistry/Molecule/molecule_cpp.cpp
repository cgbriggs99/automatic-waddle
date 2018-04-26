/*
 * molecule_cpp.cpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#include "../molecule.hpp"

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
#include "../input.h"
#include "../output.h"
#include "../linker.h"
#include "../calculations.h"

using namespace std;
using namespace compchem;

Atom::Atom(int num, double x, double y, double z) {
	this->charge = 0;
	this->id = 0;
	this->mass = amu(num);
	this->num = num;
	this->pos[0] = x;
	this->pos[1] = y;
	this->pos[2] = z;
	this->true_charge = 0;
}

Molecule::Molecule(Atom *atoms, int num) {
	long total_mem = 0;
	this->atoms = atoms;
	this->numatoms = num;

	this->electrons = 0;
	this->orbitals = 0;
	this->total_mass = 0;

	for (int i = 0; i < num; i++) {
		this->electrons += atoms[i].num;
		this->orbitals += compchem::orbitals(atoms[i].num);
		this->total_mass += atoms[i].mass;
	}

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
	total_mem += this->orbitals * this->orbitals * this->orbitals * this->orbitals;
	total_mem += this->orbitals * this->orbitals * this->orbitals * this->orbitals;
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
			*_inter_f, *_inter_w, *_tpe1, *_tpe2;

	occupied = electrons / 2;
	long shift = 0;

	_distances = new (this->memory) double[this->numatoms * this->numatoms];
	shift += this->numatoms * this->numatoms;
	_angles = new (this->memory + shift) double[this->numatoms * this->numatoms
			* this->numatoms];
	shift += this->numatoms * this->numatoms * this->numatoms;
	_plane_angles = new (this->memory + shift) double[this->numatoms
			* this->numatoms * this->numatoms * this->numatoms];
	shift += this->numatoms * this->numatoms * this->numatoms * this->numatoms;
	_torsion_angles = new (this->memory + shift) double[this->numatoms
			* this->numatoms * this->numatoms * this->numatoms];
	shift += this->numatoms * this->numatoms * this->numatoms * this->numatoms;
	_hessian = new (this->memory + shift) double[9 * this->numatoms
			* this->numatoms];
	shift += 9 * this->numatoms * this->numatoms;
	_hessian_eigs = new (this->memory + shift) double[3 * this->numatoms];
	shift += 3 * this->numatoms;
	_molecular_energies = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals;
	_overlap =
			new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals;
	_kinetic =
			new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals;
	_attraction = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals;
	_hamiltonian = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals;
	_two_electron = new (this->memory + shift) double[this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * this->orbitals * this->orbitals;
	_mo_two_electron = new (this->memory + shift) double[this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals * this->orbitals * this->orbitals;
	_spin_two_electron = new (this->memory + shift) double[16 * this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += 16 * this->orbitals * this->orbitals * this->orbitals
			* this->orbitals;
	_orthogonal = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals;
	_orthogonal_t = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals;
	_orthogonal_eigvs = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals;
	_fock = new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals;
	_density =
			new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals;
	_molecular_orbitals = new (this->memory + shift) double[this->orbitals
			* this->orbitals];
	shift += this->orbitals * this->orbitals;
	_mux = new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals;
	_muy = new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals;
	_muz = new (this->memory + shift) double[this->orbitals * this->orbitals];
	shift += this->orbitals * this->orbitals;
	_spin_fock = new (this->memory + shift) double[4 * this->orbitals
			* this->orbitals];
	shift += 4 * this->orbitals * this->orbitals;
	_t1_amplitudes = new (this->memory + shift) double[4 * this->orbitals
			* this->orbitals];
	shift += 4 * this->orbitals * this->orbitals;
	_t2_amplitudes = new (this->memory + shift) double[16 * this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += 16 * this->orbitals * this->orbitals * this->orbitals
			* this->orbitals;
	_inter_f = new (this->memory + shift) double[4 * this->orbitals
			* this->orbitals];
	shift += 4 * this->orbitals * this->orbitals;
	_inter_w = new (this->memory + shift) double[16 * this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += 16 * this->orbitals * this->orbitals * this->orbitals
			* this->orbitals;
	_tpe1 = new (this->memory + shift) double[4 * this->orbitals
			* this->orbitals];
	shift += 4 * this->orbitals * this->orbitals;
	_tpe2 = new (this->memory + shift) double[16 * this->orbitals
			* this->orbitals * this->orbitals * this->orbitals];
	shift += 16 * this->orbitals * this->orbitals * this->orbitals
			* this->orbitals;

	//Geometry data.
	distances = new Array<double>(_distances, 2, numatoms, numatoms);
	angles = new Array<double>(_angles, 3, numatoms, numatoms, numatoms);
	plane_angles = new Array<double>(_plane_angles, 4, numatoms, numatoms,
			numatoms, numatoms);
	torsion_angles = new Array<double>(_torsion_angles, 4, numatoms, numatoms,
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
	two_electron = new Array<double>(_two_electron, 4, orbitals, orbitals,
			orbitals, orbitals);
	mo_two_electron = new Array<double>(_mo_two_electron, 4, orbitals, orbitals,
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

}

Molecule::~Molecule() {
	//Counts.
	int occupied;
	int electrons;
	int orbitals;

	//Atom data.
	Atom *atoms;
	int numatoms;
	double total_mass;

	//Geometry data.
	delete distances;
	delete angles;
	delete plane_angles;
	delete torsion_angles;

	//Harmonic data.
	delete hessian;
	delete hessian_eigs;

	//Energies.
	delete molecular_energies;

	//Integrals.
	delete overlap;
	delete kinetic;
	delete attraction;
	delete hamiltonian;
	delete two_electron;
	delete mo_two_electron;
	delete spin_two_electron;

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

	//CCSD intermediates;
	delete t1_amplitudes;
	delete t2_amplitudes;
	delete inter_f;
	delete inter_w;
	delete tpe1;
	delete tpe2;

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
	delete[] atoms;
}

