
// actor.hpp

// header file for the public implementation of the actor class

#ifndef PANTHEON_ACTOR_HPP
#define PANTHEON_ACTOR_HPP

#include "pantheon.hpp"

#include "permission.hpp"
#include "transform.hpp"

#include <algorithm>
#include <map>
#include <typeindex>

namespace pantheon {

	typedef std::pair<const std::type_index, IActorComponent*> ActorComponentPair;
	typedef std::map<std::type_index, IActorComponent*>::iterator ActorComponentIterator;

	// permissions

	class P_ConstructComponent;
	typedef Permission<Actor, P_ConstructComponent> ConstructComponentPermit;

	struct IActorEventMessage {

	public:
		
		IActorEventMessage() {

		}
		
		virtual ~IActorEventMessage() = 0 {

		}

		template<typename T>
		T* const as() {

			return dynamic_cast<T* const>(this);
		}

	};

	class PANTHEON_API Actor {

	private:

		class ActorImpl;
		ActorImpl* m_actor;
		
		void addComponent( std::type_index t_typeId
			, IActorComponent* t_component );

		bool hasComponent( std::type_index t_typeId ) const;

		IActorComponent& getComponent( std::type_index t_typeId ) const;

		IActorComponent* removeComponent( std::type_index t_typeId );

		ActorComponentIterator begin() const;
		ActorComponentIterator end() const;

		bool m_isActive{ false };
		Transform m_transform;

	public:

		Actor();
		~Actor();

		template<typename T>
		void createComponent() {

			ConstructComponentPermit permit;
			addComponent( typeid(T), new T( permit, *this ) );
		}
		template<typename T, typename P>
		void createComponent( P t_properties ) {

			ConstructComponentPermit permit;
			addComponent( typeid(T), new T( permit, *this, t_properties ) );
		}

		template<typename T>
		bool hasComponent() const {

			return hasComponent( typeid(T) );
		}

		template<typename T>
		T& getComponent() const {

			return dynamic_cast<T&>(getComponent( typeid(T) ));
		}

		template<typename T>
		void destroyComponent() {

			delete ( removeComponent( typeid(T) ) );
			
		}

		template<typename F>
		void forEachComponent(F t_func) {

			std::for_each( begin(), end(), 
				[&]( ActorComponentPair& t_pair ) {

				IActorComponent& component = *t_pair.second;
				t_func( component );
			} );
		}

		void disable() {

			m_isActive = false;
		}

		void enable() {

			m_isActive = true;
		}

		bool isActive() {

			return m_isActive;
		}

		// sends event message to all the actors components
		// potentially expensive, only use for actor specific events

		template<typename T>
		void dispatchEvent( const T& t_message ) {

			T* message = new T( t_message );
			forEachComponent( [&]( IActorComponent& t_component ) {

				t_component.onEventMessage( message );
			} );
			delete message;
		}

		Transform& getTransform() {

			return m_transform;
		}
	};
}

#endif PANTHEON_ACTOR_HPP