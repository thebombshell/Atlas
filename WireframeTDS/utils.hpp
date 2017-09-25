
// utils.hpp

// Header file containing a number of utility and quality of life functions.

#ifndef WIREFRAMETDS_UTILS_HPP
#define WIREFRAMETDS_UTILS_HPP

#include "wireframe_tds.hpp"
#include <string>

namespace wireframe_tds {


	void showMessage(
		const std::string t_message
		);

	const int getFileAsString(
		std::string t_path,
		std::string& t_outContents
		);
}

#endif