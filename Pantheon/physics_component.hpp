
// physics_component.hpp

#ifndef PANTHEON_PHYSICS_COMPONENT_HPP
#define PANTHEON_PHYSICS_COMPONENT_HPP

#include "pantheon.hpp"

#include "collision_2d.hpp"
#include "iactor_component.hpp"
#include "permission.hpp"

#include <vector>

namespace pantheon {

	// forward decclaration

	class PhysicsComponent2D;

	// Permissions

	class P_ConstructPhysicsCollisionMessage2D;
	typedef Permission<PhysicsComponent2D, P_ConstructPhysicsCollisionMessage2D> ConstructPhysicsCollisionMessage2DPermit;

	// physics flags

	const int PHYSICS_IS_SOLID{ 1 };
	const int PHYSICS_IS_KINEMATIC{ 2 };
	const int PHYSICS_IS_GRAVITY_ENABLED{ 4 };

	// physics collision message

	class PANTHEON_API PhysicsCollisionMessage2D : public IActorEventMessage {

	public:
		PhysicsCollisionMessage2D( ConstructPhysicsCollisionMessage2DPermit&,
			const glm::vec2& t_resolve, const glm::vec2& t_force, PhysicsComponent2D& t_other );

		glm::vec2 resolve;
		glm::vec2 force;
		PhysicsComponent2D& other;
	};

	class PANTHEON_API PhysicsComponent2D : public IActorComponent {

	public:

		PhysicsComponent2D( ConstructComponentPermit&, Actor& );

		~PhysicsComponent2D();

		void increment( float t_delta );

		void processCollisions( const std::vector<CollisionGroup2D>& t_collisions );

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
		float airResistance{ 0.0f };
		float bounce{ 0.0f };
		glm::vec2 velocity{ 0.0f, 0.0f };
		float angularVelocity{ 0.0f };

	private:

		void onCollisionWithKinematic( const glm::vec2& t_resolve, const glm::vec2& t_axis, PhysicsComponent2D& t_other );
		void onCollisionWithDynamic( const glm::vec2& t_resolve, PhysicsComponent2D& t_other );
		int m_flags{ 0 };
		glm::vec2 m_velocitySnapshot{ 0.0f, 0.0f };

		static glm::vec2 g_gravity;
	};
}

#endif