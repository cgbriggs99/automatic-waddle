/*
 * molecule.hpp
 *
 *  Created on: Apr 25, 2018
 *      Author: Connor
 */

#ifndef MOLECULE_HPP_
#define MOLECULE_HPP_

#include <stdio.h>
#include <stdarg.h>
#include "compchem.hpp"

namespace compchem {
using namespace array;
using namespace compchem;

class Molecule;

class Atom {
	friend class Molecule;
	friend void compchem::input(compchem::Molecule **, FILE *);
private:
	Vector<double> pos;
	double mass;
	double true_charge;
	int num;
	int charge;
public:
	Atom() :
			pos(3) {
		this->mass = 0;
		this->true_charge = 0;
		this->num = 0;
		this->charge = 0;
	}
	Atom(int num, double x, double y, double z);

	int getCharge() {
		return (charge);
	}

	double getMass() {
		return (mass);
	}

	int getNum() {
		return (num);
	}

	array::Vector<double>& getPos() {
		return (pos);
	}

	double getTrueCharge() {
		return (true_charge);
	}

	void setTrueCharge(double trueCharge) {
		true_charge = trueCharge;
	}
};

template<class T>
class TEArray: Array<T> {
private:
public:
	TEArray(int dim, ...);
	TEArray(T *data, int dim, ...);	//Does not copy. Simply sets the data pointer.
	TEArray(const Array<T> &arr);	//Does copy.
	~TEArray();
	T &operator()(int i, int j, int k, int l);

	T *getArray();
};

class Molecule {

public:
	typedef enum {
		SPHERICAL_TOP, LINEAR, SYMMETRIC, ASYMMETRIC
	} rotor_t;
private:
	friend void inputHessian(Molecule **, FILE *);
	friend void inputSCF(Molecule **, FILE *, FILE *, FILE *, FILE *,
			FILE *, FILE *, FILE *, FILE *, FILE *);
	typedef int filedesc;
	filedesc blocks;
	char *memory;

	//Counts.
	int occupied;
	int electrons;
	int orbitals;

	double scf_eps;

	//Atom data.
	Atom *atoms;
	int numatoms;
	double total_mass;

	//Geometry data.
	Array<double> *distances;
	Array<double> *angles;
	Array<double> *plane_angles;
	Array<double> *torsion_angles;
	Vector<double> center_of_mass;
	Array<double> inertial_moments;	//TODO needs a getter.
	Array<double> principle_moments;
	Array<double> rotational_constants;

	rotor_t rotor;

	//Harmonic data.
	Array<double> *hessian;
	int hessian_size;
	Array<double> *hessian_eigs;
	Array<double> *vibrations;

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
	TEArray<double> *two_electron;
	TEArray<double> *mo_two_electron;
	Array<double> *spin_two_electron;

	//Important matrices.
	Array<double> *orthogonal;
	Array<double> *orthogonal_t;
	Array<double> *orthogonal_eigvs;
	Array<double> *fock;
	Array<double> *density;
	Array<double> *molecular_orbitals;
	Array<double> *mux, *muy, *muz;
	Vector<double> dipole;
	Array<double> *spin_fock;

	//CCSD intermediates;
	Array<double> *t1_amplitudes;
	Array<double> *t2_amplitudes;
	Array<double> *inter_f;
	Array<double> *inter_w;
	Array<double> *tpe1;
	Array<double> *tpe2;
public:
	/*
	 * Calculation methods.
	 */
	void computeDists();
	void computeAngles();
	void computePlaneAngles();
	void computeTorsionAngles();
	void translateCOM();
	void momentsOfInertia();
	void rotations();

	void harmonics();

	void computeSCF(bool print = false);

	void computeMOTEI();
	void computeMP2();

	void computeCCSD();

	void computeCCSDT();

	void computeSCF_DIIS();

	void computeSCF_Symm();

	void computeCCSD_DIIS();

	void computeExcited();

	void computeDLExcited();

	Molecule(Atom *atoms, int num, double scf_eps = 0.000000000001);
	~Molecule();

	Array<double>* getAngles() {
		return (angles);
	}

	Atom* getAtoms() {
		return (atoms);
	}

	Array<double>* getAttraction() {
		return (attraction);
	}

	filedesc getBlocks() {
		return (blocks);
	}

	Array<double>* getDensity() {
		return (density);
	}

	Vector<double> &getDipole() {
		return (dipole);
	}

	Array<double>* getDistances() {
		return (distances);
	}

	int getElectrons() {
		return (electrons);
	}

	double getEnuc() {
		return (enuc);
	}

	Array<double>* getFock() {
		return (fock);
	}

	Array<double>* getHamiltonian() {
		return (hamiltonian);
	}

	Array<double>* getHessian() {
		return (hessian);
	}

	Array<double>* getHessianEigs() {
		return (hessian_eigs);
	}

	int getHessianSize() {
		return (hessian_size);
	}

	Array<double>* getInterF() {
		return (inter_f);
	}

	Array<double>* getInterW() {
		return (inter_w);
	}

	Array<double>* getKinetic() {
		return (kinetic);
	}

	char* getMemory() {
		return (memory);
	}

	TEArray<double>* getMoTwoElectron() {
		return (mo_two_electron);
	}

