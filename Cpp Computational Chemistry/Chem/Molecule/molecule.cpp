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

compchem::Atom::Atom(int num, double x, double y, double z) {
	this->charge = 0;
	this->mass = compchem::amu(num);
	this->num = num;
	this->true_charge = 0;
	this->pos = new FortranArray<double>({x, y, z});
}

compchem::Atom::~Atom() {
//	if(pos != nullptr) {
//		delete pos;
//		pos = nullptr;
//	}
	;
}

compchem::Molecule::Molecule(std::vector<Atom> &atoms, int num, double scf_eps) :
		center_of_mass((new FortranArray<double>(Dimension(3)))),
		dipole((new FortranArray<double>(Dimension(3)))),
		inertial_moments((new FortranArray<double>(Dimension(3, 3)))),
		principle_moments((new FortranArray<double>(Dimension(3)))),
		rotational_constants((new FortranArray<double>(Dimension(3)))),
		atoms(*(new std::vector<Atom>())){
	long total_mem = 0;
	this->numatoms = num;
//	this->hessian_size = 3 * num;
	for(int i = 0; i < atoms.size(); i++) {
		this->atoms.push_back(atoms[i]);
	}

	this->electrons = 0;
	this->orbitals = 0;
	this->total_mass = 0;

	for(int i = 0; i < num; i++) {
		this->electrons += atoms[i].num;
		this->orbitals += compchem::orbitals(atoms[i].num);
		this->total_mass += atoms[i].mass;
	}

	this->scf_eps = scf_eps;
#ifdef __CARE_ABOUT_TOTAL_MEMORY_USAGE__
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
#else
	memory = NULL;
	blocks = -2;
#endif
	//Geometry data.
	distances = (new compchem::FortranArray<double>(
	    Dimension(numatoms, numatoms)));
	angles =
	        (new FortranArray<double>(Dimension(numatoms, numatoms, numatoms)));
	plane_angles = (new FortranArray<double>(
	    Dimension(numatoms, numatoms, numatoms, numatoms)));
	torsion_angles = (new FortranArray<double>(
	    Dimension(numatoms, numatoms, numatoms, numatoms)));
	rotor = ASYMMETRIC;

	//Harmonic data.
	hessian = (new FortranArray<double>(Dimension(3 * numatoms, 3 * numatoms)));
//	hessian_size = 3 * numatoms;
	hessian_eigs = (new FortranArray<double>(Dimension(3 * numatoms)));

	//Energies.
	enuc = 0;
	scf_energy = 0;
	molecular_energies = (new FortranArray<double>(
	    Dimension(orbitals, orbitals)));
	mp2_correction = 0;
	mp2_energy = 0;

	//Integrals.
	overlap = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	kinetic = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	attraction = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	hamiltonian = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	two_electron = (new FortranArray<double>(
	    Dimension(orbitals, orbitals, orbitals, orbitals)));
	mo_two_electron = (new FortranArray<double>(
	    Dimension(orbitals, orbitals, orbitals, orbitals)));
	spin_two_electron = (new FortranArray<double>(
	    Dimension(2 * orbitals, 2 * orbitals, 2 * orbitals, 2 * orbitals)));

	//Important matrices.
	orthogonal = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	orthogonal_t = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	orthogonal_eigvs =
	        (new FortranArray<double>(Dimension(orbitals, orbitals)));
	fock = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	density = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	molecular_orbitals = (new FortranArray<double>(
	    Dimension(orbitals, orbitals)));
	mux = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	muy = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	muz = (new FortranArray<double>(Dimension(orbitals, orbitals)));
	spin_fock =
	        (new FortranArray<double>(Dimension(2 * orbitals, 2 * orbitals)));

	//CCSD intermediates;
	t1_amplitudes = (new FortranArray<double>(
	    Dimension(2 * orbitals, 2 * orbitals)));
	t2_amplitudes = (new FortranArray<double>(
	    Dimension(2 * orbitals, 2 * orbitals, 2 * orbitals, 2 * orbitals)));
	inter_f = (new FortranArray<double>(Dimension(2 * orbitals, 2 * orbitals)));
	inter_w = (new FortranArray<double>(
	    Dimension(2 * orbitals, 2 * orbitals, 2 * orbitals, 2 * orbitals)));
	tpe1 = (new FortranArray<double>(Dimension(2 * orbitals, 2 * orbitals)));
	tpe2 = (new FortranArray<double>(
	    Dimension(2 * orbitals, 2 * orbitals, 2 * orbitals, 2 * orbitals)));

	vibrations = (new FortranArray<double>(Dimension(3 * numatoms)));
	occupied = electrons / 2;
}

Molecule::~Molecule() {

	delete center_of_mass;
	delete rotational_constants;
	delete inertial_moments;
	delete dipole;
	delete principle_moments;

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

	for(int i = 0; i < this->numatoms; i++) {
		delete (atoms[i].pos);
	}
	delete &atoms;

	if(blocks == -2) {
		//free(memory);
	} else if(blocks == -1) {
#ifndef __ELF__
		VirtualFree(memory, 0, MEM_RELEASE);
#endif
	} else {
#ifdef __ELF__
		//Close stuff.
#endif
	}

}

#endif
