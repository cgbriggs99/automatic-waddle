/*
 * update_command.hpp
 *
 *  Created on: Oct 24, 2018
 *      Author: connor
 */

#ifndef UPDATE_COMMAND_HPP_
#define UPDATE_COMMAND_HPP_

#include <vector>
#include <iterator>

class UpdateCommandReceiver;

class UpdateCommandObserver {
protected :
	bool needs_update;
public:
	UpdateCommandObserver() {
		needs_update = true;
	}

	virtual ~UpdateCommandObserver() {
		;
	}

	void setUpdate(bool update) {
		this->needs_update = update;
	}
};

class UpdateCommandReceiver {
private:
	std::vector<UpdateCommandObserver *> subjects;
	static UpdateCommandReceiver *singleton;
	UpdateCommandReceiver() {
		;
	}
public:
	static UpdateCommandReceiver *getSingleton() {
		if(singleton == nullptr) {
			singleton = new UpdateCommandReceiver();
		}
		return (singleton);
	}

	void registerObserver(UpdateCommandObserver *obs) {
		subjects.push_back(obs);
	}

	void update() {
		for(int i = 0; i < subjects.size(); i++) {
			subjects[i]->setUpdate(true);
		}
	}
};

class UpdateCommand {
public:
	void execute() {
		UpdateCommandReceiver::getSingleton()->update();
	}
};

#endif /* UPDATE_COMMAND_HPP_ */
