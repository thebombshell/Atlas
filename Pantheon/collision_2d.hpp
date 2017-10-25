
// collision_2d.hpp

// header file for public implementation of the 2d collision manager

#ifndef PANTHEON_COLLISION_HPP
#define PANTHEON_COLLISION_HPP

#include "pantheon.hpp"

#include "iactor_component.hpp"
#include "game.hpp"
#include "shapes_2d.hpp"

#include <vector>

namespace pantheon {

	class Collision2DMessage;
	class Collision2DComponent;
	class Collision2DManager;

	// permissions

	class P_Collision2DRegistry;
	class P_CallPhysics2DIncrement;
	typedef Permission<Collision2DComponent, P_Collision2DRegistry> Collision2DRegistryPermit;
	typedef Permission<Collision2DManager, P_Collision2DRegistry> CallPhysics2DIncrementPermit;

	struct Collision2DMessage : public IActorEventMessage {

	public:

		Collision2DMessage(const std::vector<Collision2D>& t_collisions
			, Actor& t_other ) : collisions{ t_collisions }, other{ t_other } {

		}

		~Collision2DMessage() {

		}

		std::vector<Collision2D> collisions;
		Actor& other;

	};

	class PANTHEON_API Collision2DComponent : public IActorComponent {

	public:

		Collision2DComponent(ConstructComponentPermit&, Actor& t_owner);
		~Collision2DComponent();

		void addCollider( ICollider2D* const t_collider );
		void removeCollider( ICollider2D* const t_collider );
		void setCollisionFlags( unsigned int t_flags );
		unsigned int getCollissionFlags();
		void setCollideWithFlags( unsigned int t_flags);
		unsigned int getCollideWithFlags();
		const std::vector<ICollider2D*>& getColliders() const;

	private:

		unsigned int m_collisionFlags{ 0xffffffff };
		unsigned int m_collideWithFlags{ 0xffffffff };
		std::vector<ICollider2D*> m_colliders;

	};

	class PANTHEON_API Collision2DManager : public IGameCollision {
	private:

		class CollisionImpl;
		CollisionImpl* m_collision{ nullptr };

		Collision2DManager( ConstructCollisionPermit& );
		~Collision2DManager();

	public:

		static IGameCollision* createInstance( ConstructCollisionPermit& );

		void simulate( float t_delta ) override;

		// register and unregister collision components

		void registerComponent( Collision2DRegistryPermit&
			, Collision2DComponent* const t_actor );
		void unregisterComponent( Collision2DRegistryPermit&
			, Collision2DComponent* const t_actor );

	};
}

#endif