/*
 * command.hpp
 *
 *  Created on: Oct 26, 2018
 *      Author: cgbri
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include <vector>
#include <map>
#include "../../Design Patterns/design_patterns.hpp"

class Command {
public:
	virtual ~Command() = 0;
	virtual void execute() {
		return;
	}
};

class Receiver {
public:
	virtual ~Receiver() = 0;

	template<typename _C>
	virtual void receive(_C &) = 0;

};

class CommandHandlerFacade : public design_patterns::Singleton<CommandHandlerFacade> {
private:
	std::map<std::type_info, std::vector<Receiver *>> receivers;

public:

	void registerCommandHandler(Receiver *rec, std::type_info type) {
		try {
			this->receivers.at(type).push_back(rec);
		} catch(std::out_of_range &e) {
			std::vector<Receiver *> v;
			this->receivers.insert(std::pair<std::type_info, std::vector<Receiver *>>(type, v));
			this->receivers[type].push_back(rec);
		}
	}

	template<typename _C>
	void handleCommand(_C &command) {
		try {
			for(int i = 0; i < this->receivers.at(typeid(command)).size(); i++) {
				this->receivers[typeid(command)][i]->receive(command);
			}
		} catch(std::out_of_range &e) {
			return ;
		}
	}

};

#endif /* COMMAND_HPP_ */
