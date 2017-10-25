
// explode_component.hpp

// header file for the explosion component class

#ifndef ATLAS_EXPLODE_COMPONENT_HPP
#define ATLAS_EXPLODE_COMPONENT_HPP

#include <pantheon.hpp>

namespace atlas {

	struct ExplodeInfo {

	public:

		ExplodeInfo( pthn::Transform t_transform, glm::vec2 t_velocity ) 
			: transform{ t_transform }, velocity{ t_velocity } {

		}

		pthn::Transform transform;
		glm::vec2 velocity;

	};

	class ExplodeComponent : public pthn::IUpdatable, pthn::IRenderable {

	private:

		pthn::Source* m_source;
		float m_timer{ 0.0f };
		glm::vec2 m_velocity;
		glm::vec2 m_vertices[4];
		glm::vec2 m_normals[4];
		float m_scale;

	public:
		ExplodeComponent( pthn::ConstructComponentPermit& t_permit
			, pthn::Actor& t_owner, const ExplodeInfo& t_info )
			: IActorComponent{ t_permit, t_owner }
			, m_velocity{ t_info.velocity } {

			pthn::Audio& audio = pthn::Game::GetAudio();
			if ( !audio.hasSound( "explode" ) ) {

				audio.loadSound( "audio/explode-01.wav", "explode" );
			}
			m_source = audio.createSource( "explode" );
			m_source->play(); 
			setupVertices();
			getOwner().getTransform() = t_info.transform;
		}

		~ExplodeComponent() {

			pthn::Game::GetAudio().deleteSource( m_source );
		}

		void setupVertices();

		void update( float t_delta ) {

			m_timer += t_delta;
			if ( m_timer > 3.0f ) {

				pthn::Game::GetScene().destroyActor( &getOwner() );
			}
		}

		void render();
	};
}

#endif