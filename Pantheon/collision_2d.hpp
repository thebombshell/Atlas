
// collision_2d.hpp

// header file for public implementation of the 2d collision manager

#ifndef PANTHEON_COLLISION_HPP
#define PANTHEON_COLLISION_HPP

#include "pantheon.hpp"

#include "attourney.hpp"
#include "iactor_component.hpp"
#include "game.hpp"
#include "shapes_2d.hpp"

#include <vector>

namespace pantheon {

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

	private:
		
		std::vector<ICollider2D*> m_colliders;
	public:

		Collision2DComponent( Actor::Key t_key, Actor& t_owner );
		~Collision2DComponent();

		void addCollider( ICollider2D* const t_collider );
		void removeCollider( ICollider2D* const t_collider );
		const std::vector<ICollider2D*>& getColliders() const;

		typedef Attourney<Collision2DComponent> Key;

	};

	class PANTHEON_API Collision2DManager : public IGameCollision {
	private:

		class CollisionImpl;
		CollisionImpl* m_collision{ nullptr };

		Collision2DManager();
		~Collision2DManager();

	public:

		static IGameCollision* createInstance();

		void simulate() override;

		// register and unregister collision components

		void registerComponent( Collision2DComponent::Key
			, Collision2DComponent* const t_actor );
		void unregisterComponent( Collision2DComponent::Key
			, Collision2DComponent* const t_actor );

		typedef Attourney<Collision2DManager> Key;
	};
}

#endif