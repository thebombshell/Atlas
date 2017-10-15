
// explode_component.hpp

// header file for the explosion component class

#ifndef ATLAS_EXPLODE_COMPONENT_HPP
#define ATLAS_EXPLODE_COMPONENT_HPP

#include <pantheon.hpp>

namespace atlas {

	struct ExplodeInfo {

	public:

		ExplodeInfo( glm::vec3 t_position, glm::vec2 t_direction, float t_scale ) 
			: position{ t_position }, direction { t_direction }, scale{ t_scale } {

		}

		glm::vec3 position;
		glm::vec2 direction;
		float scale;

	};

	class ExplodeComponent : public pantheon::IUpdatable, pantheon::IRenderable {

	private:

		pantheon::Source* m_source;
		float m_timer{ 0.0f };
		glm::vec2 m_velocity;
		float m_scale;

	public:
		ExplodeComponent( pantheon::ConstructComponentPermit& t_permit
			, pantheon::Actor& t_owner, const ExplodeInfo& t_info )
			: IActorComponent{ t_permit, t_owner }
			, m_velocity{ t_info.direction }, m_scale{ t_info.scale } {

			pantheon::Audio& audio = pantheon::Game::GetAudio();
			if ( !audio.hasSound( "explode" ) ) {

				audio.loadSound( "audio/explode-01.wav", "explode" );
			}
			m_source = audio.createSource( "explode" );
			m_source->play();
			getOwner().getTransform().position = t_info.position;
		}

		~ExplodeComponent() {

			pantheon::Game::GetAudio().deleteSource( m_source );
		}

		void update( float t_delta ) {

			m_timer += t_delta;
			if ( m_timer > 3.0f ) {

				pantheon::Game::GetScene().destroyActor( &getOwner() );
			}
		}

		void render();
	};
}

#endif