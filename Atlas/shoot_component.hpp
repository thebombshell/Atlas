
// shoot_component.hpp

// header file for the shoot component class

#ifndef ATLAS_SHOOT_COMPONENT_HPP
#define ATLAS_SHOOT_COMPONENT_HPP

#include "bullet.hpp"

#include <pantheon.hpp>

namespace atlas {

	class ShootComponent : public pantheon::IUpdatable {

	private:

		float m_shootTimer{ 0.0f };

	public:
		ShootComponent( pantheon::Actor::Key t_key, pantheon::Actor& t_owner )
			: IActorComponent{ t_key, t_owner } {

		}

		~ShootComponent() {

		}

		void update( float t_delta ) {

			if ( m_shootTimer >= 0.0f ) {
				m_shootTimer -= t_delta;
			}
		}

		bool shoot() {

			if ( m_shootTimer <= 0.0f ) {

				pantheon::Actor& owner = getOwner();
				pantheon::Transform& transform = owner.getTransform();
				glm::vec3 up = transform.findUp();
				pantheon::Game::GetScene().createPrefab<Bullet>(
					BulletInfo( owner
						, transform.position + up * 5.0f
						, transform.findUp() * 160.0f ) );
				m_shootTimer = cooldown;
				return true;
			}
			return false;
		}

		float cooldown{ 0.33f };
	};
}

#endif