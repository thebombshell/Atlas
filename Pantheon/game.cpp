
// game.cpp

// source file for the game class

#include "game.hpp"

#include "iactor_component.hpp"

#define GLEW_DYNAMIC
#include <algorithm>
#include <GL\glew.h>
#include <SDL.h>
#include <set>

using namespace pantheon;

const int GAME_STATE_UNINITIALIZED{ 0 };
const int GAME_STATE_INITIALIZED{ 1 };
const int GAME_STATE_RUNNING{ 2 };

const int DRAW_STATE_CLEAN{ 0 };
const int DRAW_STATE_PENDING{ 1 };

class Game::GameImpl {

	friend class Game;

	GameImpl() {

		// initialize, make error knowable if occured

		if ( initialize() == PANTHEON_ERROR ) {

			m_error = PANTHEON_ERROR;
		}
	}

	~GameImpl() {

		// cleanup everything from initialization

		finalize();
	}

	// initializes the dependencies of Game

	int initialize() {

		// initialize sdl

		int isError = SDL_Init( SDL_INIT_EVERYTHING );
		if ( isError ) {

			SDL_Log( "SDL did not initialize correctly: %s", SDL_GetError() );
			return PANTHEON_ERROR;
		}

		// initialize gl attributes

		isError = initializeGlAttributes();
		if ( isError ) {

			SDL_Log( "An error has occured and rendering could not be initialized" );
			return PANTHEON_ERROR;
		}

		// initialize window and gl contexts

		m_window = SDL_CreateWindow
		( "Pantheon Hidden Event Window"
			, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED
			, 1280, 720
			, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

		m_context = SDL_GL_CreateContext( m_window );
		m_eventContext = SDL_GL_CreateContext( m_window );

		// create and render mutex

		m_renderMutex = SDL_CreateMutex();

		// create supporting objects

		m_input = new Input();
		m_scene = new Scene();

		m_gameState = GAME_STATE_INITIALIZED;

		return PANTHEON_OK;
	}

	// cleans up the dependencies of game

	void finalize() {
		
		if ( !m_error ) {

			m_gameState = GAME_STATE_UNINITIALIZED;

			// delete supporting objects

			delete m_input;
			delete m_scene;

			// shutdown sdl

			SDL_GL_DeleteContext( m_context );
			SDL_GL_DeleteContext( m_eventContext );
			SDL_Quit();
		}
	}

	// initialize pre window gl attributes

	int initializeGlAttributes() {

		// set gl context settings

		int isError = SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		if ( isError ) {

			SDL_Log( "Could not set gl major version." );
			return PANTHEON_ERROR;
		}
		isError = SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 0 );
		if ( isError ) {

			SDL_Log( "Could not set gl minor version." );
			return PANTHEON_ERROR;
		}
		isError = SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
		if ( isError ) {

			SDL_Log( "Could not set gl profile." );
			return PANTHEON_ERROR;
		}

		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
		SDL_GL_SetAttribute( SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1 );

		return PANTHEON_OK;
	}

	// the member logic thread function

	void logicThread() {

		m_collision = m_collisionCreateFunc();
		m_gameStartFunc();

		unsigned long long previousFrameTicks{ SDL_GetPerformanceCounter() };
		while ( m_gameState == GAME_STATE_RUNNING ) {

			unsigned long long  currentFrameTicks{ SDL_GetPerformanceCounter() };
			float timeDelta = (currentFrameTicks - previousFrameTicks) / static_cast<float>(SDL_GetPerformanceFrequency());
			previousFrameTicks = currentFrameTicks;

			m_scene->update();

			IUpdatable::call( { }, timeDelta );

			m_collision->simulate();

			// try to get exclusive renderer access

			if ( m_renderer != nullptr && SDL_TryLockMutex( m_renderMutex ) == 0 ) {

				m_renderer->beforeActorsDraw();

				IRenderable::call( { } );

				m_renderer->afterActorsDraw();

				// tell renderer

				m_hasRendered = false;

				// return renderer access

				SDL_UnlockMutex( m_renderMutex );
			}
			else {

				SDL_Delay( 2 );
			}
		}

	}

	// the static logic thread function

	static int logicThreadFunc( void* t_data ) {

		GameImpl* game = static_cast<GameImpl*>( t_data );
		game->logicThread();
		return 0;
	}

	// the member render thread function

