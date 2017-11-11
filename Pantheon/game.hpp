
// game.hpp

// header for public implementation of game class

#ifndef PANTHEON_GAME_HPP
#define PANTHEON_GAME_HPP

#include "pantheon.hpp"

#include "permission.hpp"
#include "audio.hpp"
#include "input.hpp"
#include "scene.hpp"

namespace pantheon {

	// permissions

	class P_ConstructRenderer;
	class P_ConstructCollision;
	class P_CallUpdatables;
	class P_CallRenderables;
	class P_AccessGl;
	typedef Permission<Game, P_ConstructRenderer> ConstructRendererPermit;
	typedef Permission<Game, P_ConstructCollision> ConstructCollisionPermit;
	typedef Permission<Game, P_CallUpdatables> CallUpdatablesPermit;
	typedef Permission<Game, P_CallRenderables> CallRenderablesPermit;
	typedef Permission<IGameRenderer, P_AccessGl> AccessGlPermit;

	// returns the logic threads time delta
	
	float getTimeDelta();

	// returns the logic threads time alpha

	float getTimeAlpha();

	// interface for a renderer

	class PANTHEON_API IGameRenderer {

	public:

		typedef IGameRenderer* (*CreateInstance)( ConstructRendererPermit& );

		IGameRenderer( ConstructRendererPermit& t_permit ) {

			t_permit.use();
		}

		virtual ~IGameRenderer() = 0 {

		}

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

		typedef IGameCollision* (*CreateInstance)(ConstructCollisionPermit&);

		IGameCollision( ConstructCollisionPermit& t_permit ) {

			t_permit.use();
		}

		virtual ~IGameCollision() = 0 {

		}

		// assuming exclusive access to actors, simulate collisions

		virtual void simulate( float t_delta ) = 0;

	};

	// public implementation of game

	class PANTHEON_API Game {

	private:

		Game();
		~Game();

		// private implementation class

		class GameImpl;
		GameImpl* m_game{ nullptr };
		ConstructCollisionPermit m_collisionPermit;
		ConstructRendererPermit m_rendererPermit;

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
			, IGameRenderer::CreateInstance t_rendererCreateFunc
			, IGameCollision::CreateInstance t_collisionCreateFunc );
		static void Start
			( void( *t_gameStartFunc )()
				, IGameRenderer::CreateInstance t_rendererCreateFunc
				, IGameCollision::CreateInstance t_collisionCreateFunc ) {

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

		Audio& getAudio() const;
		static Audio& GetAudio() {

			return getInstance().getAudio();
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

			return dynamic_cast<T*>(&(getInstance().getRenderer()));
		}

		IGameCollision& Game::getCollisionManager();
		static IGameCollision& Game::GetCollisionManager() {

			return getInstance().getCollisionManager();
		}
		template<typename T>
		static T* const GetCollisionManagerAs() {

			return dynamic_cast<T*>(&(getInstance().getCollisionManager()));
		}

		Scene& getScene();
		static Scene& GetScene() {

			return getInstance().getScene();
		}

	};
}

#endif // PANTHEON_GAME_HPP