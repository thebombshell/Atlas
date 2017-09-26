
// error.cpp

// Source file responsible for error helper functions

#include "error.hpp"

std::string atlas::getErrorMessage(
	const int t_error
	) {

	switch ( t_error ) {
		case WTDS_ERROR_OK:
			return "No error has occured.";
		case WTDS_ERROR_FAILED:
			return "A failure has occured.";
		case WTDS_ERROR_SHADER_COMPILE:
			return "Could not compile shader, see retunred error string for details.";
		case WTDS_ERROR_VERTEX_SHADER_COMPILE:
			return "Could not compile vertex shader, see returned error string for details.";
		case WTDS_ERROR_FRAGMENT_SHADER_COMPILE:
			return "Could not compile fragment shader, see returned error string for details.";
		case WTDS_ERROR_PROGRAM_LINK:
			return "Could not link program, see returned error string for details.";
		case WTDS_ERROR_COULD_NOT_OPEN_VERTEX_SHADER:
			return "Could not open vertex shader file.";
		case WTDS_ERROR_COULD_NOT_OPEN_FRAGMENT_SHADER:
			return "Could not open fragment shader file.";
		case WTDS_ERROR_COULT_NOT_INIT_GL:
			return "Could not initialize an opengl context.";
		case WTDS_ERROR_UNEVEN_ELEMENT_COUNT:
			return "Element count when drawing lines must be divisible by 2, as 2 elements constitute a line.";
		case WTDS_ERROR_TOO_MANY_ELEMENTS:
			return "Too many elements are queued to draw.";
		case WTDS_ERROR_GL_VERSION_NOT_SUPPORTED:
			return "OpenGL version 3.2.0 is not supported by this system.";
		case WTDS_ERROR_UTLS_COULD_NOT_OPEN_FILE:
			return "Could not open file.";
	}
	return "The error is not recognised.";
}