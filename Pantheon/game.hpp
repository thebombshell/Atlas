
// game.hpp

// header for public implementation of game class

#ifndef PANTHEON_GAME_HPP
#define PANTHEON_GAME_HPP

#include "pantheon.hpp"

#include "attourney.hpp"
#include "input.hpp"
#include "scene.hpp"

namespace pantheon {

	// interface for a renderer

	class PANTHEON_API IGameRenderer {

	public:

		// given exclusive access to the rendering context, render the scene

		virtual void render() = 0;

		// before actors get exclusive access to rendering context

		virtual void beforeActorsDraw() = 0;

		// after actors get exclusive access to rendering context

		virtual void afterActorsDraw() = 0;

	};

	// interface for a collision manager

	class PANTHEON_API IGameCollision {

	public:

		// assuming exclusive access to actors, simulate collisions

		virtual void simulate() = 0;

	};

	// public implementation of game

	class PANTHEON_API Game {

	private:

		Game();
		~Game();

		// private implementation class

		class GameImpl;
		GameImpl* m_game{ nullptr };

	public:

		// returns a singleton instance of Game

		static Game& getInstance() {

			static Game instance;
			return instance;
		}

		int getError();
		static int GetError() {

			return getInstance().getError();
		}

		// enters the event loop and starts rendering and logic threads

		void start
			( void( *t_gameStartFunc )( )
			, IGameRenderer*( *t_rendererCreateFunc )( )
			, IGameCollision*( *t_collisionCreateFunc )( ) );
		static void Start
			( void( *t_gameStartFunc )( )
			, IGameRenderer*( *t_rendererCreateFunc )( )
			, IGameCollision*( *t_collisionCreateFunc )( ) ) {

			getInstance().start( t_gameStartFunc, t_rendererCreateFunc
				, t_collisionCreateFunc );
		}
		template<typename T_RENDERER, typename T_COLLISION>
		static void Start( void( *t_gameStartFunc )( ) ) {

			getInstance().start
				( t_gameStartFunc
				, T_RENDERER::createInstance
				, T_COLLISION::createInstance );
		}

		// tells game to stop running, threads will conclude work on next cycle

		void stop();
		static void Stop() {

			getInstance().stop();
		}

		// get management objects

		IGameCollision& Game::getCollisionManager();
		static IGameCollision& Game::GetCollisionManager() {

			return getInstance().getCollisionManager();
		}

		const Input& getInput() const;
		static const Input& GetInput() {

			return getInstance().getInput();
		}

		IGameRenderer& getRenderer();
		static IGameRenderer& GetRenderer() {

			return getInstance().getRenderer();
		}
		template<typename T>
		static T* const GetRendererAs() {

			return dynamic_cast<T* const>(&getInstance().getRenderer());
		}

		Scene& getScene();
		static Scene& GetScene() {

			return getInstance().getScene();
		}

		typedef Attourney<Game> Key;
	};
}

#endif // PANTHEON_GAME_HPP