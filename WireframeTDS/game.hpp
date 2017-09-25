
// game.hpp

// Header file for the game management class.

#ifndef WIREFRAMETDS_HPP
#define WIREFRAMETDS_HPP

#include "wireframe_tds.hpp"

#include "forward_declaration.hpp"
#include "input_manager.hpp"
#include "game_actor.hpp"

#include <assert.h>
#include <set>
#include <string>

namespace wireframe_tds {

	// game constants

	const int WTDS_GAME_NOT_INITIALIZED{ 0 };
	const int WTDS_GAME_INITIALIZED{ 1 };
	const int WTDS_GAME_IS_RUNNING{ 2 };
	const int WTDS_GAME_MAX_ELEMENT_COUNT{ 20000 };

	// A class which wraps functionality up and provides a single interface for the game

	class Game {
	private:

		Game(
			);

		~Game(
			);

		// per display function

		void onDisplay(
			);

		// per loop function

		void onLoop(
			);

		// process collidable actor helper function

		void processCollidable(
			IGameCollidable* t_actor
			);

		// queues elements to draw lines

		int queueDrawLines(
			const unsigned int t_elementCount,
			const float* t_elements
			);

		// lazy singleton

		Game( Game const& ) = delete;
		void operator=( Game const& ) = delete;
		static Game& getInstance(
			) {

			static Game instance;
			return instance;
		}

		// member variables

		int m_state{ 0 };
		unsigned int m_programId{ 0 };
		unsigned int m_drawElementCount{ 0 };
		unsigned int* m_bufferIds{ nullptr };
		unsigned int* m_vertexArrayObjects{ nullptr };
		float* m_vertexBufferData{ nullptr };
		std::set<IGameActor*> m_actors;
		InputManager m_input;
		int m_previousFrameTime{ 0 };
		float m_timeDelta{ 0.0f };
		std::set<IGameActor*> m_addQueue;
		std::set<IGameActor*> m_removeQueue;

	public:

		// public functions

		static int drawLines(
			const unsigned int t_elementCount,
			const float* t_elements
			);

		// static display callback function

		static void display(
			);

		// static idle callback function

		static void idle(
			);

		// static is initialized function

		static int getGameState(
			);

		// static initialize game function

		static int initialize(
			int* t_argc,
			char** t_argv,
			std::string& t_errorString
			);

		// static finalize game function

		static int finalize(
			);

		// static run game loop function

		static void run(
			);

		// static add game actor function

		template<typename T, typename P>
		static void const addActor(
			const P& t_args
			) {

			// assert game is initialized

			assert( Game::getGameState() != WTDS_GAME_NOT_INITIALIZED );

			// create actor

			T* actor = new T( t_args );

			// insert in add queue

			Game& singleton = Game::getInstance();
			singleton.m_addQueue.insert( static_cast<IGameActor*>(actor) );
		}

		// static remove game actor function

		static void removeActor(
			IGameActor* t_gameActor
			);

		// static key down callback function

		static void Game::keydown(
			unsigned char t_key,
			int t_x,
			int t_y
			);

		// static key up callback function

		static void Game::keyup(
			unsigned char t_key,
			int t_x,
			int t_y
			);

		// static is key down function

		static bool Game::isKeyDown(
			std::string t_key
			);
	};
}

#endif