	void renderThread() {

		SDL_GL_MakeCurrent( m_window, m_context );

		int isError = glewInit();
		if ( isError ) {

			SDL_Log
			( "Glew did not initialize correctly: %s"
				, glewGetErrorString( isError ) );
			return;
		}

		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glViewport( 0, 0, 1280, 720 );

		// initialize renderer

		m_renderer = m_rendererCreateFunc();

		while ( m_gameState == GAME_STATE_RUNNING ) {

			// if has not rendered, render, if has rendered no render is necassary

			if ( !m_hasRendered ) {

				SDL_LockMutex( m_renderMutex );

				// start frame

				glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

				// run renderer

				m_renderer->render();

				// swap buffers

				SDL_GL_SwapWindow( m_window );
				m_hasRendered = true;

				// end frame

				SDL_UnlockMutex( m_renderMutex );
			}
			
			SDL_Delay( 0 );
		}

		delete m_renderer;
	}

	// the static render thread function

	static int renderThreadFunc( void* t_data ) {

		GameImpl* game = static_cast<GameImpl*>(t_data);
		game->renderThread();
		return 0;
	}

	// starts the logic loop and enters the event loop

	void start
		( void(*t_gameStartFunc)()
		, IGameRenderer*(*t_rendererCreateFunc)()
		, IGameCollision*(*t_collisionCreateFunc)()) {

		m_gameStartFunc = t_gameStartFunc;
		m_rendererCreateFunc = t_rendererCreateFunc;
		m_collisionCreateFunc = t_collisionCreateFunc;

		// set is running

		m_gameState = GAME_STATE_RUNNING;

		// start logic thread
		
		m_logicThread = SDL_CreateThread( logicThreadFunc, "Pantheon Logic Thread", this );

		// start render thread

		m_renderThread = SDL_CreateThread( renderThreadFunc, "Pantheon Render Thread", this );
		SDL_DetachThread( m_renderThread );

		// enter in event loop
		
		SDL_Event eventMessage;
		while ( m_gameState == GAME_STATE_RUNNING ) {

			SDL_WaitEventTimeout( &eventMessage, 33 );
			switch ( eventMessage.type ) {

				case SDL_KEYDOWN:
					m_input->pressKey( SDL_GetKeyName( eventMessage.key.keysym.sym ) );
				break;
				case SDL_KEYUP:
					m_input->releaseKey( SDL_GetKeyName( eventMessage.key.keysym.sym ) );
				break;
				case SDL_JOYAXISMOTION:
					m_input->updateJoystickAxis( eventMessage.jaxis.which, eventMessage.jaxis.axis );
				break;
				case SDL_JOYBUTTONDOWN:
				case SDL_JOYBUTTONUP:
					m_input->updateJoystickButton( eventMessage.jbutton.which, eventMessage.jbutton.button );
				break;
				case SDL_QUIT:
					SDL_Log( "A quit event has been recieved." );
				return;
			}
		}
	}

	// tell threads to cease

	void stop() {

		m_gameState = GAME_STATE_INITIALIZED;
	}

	SDL_Window* m_window{ nullptr };
	SDL_GLContext m_context, m_eventContext;

	SDL_mutex* m_renderMutex{ nullptr };
	SDL_Thread* m_logicThread{ nullptr };
	SDL_Thread* m_renderThread{ nullptr };
	Input* m_input{ nullptr };
	Scene* m_scene{ nullptr };
	void( *m_gameStartFunc )( ) { nullptr };
	IGameRenderer*(*m_rendererCreateFunc)() { nullptr };
	IGameRenderer* m_renderer{ nullptr };
	IGameCollision*(*m_collisionCreateFunc)() { nullptr };
	IGameCollision* m_collision{ nullptr };

	bool m_hasRendered{ false };

	int m_gameState{ GAME_STATE_UNINITIALIZED };
	int m_error{ PANTHEON_OK };

};

Game::Game() {
	
	m_game = new GameImpl();
}

Game::~Game() {

	delete m_game;
	m_game = nullptr;
}

int Game::getError() {

	return m_game->m_error;
}

void Game::start
	( void( *t_gameStartFunc )( )
	, IGameRenderer*( *t_rendererCreateFunc )( )
	, IGameCollision*( *t_collisionCreateFunc )( ) ) {

	m_game->start( t_gameStartFunc, t_rendererCreateFunc, t_collisionCreateFunc );
}

void Game::stop() {

	m_game->stop();
}

IGameCollision& Game::getCollisionManager() {

	return *m_game->m_collision;
}

const Input& Game::getInput() const {

	return *m_game->m_input;
}

IGameRenderer& Game::getRenderer() {

	return *m_game->m_renderer;
}

Scene& Game::getScene() {

	return *m_game->m_scene;
}