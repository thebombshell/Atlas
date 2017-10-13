
// attourney.hpp

// header file for the attourney accessor key template

#ifndef PANTHEON_KEY_HPP
#define PANTHEON_KEY_HPP

#include <assert.h>

namespace pantheon {

	template<typename T>
	class Attourney {

	private:

		friend typename T;

		Attourney() {

		}

	public:

		~Attourney() {

		}
	};
}

#endif