
// default_level.hpp

// header file of the public implementation of the default level actor

#ifndef ATLAS_DEFAULT_LEVEL_HPP
#define ATLAS_DEFAULT_LEVEL_HPP

#include <pantheon.hpp>

namespace atlas {

	class DefaultLevel : public pantheon::IUpdatable, public pantheon::IRenderable {

	private:

		pantheon::Transform m_transform{ };
	public:

		DefaultLevel( pantheon::Actor::Key, pantheon::Actor& );
		~DefaultLevel();

		void update( float t_delta ) override;

		void render( void ) override;

	};
}

#endif