
// physics_component.hpp

#ifndef PANTHEON_PHYSICS_COMPONENT_HPP
#define PANTHEON_PHYSICS_COMPONENT_HPP

#include "pantheon.hpp"

#include "collision_2d.hpp"
#include "iactor_component.hpp"

namespace pantheon {

	// physics flags

	const int PHYSICS_IS_SOLID{ 1 };
	const int PHYSICS_IS_KINEMATIC{ 2 };
	const int PHYSICS_IS_GRAVITY_ENABLED{ 4 };

	class PANTHEON_API PhysicsComponent2D : public IActorComponent {

	public:

		PhysicsComponent2D( ConstructComponentPermit&, Actor& );

		~PhysicsComponent2D();

		void increment( float t_delta );

		void onEventMessage( pantheon::IActorEventMessage* const ) override;

		void setSolid();
		void setSoft();
		void setKinematic();
		void setDynamic();
		void setGravityEnabled();
		void setGravityDisabled();
		bool isSolid();
		bool isKinematic();
		bool isGravityEnabled();

		static void call( CallPhysics2DIncrementPermit&, float t_delta );
		static void setGravity(const glm::vec2& t_newGravity);
		static glm::vec2 getGravity();

		float mass{ 1.0f };
		float friction{ 0.0f };
		float bounce{ 0.0f };
		glm::vec2 velocity{ 0.0f, 0.0f };
		float angularVelocity{ 0.0f };

	private:

		void onCollision( const glm::vec2& t_resolve );

		int m_flags{ 0 };

		static glm::vec2 g_gravity;
	};
}

#endif