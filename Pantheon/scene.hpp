
// scene.hpp

// header file for the actor collection

#ifndef PANTHEON_SCENE_HPP
#define PANTHEON_SCENE_HPP

#include "pantheon.hpp"

#include "actor.hpp"

#include <algorithm>
#include <vector>

namespace pantheon {

	class PANTHEON_API Scene {
	private:

		std::vector<Actor*> m_addQueue{ };
		std::vector<Actor*> m_removeQueue{ };
		std::vector<Actor*> m_actors{ };

	public:

		Scene() {

		}
		~Scene() {

		}

		// creates an actor and adds it to scene
		
		Actor& createActor();

		template<typename T>
		Actor& createPrefab() {

			Actor& actor = createActor();
			actor.createComponent<T>();
			actor.enable();
			return actor;
		}

		template<typename T, typename P>
		Actor& createPrefab( P t_properties ) {

			Actor& actor = createActor();
			actor.createComponent<T>( t_properties );
			actor.enable();
			return actor;
		}

		// removes an actor from scene and destroys it

		void destroyActor( Actor* const t_actor );

		// processes and then empties add and remove queues

		void update();

		// calls a callback function passing each actor in the scene

		template<typename F>
		void forEach( F t_callback ) {

			update();
			std::for_each( m_actors.begin(), m_actors.end(), t_callback );
		}
	};
}

#endif