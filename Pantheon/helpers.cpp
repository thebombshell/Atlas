
// helpers.cpp

// source file for helpers

#include "helpers.hpp"

#include <fstream>

using namespace pantheon;

std::string pantheon::getFileAsString( std::string t_path ) {

	std::ifstream file;
	file.exceptions( std::ifstream::badbit | std::ifstream::failbit | std::ifstream::eofbit );
	file.open( t_path );
	std::string contents(
		(std::istreambuf_iterator<char>( file )),
		(std::istreambuf_iterator<char>()) );
	return contents;

}