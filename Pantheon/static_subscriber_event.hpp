
// static_subscriber_event.hpp

// the header file for the static subscriber event tempalte

#ifndef PANTHEON_STATIC_SUBSCRIBER_EVENT_HPP
#define PANTHEON_STATIC_SUBSCRIBER_EVENT_HPP

#include <algorithm>
#include <vector>

namespace pantheon {


	template<typename T>
	class StaticSubscriptionEvent {

	private:

		std::vector<T*> m_addQueue;
		std::vector<T*> m_removeQueue;
		std::vector<T*> m_subscribers;

		StaticSubscriptionEvent() {

		}
		~StaticSubscriptionEvent() {

		}

		static StaticSubscriptionEvent& get() {

			static StaticSubscriptionEvent instance;
			return instance;
		}

		static void update() {

			std::vector<T*>& addQueue = get().m_addQueue;
			std::vector<T*>& removeQueue = get().m_removeQueue;
			std::vector<T*>& subscribers = get().m_subscribers;
			subscribers.insert( subscribers.end(), addQueue.begin(), addQueue.end() );
			addQueue.clear();

			for( auto subscriber : removeQueue ) {

				auto iter = std::find( subscribers.begin(), subscribers.end()
					, subscriber );
				subscribers.erase( iter );
			}
			removeQueue.clear();
		}

	public:

		static void add( T* const t_element ) {

			get().m_addQueue.push_back( t_element );
		}

		static void remove( T* const t_element ) {

			get().m_removeQueue.push_back( t_element );
		}

		template<typename F>
		static void forEach( F t_func ) {

			update();
			std::for_each( get().m_subscribers.begin(), get().m_subscribers.end(), t_func );
		}
	};
}
#endif