
// ship_move_component.hpp

// header fiel for ship movement component class

#ifndef ATLAS_SHIP_MOVE_COMPONENT_HPP
#define ATLAS_SHIP_MOVE_COMPONENT_HPP

#include <pantheon.hpp>

namespace atlas {

	class ShipMoveComponent : public pantheon::IActorComponent {

	private:

		glm::vec2 m_velocity{ 0.0f, 0.0f };
		float m_angularVelocity{ 0.0f };

	public:
		ShipMoveComponent( pantheon::Actor::Key t_key, pantheon::Actor& t_owner )
			: IActorComponent{ t_key, t_owner } {

		}

		~ShipMoveComponent() {

		}

		void update(float t_delta, float t_rotationInput, float t_thrustInput ) {

			pantheon::Transform& transform = getOwner().getTransform();

			m_angularVelocity -= m_angularVelocity * angularDeccelFactor * t_delta;
			m_angularVelocity += t_rotationInput * angularAccelSpeed * t_delta;
			transform.rotation *= glm::angleAxis( m_angularVelocity * t_delta
					, glm::vec3( 0.0f, 0.0f, 1.0f ) );

			m_velocity -= m_velocity * deccelerationFactor * t_delta;
			m_velocity += glm::vec2( transform.findUp() ) * accelerationSpeed * t_thrustInput * t_delta;
			transform.position += glm::vec3( m_velocity, 0.0f ) * t_delta;
		}

		void reset() {

			m_velocity = { 0.0f, 0.0f };
			m_angularVelocity = 0.0f;
		}

		float angularAccelSpeed{ 8.0f };
		float angularDeccelFactor{ 2.0f };

		float accelerationSpeed{ 80.0f };
		float deccelerationFactor{ 0.5f };
	};
}

#endif