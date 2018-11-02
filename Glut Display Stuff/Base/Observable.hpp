/*
 * Observable.hpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */

#ifndef OBSERVABLE_HPP_
#define OBSERVABLE_HPP_

#include <typeinfo>
#include <typeindex>
#include <map>
#include <vector>

#include "../namespace_name.hpp"
#include "base.hpp"

#include "../../Design Patterns/design_patterns.hpp"

namespace NAME {

class ObservableConnector : public design_patterns::Singleton<ObservableConnector> {
public :
	void sendReport(Report &r);

	void registerObserver(Observer *o, std::type_index report_type);

	void deregisterObserver(Observer *o, std::type_index report_type);

	bool isObserving(Observer *o, std::type_index report_type);
private:
	std::map<std::type_index, std::vector<Observer *>> observers;
};

#include "Observable.cpp"

}

#endif /* OBSERVABLE_HPP_ */
