
// helpers.cpp

// source file for helpers

#include "helpers.hpp"

using namespace pantheon;

int pantheon::getFileAsString
	( std::string t_path
	, std::string& t_outContents ) {

	std::ifstream file;
	file.open( t_path );
	if ( file.is_open() ) {
		std::string contents(
			(std::istreambuf_iterator<char>( file )),
			(std::istreambuf_iterator<char>()) );
		t_outContents = contents;
	}
	else {
		return PANTHEON_ERROR;
	}

	return PANTHEON_OK;
}