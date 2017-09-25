
// game_actor.hpp

// Header file for game actor and event interfaces

#ifndef WIREFRAMETDS_GAME_ACTOR_HPP
#define WIREFRAMETDS_GAME_ACTOR_HPP

#include "wireframe_tds.hpp"
#include "forward_declaration.hpp"
#include "collisions.hpp"

namespace wireframe_tds {

	const int WTDS_ACTOR_DISABLED{ 0 };
	const int WTDS_ACTOR_ENABLED{ 1 };

	// interface for game actor

	class IGameActor {
	private:
		friend class Game;

		int m_state{ WTDS_ACTOR_DISABLED };

		void setState(
			int t_state ) {

			m_state = t_state;
		}
	public:

		// ensure abstract

		virtual ~IGameActor(
			) = 0;

		int getState(
			) {

			return m_state;
		}

	};

	// destructor definition

	inline IGameActor::~IGameActor(
		) {

	}

	// interface for onCollision

	class IGameCollidable : public virtual IGameActor {
	public:

		// ensure abstract

		virtual ~IGameCollidable(
			) = 0;

		// returns anonymous collider

		virtual const collisions::ICollider* getCollider(
			) const = 0;

		// called on collision

		virtual void onCollision(
			const glm::vec3& t_axis,
			IGameActor& t_other
			) = 0;
	};

	// definition

	inline IGameCollidable::~IGameCollidable(
		) {

	}

	inline void IGameCollidable::onCollision(
		const glm::vec3&,
		IGameActor&
		) {

	}

	// interface for onRender

	class IGameRenderable : public virtual IGameActor {
	public:

		// ensure abstract

		virtual ~IGameRenderable(
			) = 0;

		// called before rendering

		virtual void onRender(
			float delta
			) const = 0;
	};

	// destructor definition

	inline IGameRenderable::~IGameRenderable(
		) {

	}

	inline void IGameRenderable::onRender(
		float
		) const {

	}

	// interface for onUpdate

	class IGameUpdatable : public virtual IGameActor {
	public:

		// ensure abstract

		virtual ~IGameUpdatable(
			) = 0;

		// called on update

		virtual void onUpdate( 
			float delta 
			) = 0;
	};

	// destructor definition

	inline IGameUpdatable::~IGameUpdatable(
		) {

	}

	inline void IGameUpdatable::onUpdate(
		float
		) {

	}
}

#endif