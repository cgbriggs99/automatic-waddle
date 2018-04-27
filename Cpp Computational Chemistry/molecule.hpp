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
using namespace compchem;

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
private:
	friend void compchem::input(compchem::Molecule **, FILE *);
	friend void compchem::inputHessian(Molecule **, FILE *);
	friend void compchem::inputSCF(Molecule **, FILE *, FILE *, FILE *, FILE *,
			FILE *, FILE *, FILE *, FILE *, FILE *);
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
	double inertial_moments[3][3];	//TODO needs a getter.
	double principle_moments[3];
	double rotational_constants[3];
	double center_of_mass[3];
	typedef enum {
		SPHERICAL_TOP, LINEAR, SYMMETRIC, ASYMMETRIC
	} rotor_t;
	rotor_t rotor;

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

	void computeSCF();

	void computeMP2();

	void computeCCSD();

	void computeCCSDT();

	void computeSCF_DIIS();

	void computeSCF_Symm();

	void computeCCSD_DIIS();

	void computeExcited();

	void computeDLExcited();
public:
	Molecule(Atom *atoms, int num);
	~Molecule();

	const Array<double>* getAngles() const {
		return angles;
	}

	const Atom* getAtoms() const {
		return atoms;
	}

	const Array<double>* getAttraction() const {
		return attraction;
	}

	filedesc getBlocks() const {
		return blocks;
	}

	const Array<double>* getDensity() const {
		return density;
	}

	const double* getDipole() const {
		return dipole;
	}

	const Array<double>* getDistances() const {
		return distances;
	}

	int getElectrons() const {
		return electrons;
	}

	double getEnuc() const {
		return enuc;
	}

	const Array<double>* getFock() const {
		return fock;
	}

	const Array<double>* getHamiltonian() const {
		return hamiltonian;
	}

	const Array<double>* getHessian() const {
		return hessian;
	}

	const Array<double>* getHessianEigs() const {
		return hessian_eigs;
	}

	int getHessianSize() const {
		return hessian_size;
	}

	const Array<double>* getInterF() const {
		return inter_f;
	}

	const Array<double>* getInterW() const {
		return inter_w;
	}

	const Array<double>* getKinetic() const {
		return kinetic;
	}

	char* getMemory() const {
		return memory;
	}

	const Array<double>* getMoTwoElectron() const {
		return mo_two_electron;
	}

	const Array<double>* getMolecularEnergies() const {
		return molecular_energies;
	}

	const Array<double>* getMolecularOrbitals() const {
		return molecular_orbitals;
	}

	double getMp2Correction() const {
		return mp2_correction;
	}

	double getMp2Energy() const {
		return mp2_energy;
	}

	const Array<double>* getMux() const {
		return mux;
	}

	const Array<double>* getMuy() const {
		return muy;
	}

	const Array<double>* getMuz() const {
		return muz;
	}

	int getNumatoms() const {
		return numatoms;
	}

	int getOccupied() const {
		return occupied;
	}

	int getOrbitals() const {
		return orbitals;
	}

	const Array<double>* getOrthogonal() const {
		return orthogonal;
	}

	const Array<double>* getOrthogonalEigvs() const {
		return orthogonal_eigvs;
	}

	const Array<double>* getOrthogonalT() const {
		return orthogonal_t;
	}

	const Array<double>* getOverlap() const {
		return overlap;
	}

	const Array<double>* getPlaneAngles() const {
		return plane_angles;
	}

	const double* getPrincipleMoments() const {
		return principle_moments;
	}

	const double* getRotationalConstants() const {
		return rotational_constants;
	}

	rotor_t getRotor() const {
		return rotor;
	}

	double getScfEnergy() const {
		return scf_energy;
	}

	const Array<double>* getSpinFock() const {
		return spin_fock;
	}

	const Array<double>* getSpinTwoElectron() const {
		return spin_two_electron;
	}

	const Array<double>* getT1Amplitudes() const {
		return t1_amplitudes;
	}

	const Array<double>* getT2Amplitudes() const {
		return t2_amplitudes;
	}

	const Array<double>* getTorsionAngles() const {
		return torsion_angles;
	}

	double getTotalMass() const {
		return total_mass;
	}

	const Array<double>* getTpe1() const {
		return tpe1;
	}

	const Array<double>* getTpe2() const {
		return tpe2;
	}

	const Array<double>* getTwoElectron() const {
		return two_electron;
	}
};

extern int TEI(int mu, int nu, int lam, int sig);

extern double amu(int z);

extern int valence(int z);

extern int orbitals(int z);

}


#endif /* MOLECULE_HPP_ */
