/*
 * Observable.cpp
 *
 *  Created on: Nov 2, 2018
 *      Author: connor
 */


#include "Observable.hpp"

void NAME::ObservableConnector::deregisterObserver(Observer *o, std::type_index report) {
	std::vector<Observer *> v = this->observers[report];
	for(int i = 0; i < v.size(); i++) {
		if(v[i] == o) {
			Observer *temp = v[v.size() - 1];
			v[v.size() - 1] = v[i];
			v[i] = temp;
			v.pop_back();
		}
	}
}

bool NAME::ObservableConnector::isObserving(Observer *o, std::type_index report) {
	std::vector<Observer *> v = this->observers[report];
	for(int i = 0; i < v.size(); i++) {
		if(v[i] == o) {
			return (true);
		}
	}
	return (false);
}

void NAME::ObservableConnector::registerObserver(Observer *o, std::type_index report) {
	this->observers[report].push_back(o);
}

void NAME::ObservableConnector::sendReport(Report &r) {
	for(int i = 0; i < this->observers[std::type_index(typeid(r))].size(); i++) {
		this->observers[std::type_index(typeid(r))][i]->receiveReport(r);
	}
}
