
// helper_functions.hpp

// header file containing helper functions

#ifndef PANTHEON_HELPER_FUNCTIONS_HPP
#define PANTHEON_HELPER_FUNCTIONS_HPP

#include "pantheon.hpp"

#include <assert.h>
#include <fstream>

namespace pantheon {

	// increments an iterator, looping the iterator when it is equivilent to end

	template<typename T>
	void loopIncrementIterator( T& t_iter, const T t_begin, const T t_end ) {

		++t_iter;
		if ( t_iter == t_end ) {

			t_iter = t_begin;
		}
	}

	// decrements an iterator, looping the iterator when it is equivilent to begin

	template<typename T>
	void loopDecrementIterator( T& t_iter, const T t_begin, const T t_end ) {

		if ( t_iter != t_begin ) {

			--t_iter;
		}
		else {

			t_iter = t_end;
		}
	}

	// increments an iterator, clamping the iterator when it falls out of range

	template<typename T>
	void clampIncrementIterator( T& t_iter, const T t_end ) {

		++t_iter;
		if ( t_iter == t_end ) {

			--t_iter;
		}
	}

	// decrements an iterator, clamping the iterator when it falls out of range

	template<typename T>
	void clampDecrementIterator( T& t_iter, const T t_begin ) {

		if ( t_iter != t_begin ) {

			--t_iter;
		}
	}

	// reads a file and returns its contents as a string

	PANTHEON_API int getFileAsString(
		std::string t_path,
		std::string& t_outContents
		);

}

#endif //PANTHEON_HELPER_FUNCTIONS_HPP