	Array<double>* getMolecularEnergies() {
		return (molecular_energies);
	}

	Array<double>* getMolecularOrbitals() {
		return (molecular_orbitals);
	}

	double getMp2Correction() {
		return (mp2_correction);
	}

	double getMp2Energy() {
		return (mp2_energy);
	}

	Array<double>* getMux() {
		return (mux);
	}

	Array<double>* getMuy() {
		return (muy);
	}

	Array<double>* getMuz() {
		return (muz);
	}

	int getNumatoms() {
		return (numatoms);
	}

	int getOccupied() {
		return (occupied);
	}

	int getOrbitals() {
		return (orbitals);
	}

	Array<double>* getOrthogonal() {
		return (orthogonal);
	}

	Array<double>* getOrthogonalEigvs() {
		return (orthogonal_eigvs);
	}

	Array<double>* getOrthogonalT() {
		return (orthogonal_t);
	}

	Array<double>* getOverlap() {
		return (overlap);
	}

	Array<double>* getPlaneAngles() {
		return (plane_angles);
	}

	Array<double>& getPrincipleMoments() {
		return (principle_moments);
	}

	Array<double>& getRotationalConstants() {
		return (rotational_constants);
	}

	rotor_t getRotor() {
		return (rotor);
	}

	double getScfEnergy() {
		return (scf_energy);
	}

	Array<double>* getSpinFock() {
		return (spin_fock);
	}

	Array<double>* getSpinTwoElectron() {
		return (spin_two_electron);
	}

	Array<double>* getT1Amplitudes() {
		return (t1_amplitudes);
	}

	Array<double>* getT2Amplitudes() {
		return (t2_amplitudes);
	}

	Array<double>* getTorsionAngles() {
		return (torsion_angles);
	}

	double getTotalMass() {
		return (total_mass);
	}

	Array<double>* getTpe1() {
		return (tpe1);
	}

	Array<double>* getTpe2() {
		return (tpe2);
	}

	TEArray<double>* getTwoElectron() {
		return (two_electron);
	}

	void setEnuc(double enuc) {
		this->enuc = enuc;
	}

	Vector<double>& getCenterOfMass() {
		return (center_of_mass);
	}

	Array<double>& getInertialMoments() {
		return (inertial_moments);
	}
};

int TEI(int mu, int nu, int lam, int sig);

double amu(int z);

int valence(int z);

int orbitals(int z);

template<class T>
TEArray<T>::TEArray(int dim, ...) {
	va_list lst;
	va_start(lst, dim);

	this->dim = dim;
	this->sizes = (size_t *) calloc(dim, sizeof(size_t));
	this->total = 1;

	for (int i = 0; i < dim; i++) {
		this->sizes[i] = va_arg(lst, int);
		this->total *= this->sizes[i];
	}
	this->data = new T[this->total];
	this->freeOnDelete = true;
	va_end(lst);
}

template<class T>
TEArray<T>::TEArray(T *data, int dim, ...) {
	va_list lst;
	va_start(lst, dim);

	this->dim = dim;
	this->sizes = (size_t *) calloc(dim, sizeof(size_t));
	this->total = 1;

	for (int i = 0; i < dim; i++) {
		this->sizes[i] = va_arg(lst, int);
		this->total *= this->sizes[i];
	}
	this->data = data;
	this->freeOnDelete = false;
	va_end(lst);
}

template<class T>
TEArray<T>::TEArray(const Array<T> &arr) {
	this->dim = arr.dim;
	this->total = arr.total;
	this->data = new T[this->total];
	this->sizes = (size_t *) calloc(this->dim, sizeof(size_t));
	for (int i = 0; i < this->total; i++) {
		T temp = arr.data[i];
		this->data[i] = temp;
	}
	for (int i = 0; i < this->dim; i++) {
		this->sizes[i] = arr.sizes[i];
	}
	this->freeOnDelete = true;
}

template<class T>
TEArray<T>::~TEArray() {
	puts("debug15");
	fflush(stdout);
	free(this->sizes);
	puts("debug17");
		fflush(stdout);
	if (this->freeOnDelete) {
		delete [] this->data;
	}
	puts("debug16");
	fflush(stdout);
}

template<class T>
T &TEArray<T>::operator()(int i, int j, int k, int l) {
	size_t ij, kl, subsc;
	if (i < j) {
		ij = ((long) j * (long) (j + 1)) / 2 + i;
	} else {
		ij = ((long) i * (long) (i + 1)) / 2 + j;
	}

	if (k < l) {
		kl = ((long) l * (long) (l + 1)) / 2 + k;
	} else {
		kl = ((long) k * (long) (k + 1)) / 2 + l;
	}


	if (ij < kl) {
		subsc = ((long) kl * (long) (kl + 1)) / 2 + ij;
	} else {
		subsc = ((long) ij * (long) (ij + 1)) / 2 + kl;
	}
	if(subsc > this->total) {
		throw array::ArrayIndexOutOfBoundsException();
	}

	return (*&(this->data[subsc]));
}

template<class T>
T *TEArray<T>::getArray() {
	return (this->data);
}

}

#endif /* MOLECULE_HPP_ */
