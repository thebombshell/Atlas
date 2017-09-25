
// error.hpp

// Header file of error constants

#ifndef WIREFRAMETDS_ERROR_HPP
#define WIREFRAMETDS_ERROR_HPP

#include "wireframe_tds.hpp"

#include <string>

namespace wireframe_tds {

	// error constants

	const int WTDS_ERROR_OK{ 0 };
	const int WTDS_ERROR_FAILED{ 1 };
	const int WTDS_ERROR_SHADER_COMPILE{ 258 };
	const int WTDS_ERROR_VERTEX_SHADER_COMPILE{ 259 };
	const int WTDS_ERROR_FRAGMENT_SHADER_COMPILE{ 260 };
	const int WTDS_ERROR_PROGRAM_LINK{ 261 };
	const int WTDS_ERROR_COULD_NOT_OPEN_VERTEX_SHADER{ 262 };
	const int WTDS_ERROR_COULD_NOT_OPEN_FRAGMENT_SHADER{ 263 };
	const int WTDS_ERROR_COULT_NOT_INIT_GL{ 264 };
	const int WTDS_ERROR_UNEVEN_ELEMENT_COUNT{ 265 };
	const int WTDS_ERROR_TOO_MANY_ELEMENTS{ 266 };
	const int WTDS_ERROR_GL_VERSION_NOT_SUPPORTED{ 267 };
	const int WTDS_ERROR_UTLS_COULD_NOT_OPEN_FILE{ 512 };

	// returns a string of an error message

	std::string getErrorMessage(
		const int t_error
		);
}

#endif