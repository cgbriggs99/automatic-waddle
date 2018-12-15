/*
 * Report.hpp
 *
 *  Created on: Nov 5, 2018
 *      Author: cgbri
 */

#ifndef REPORT_HPP_
#define REPORT_HPP_

#include "base.hpp"
#include "../namespace_name.hpp"

namespace NAME {

class Report {
public:
	virtual ~Report() {};

	virtual std::type_index getType() = 0;
};

}

#endif /* REPORT_HPP_ */
