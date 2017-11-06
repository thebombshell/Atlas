
// shoot_component.hpp

// header file for the shoot component class

#ifndef ATLAS_SHOOT_COMPONENT_HPP
#define ATLAS_SHOOT_COMPONENT_HPP

#include "bullet.hpp"

#include <pantheon.hpp>

namespace atlas {

	class ShootComponent : public pantheon::IUpdatable {

	private:

		pantheon::Source* m_source;
		float m_shootTimer{ 0.0f };

	public:
		ShootComponent( pantheon::ConstructComponentPermit& t_permit, pantheon::Actor& t_owner )
			: IActorComponent{ t_permit , t_owner } {

			pantheon::Audio& audio = pantheon::Game::GetAudio();
			if ( !audio.hasSound( "shoot" ) ) {

				audio.loadSound( "audio/shoot.wav", "shoot" );
			}
			m_source = audio.createSource( "shoot" );
		}

		~ShootComponent() {

			pantheon::Game::GetAudio().deleteSource( m_source );
		}

		void update( float t_delta ) {

			if ( m_shootTimer >= 0.0f ) {
				m_shootTimer -= t_delta;
			}
		}

		bool shoot( glm::vec3 t_colour, unsigned int t_flag ) {

			if ( m_shootTimer <= 0.0f ) {

				pantheon::Actor& owner = getOwner();
				pantheon::Transform& transform = owner.getTransform();
				glm::vec3 up = transform.findUp();
				pantheon::Game::GetScene().createPrefab<Bullet>(
					BulletInfo( owner
						, transform.position + up * transform.scale * 5.0f
						, transform.findUp() * 160.0f, t_colour, t_flag ) );
				m_shootTimer = cooldown;
				m_source->play();
				return true;
			}
			return false;
		}

		float cooldown{ 0.33f };
	};
}

#endif