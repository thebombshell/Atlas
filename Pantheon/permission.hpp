
// attourney.hpp

// header file for the attourney accessor key template

#ifndef PANTHEON_KEY_HPP
#define PANTHEON_KEY_HPP

#include <assert.h>

namespace pantheon {

	template<typename T, typename T_Permit>
	class Permission {

	public:

		Permission( const Permission& ) = delete;

		~Permission() {

		}

		void use() {

			assert( m_uses != 0 && "No further use is permitted" );
			--m_uses;
		}

	private:

		friend typename T;

		Permission() {

		}

		Permission( unsigned int t_uses ) : m_uses{ t_uses } {

		}

		unsigned int m_uses{ 1 };

	};
}

#endif