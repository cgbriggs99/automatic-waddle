/*
 * molecule_factory.hpp
 *
 *  Created on: Dec 4, 2018
 *      Author: cgbri
 */

#ifndef MOLECULE_FACTORY_HPP_
#define MOLECULE_FACTORY_HPP_

#include "../../../molecule.hpp"

namespace compchem {

class MoleculeFactoryStrategy {
public:
	virtual ~MoleculeFactoryStrategy() = 0;

	virtual Molecule *build();
};

class MoleculeFactory {
private:
	static MoleculeFactory *singleton;

	MoleculeFactory();
public:
	~MoleculeFactory() = default;

	static MoleculeFactory *getSingleton();

	Molecule *buildMolecule(MoleculeFactoryStrategy &strat);
};

}

#endif /* MOLECULE_FACTORY_HPP_ */
