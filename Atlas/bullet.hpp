
// bullet.hpp

// header file for the public implementation of the bullet class

#ifndef ATLAS_BULLET_HPP
#define ATLAS_BULLET_HPP

#include <pantheon.hpp>
#include <shapes_2d.hpp>

namespace atlas {

	struct BulletInfo {

	public:

		BulletInfo( pantheon::Actor& t_firer, glm::vec2 t_position, glm::vec2 t_velocity, glm::vec3 t_colour )
			: firer( t_firer ), position( t_position ), velocity( t_velocity ), colour{ t_colour } {

		}

		pantheon::Actor& firer;
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec3 colour;
	};

	class Bullet : public pantheon::IUpdatable, pantheon::IRenderable {

	private:

		pantheon::Actor& m_firer;
		glm::vec2 m_velocity;
		glm::vec3 m_colour;
		float m_timer{ 0.0f };
		pantheon::Circle m_collider;

	public:

		Bullet( pantheon::ConstructComponentPermit&, pantheon::Actor&, const BulletInfo& t_message );
		~Bullet();

		void update( float ) override;

		void render( void ) override;

		void onEventMessage( pantheon::IActorEventMessage* const ) override;

	};
}

#endif