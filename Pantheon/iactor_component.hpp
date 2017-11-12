
// iactor_component.hpp

// header file for the public implementation of the actor component interface

#ifndef PANTHEON_IACTOR_COMPONENT_HPP
#define PANTHEON_IACTOR_COMPONENT_HPP

#include "pantheon.hpp"

#include "actor.hpp"
#include "game.hpp"

#include <typeinfo>

namespace pantheon {

	class PANTHEON_API IActorComponent {

	private:

		bool m_isActive{ true };
	protected:

		Actor& m_owner;
	public:

		IActorComponent( ConstructComponentPermit& t_permit, Actor& t_owner ) : m_owner{ t_owner } {

			t_permit.use();
		}

		virtual ~IActorComponent() = 0 {

		}

		virtual void onEventMessage( IActorEventMessage* const ) {

		}

		void disable();
		void enable();
		virtual void onDisable();
		virtual void onEnable();
		bool isActive();
		Actor& getOwner();
	};

	class PANTHEON_API IUpdatable : public virtual IActorComponent {

	public:

		IUpdatable();

		~IUpdatable();

		virtual void update( float t_delta ) = 0;

		static void call( CallUpdatablesPermit&, float t_delta );
	};

	class PANTHEON_API  IRenderable : public virtual IActorComponent {

	public:

		IRenderable();

		~IRenderable();

		virtual void render() = 0;

		static void call( CallRenderablesPermit& );
	};
}

#endif