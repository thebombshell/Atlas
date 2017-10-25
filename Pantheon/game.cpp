
// game.cpp

// source file for the game class

#include "game.hpp"

#include "iactor_component.hpp"

#define GLEW_DYNAMIC
#include <algorithm>
#include <GL\glew.h>
#include <mutex>
#include <queue>
#include <SDL.h>
#include <SDL_image.h>
#include <thread>

using namespace pantheon;

const int GAME_STATE_UNINITIALIZED{ 0 };
const int GAME_STATE_INITIALIZED{ 1 };
const int GAME_STATE_RUNNING{ 2 };

const int DRAW_STATE_CLEAN{ 0 };
const int DRAW_STATE_PENDING{ 1 };

class Game::GameImpl {

	friend class Game;

	GameImpl
		( ConstructRendererPermit& t_rendererPermit
		, ConstructCollisionPermit& t_collisionPermit ) 
		: m_rendererPermit{ t_rendererPermit }
		, m_collisionPermit{ t_collisionPermit } {

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

		int isError = SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC );
		if ( isError ) {

			SDL_Log( "SDL did not initialize correctly: %s", SDL_GetError() );
			return PANTHEON_ERROR;
		}

		// initialize sdl image loading extension

		int flags = IMG_INIT_JPG | IMG_INIT_PNG;
		isError = IMG_Init( flags );
		if ( (isError & flags) != flags ) {

			SDL_Log( "SDL_Image did not initialize correctly: %s", IMG_GetError() );
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

		// create supporting objects

		m_input = new Input();
		m_scene = new Scene();
		m_audio = new Audio();

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
			delete m_audio;

			// shutdown sdl extentions

			IMG_Quit();

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

		m_collision = m_collisionCreateFunc( m_collisionPermit );

		while ( m_renderer == nullptr );
		m_gameStartFunc();
		unsigned long long previousFrameTicks{ SDL_GetPerformanceCounter() };
		while ( m_gameState == GAME_STATE_RUNNING ) {

			unsigned long long  currentFrameTicks{ SDL_GetPerformanceCounter() };
			float timeDelta = (currentFrameTicks - previousFrameTicks) / static_cast<float>(SDL_GetPerformanceFrequency());
			previousFrameTicks = currentFrameTicks;

			m_scene->update();

			CallUpdatablesPermit updatePermit;
			IUpdatable::call( updatePermit, timeDelta );

			m_collision->simulate( timeDelta );

			// try to get exclusive renderer access

			if ( m_renderer != nullptr && m_renderMutex.try_lock() ) {

				m_renderer->beforeActorsDraw();

				CallRenderablesPermit renderPermit;
				IRenderable::call( renderPermit );

				m_renderer->afterActorsDraw();

				// tell renderer

				m_hasRendered = false;

				m_renderMutex.unlock();

			}
			else {

				SDL_Delay( 0 );
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

		m_renderer = m_rendererCreateFunc( m_rendererPermit );

		while ( m_gameState == GAME_STATE_RUNNING ) {

			// if has not rendered, render, if has rendered no render is necassary

			if ( !m_hasRendered ) {

				std::lock_guard<std::mutex> guard{ m_renderMutex };

				// start frame

				//glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

				// run renderer

				m_renderer->render();

				// swap buffers

				SDL_GL_SwapWindow( m_window );
				m_hasRendered = true;

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
		, IGameRenderer::CreateInstance t_rendererCreateFunc
		, IGameCollision::CreateInstance t_collisionCreateFunc) {

		m_gameStartFunc = t_gameStartFunc;
		m_rendererCreateFunc = t_rendererCreateFunc;
		m_collisionCreateFunc = t_collisionCreateFunc;

		// set is running

		m_gameState = GAME_STATE_RUNNING;

		// start logic thread
		
		m_logicThread = new std::thread( logicThreadFunc, this );

		// start render thread

		m_renderThread = new std::thread( renderThreadFunc, this );

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

	std::mutex m_renderMutex;
	std::thread* m_logicThread{ nullptr };
	std::thread* m_renderThread{ nullptr };
	Input* m_input{ nullptr };
	Scene* m_scene{ nullptr };
	Audio* m_audio{ nullptr };
	void( *m_gameStartFunc )( ) { nullptr };
	ConstructRendererPermit& m_rendererPermit;
	ConstructCollisionPermit& m_collisionPermit;
	IGameRenderer::CreateInstance m_rendererCreateFunc { nullptr };
	IGameCollision::CreateInstance m_collisionCreateFunc { nullptr };
	IGameRenderer* m_renderer{ nullptr };
	IGameCollision* m_collision{ nullptr };

	bool m_hasRendered{ false };

	int m_gameState{ GAME_STATE_UNINITIALIZED };
	int m_error{ PANTHEON_OK };

};

Game::Game() {
	
	m_game = new GameImpl(m_rendererPermit, m_collisionPermit);
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
	, IGameRenderer*( *t_rendererCreateFunc )( ConstructRendererPermit& )
	, IGameCollision*( *t_collisionCreateFunc )( ConstructCollisionPermit& ) ) {

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

Audio& Game::getAudio() const {

	return *m_game->m_audio;
}