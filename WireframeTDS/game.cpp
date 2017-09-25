
// game.cpp

// Source file for the game class.

#include "game.hpp"
#include "collisions.hpp"
#include "error.hpp"
#include "game_actor.hpp"
#include "render_helper.hpp"
#include "utils.hpp"
#include "window.hpp"

#include <assert.h>
#include <iostream>
#define GLM_FORCE_CXX03
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

// gl includes.

#define GLEW_DYNAMIC
#include <glew.h>
#include <wglew.h>
#include <freeglut.h>

using namespace wireframe_tds;

Game::Game(
	) {
	
	m_vertexBufferData = new float[WTDS_GAME_MAX_ELEMENT_COUNT * 6];
}

Game::~Game(
	) {

	delete[] m_vertexBufferData;
	m_vertexBufferData = nullptr;
}

// display callback function

void Game::onDisplay(
	) {

	// assert game is running

	assert( Game::getGameState() == WTDS_GAME_IS_RUNNING );

	// clear the context

	glClear( GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	m_drawElementCount = 0;

	// render all renderable actors

	for ( auto iter = m_actors.begin(); iter != m_actors.end(); ++iter ) {

		IGameRenderable* actor = dynamic_cast<IGameRenderable*>((*iter));
		if ( actor != nullptr && actor->getState() == WTDS_ACTOR_ENABLED)
			actor->onRender( m_timeDelta );
	}

	// if nothing is to be drawn, skip drawing

	if ( m_drawElementCount <= 0 ) {

		glutSwapBuffers();
		return;
	}

	// fill vertex buffer

	RenderHelper::fillBuffer(
		m_bufferIds[0],
		m_vertexBufferData,
		sizeof( float ) * 6 * m_drawElementCount,
		GL_DYNAMIC_DRAW );

	// set up binds

	glUseProgram( m_programId );
	glBindVertexArray( m_vertexArrayObjects[0] );

	// create and use view matrix

	glm::vec3 eye{ 0.0f, 0.0f, 0.0f };
	glm::vec3 look{ 0.0f, 0.0f, 1.0f };
	glm::vec3 up{ 0.0f, 1.0f, 0.0f };
	glm::mat4 viewMatrix{ glm::lookAt( eye, look, up ) };
	float* view{ glm::value_ptr( viewMatrix ) };
	glUniformMatrix4fv(
		glGetUniformLocation( m_programId, "uniformView" ), 1, GL_FALSE, view );

	// create and use projection matrix

	float windowWidth{ static_cast<float>(glutGet( GLUT_WINDOW_WIDTH )) };
	float windowHeight{ static_cast<float>(glutGet( GLUT_WINDOW_HEIGHT )) };
	float hRatio{ 1.0f };
	float vRatio{ 1.0f };
	if ( windowWidth > windowHeight ) {

		hRatio = windowWidth / windowHeight;
	}
	else {

		vRatio = windowHeight / windowWidth;
	}

	float halfViewWidth{ 128.0f * hRatio * 0.5f };
	float halfViewHeight{ 128.0f * vRatio * 0.5f };

	glm::mat4 projectionMatrix{ glm::ortho(
		halfViewWidth, -halfViewWidth, -halfViewHeight, halfViewHeight, 0.1f, 1.0f ) };
	float* projection{ glm::value_ptr( projectionMatrix ) };
	glUniformMatrix4fv( 
		glGetUniformLocation( m_programId, "uniformProjection" ), 1, GL_FALSE, projection );
	glDrawArrays( GL_LINES, 0, m_drawElementCount );

	// finish and swap buffers

	glutSwapBuffers();
}

// process collidable actor helper function

void Game::processCollidable(
	IGameCollidable* t_actor
	) {

	const collisions::ICollider* colliderA = t_actor->getCollider();

	for ( auto iter = m_actors.begin(); iter != m_actors.end(); ++iter ) {

		IGameCollidable* actor = dynamic_cast<IGameCollidable*>( *iter );
		if ( actor != nullptr && actor != t_actor  && actor->getState() == WTDS_ACTOR_ENABLED ) {

			const collisions::ICollider* colliderB = actor->getCollider();
			glm::vec3 result = collisions::findMinimumSeparatingAxis( *colliderA, *colliderB );
			if ( collisions::getAxisState( result ) == collisions::WTDS_COLLISION_FOUND ) {

				t_actor->onCollision( result, *actor );
				actor->onCollision( result, *t_actor );
			}
		}
	}
}

// per loop function

void Game::onLoop(
	) {

	// assert game is running

	assert( Game::getGameState() == WTDS_GAME_IS_RUNNING );

	// update time delta

	int currentFrameTime = glutGet( GLUT_ELAPSED_TIME );
	m_timeDelta = static_cast<float>(currentFrameTime - m_previousFrameTime) / 1000.0f;
	m_previousFrameTime = currentFrameTime;

	// add all members of add queue and cleanup

	for ( auto iter = m_addQueue.begin(); iter != m_addQueue.end(); ++iter ) {

		IGameActor* actor = *iter;
		actor->setState( WTDS_ACTOR_ENABLED );
		m_actors.insert( actor );
	}
	m_addQueue.clear();

	// update all updatable actors

	for ( auto iter = m_actors.begin(); iter != m_actors.end(); ++iter ) {

		IGameUpdatable* actor = dynamic_cast<IGameUpdatable*>( (*iter) );
		if ( actor != nullptr  && actor->getState() == WTDS_ACTOR_ENABLED )
			actor->onUpdate( m_timeDelta );
	}

	// remove all members of remove queue and cleanup

	for ( auto iter = m_removeQueue.begin(); iter != m_removeQueue.end(); ++iter ) {

		m_actors.erase( *iter );
		delete *iter;
	}
	m_removeQueue.clear();

	// test collide all collidable actors

	for ( auto iter = m_actors.begin(); iter != m_actors.end(); ++iter ) {

		IGameCollidable* actor = dynamic_cast<IGameCollidable*>( *iter );
		if ( actor != nullptr && actor->getState() == WTDS_ACTOR_ENABLED) {

			processCollidable( actor );
		}
	}

	// post display request

	glutPostRedisplay();
}

// queue elements to draw lines function

int Game::queueDrawLines(
	const unsigned int t_elementCount,
	const float* t_elements
	) {

	// assert game is initialized

	assert( Game::getGameState() != WTDS_GAME_NOT_INITIALIZED );

	// if element count is not even unwanted lines may occur, return error

	if ( t_elementCount % 2 != 0 ) {

		return WTDS_ERROR_UNEVEN_ELEMENT_COUNT;
	}

	// if elements would overflow buffer

	unsigned int newElementCount = m_drawElementCount + t_elementCount;
	if ( newElementCount > WTDS_GAME_MAX_ELEMENT_COUNT ) {

		return WTDS_ERROR_TOO_MANY_ELEMENTS;
	}

	// copy memory and adjust draw count
	
	std::memcpy( &(m_vertexBufferData[m_drawElementCount * 6]), t_elements, sizeof( float ) * 6 * t_elementCount );
	m_drawElementCount = newElementCount;

	return WTDS_ERROR_OK;
}

// static queue elements to draw lines function

int Game::drawLines(
	const unsigned int t_elementCount,
	const float* t_elements
	) {

	// assert game is initialized

	assert( Game::getGameState() != WTDS_GAME_NOT_INITIALIZED );

	Game& singleton = Game::getInstance();
	return singleton.queueDrawLines( t_elementCount, t_elements );
}

void Game::display(
	) {

	// assert game is running

	assert( Game::getGameState() == WTDS_GAME_IS_RUNNING );

	Game& singleton = Game::getInstance();
	singleton.onDisplay();
}

void Game::idle(
	) {

	// assert game is running

	assert( Game::getGameState() == WTDS_GAME_IS_RUNNING );

	Game& singleton = Game::getInstance();
	singleton.onLoop();
}

void Game::keydown(
	unsigned char t_key,
	int t_x,
	int t_y
	) {

	// assert game is initialized

	assert( Game::getGameState() != WTDS_GAME_NOT_INITIALIZED );

	Game& singleton = Game::getInstance();
	singleton.m_input.onKeyPressed( std::string( 1, t_key ), t_x, t_y );
}

void Game::keyup(
	unsigned char t_key,
	int t_x,
	int t_y
	) {

	// assert game is initialized

	assert( Game::getGameState() != WTDS_GAME_NOT_INITIALIZED );

	Game& singleton = Game::getInstance();
	singleton.m_input.onKeyReleased( std::string( 1, t_key ), t_x, t_y );
}

int Game::getGameState(
	) {

	Game& singleton = Game::getInstance();
	return singleton.m_state;
}

int Game::initialize(
	int* t_argc,
	char** t_argv,
	std::string& t_errorString
	) {

	// assert game is not initialized

	assert( Game::getGameState() == WTDS_GAME_NOT_INITIALIZED );

	int isError{ 0 };
	t_errorString = "";
	Game& singleton = Game::getInstance();

	// initialize window

	isError = window::initializeWindow( t_argc, t_argv );

	// if is error return error

	if ( isError ) {

		return isError;
	}

	// initialize rendering api

	isError = RenderHelper::initializeApi();

	// if is error return error

	if ( isError ) {

		return isError;
	}

	// with gl now available create buffers and vertex arrays 

	isError = RenderHelper::createBuffers( 1, singleton.m_bufferIds );
	
	if ( isError ) {

		return isError;
	}

	isError = RenderHelper::createVertexArrayObjects( 1, singleton.m_vertexArrayObjects );

	if ( isError ) {

		return isError;
	}


	// describe vertex arrays

	glBindVertexArray( singleton.m_vertexArrayObjects[0] );
	glBindBuffer( GL_ARRAY_BUFFER, singleton.m_bufferIds[0] );
	glVertexAttribPointer(
		0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, (void*)0 );
	glVertexAttribPointer(
		1, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, (void*)(sizeof( float ) * 3) );
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glBindVertexArray( NULL );

	// load vertex shader

	std::string vertexShader;
	isError = wireframe_tds::getFileAsString(
		"shaders/color_line_vs_120.glsl", vertexShader );

	if ( isError ) {
		t_errorString = "";
		return WTDS_ERROR_COULD_NOT_OPEN_VERTEX_SHADER;
	}

	// load fragment shader

	std::string fragmentShader;
	isError = wireframe_tds::getFileAsString(
		"shaders/color_line_fs_120.glsl", fragmentShader );

	if ( isError ) {
		t_errorString = "";
		return WTDS_ERROR_COULD_NOT_OPEN_FRAGMENT_SHADER;
	}

	// create program from shaders

	std::string errorString{ "" };
	isError = RenderHelper::createProgram(
		vertexShader,
		fragmentShader,
		{ 
			{ "attributePosition", 0 },
			{ "attributeColor", 1 }
		},
		singleton.m_programId,
		errorString );

	if ( isError ) {

		t_errorString = errorString;
		return isError;
	}

	// set glut events

	glutDisplayFunc( &Game::display );
	glutIdleFunc( &Game::idle );
	glutKeyboardFunc( &Game::keydown );
	glutKeyboardUpFunc( &Game::keyup );

	// set up misc singleton variables

	singleton.m_state = WTDS_GAME_INITIALIZED;
	singleton.m_previousFrameTime = glutGet( GLUT_ELAPSED_TIME );

	return WTDS_ERROR_OK;
}

int Game::finalize(
	) {

	// assert game is initialized but not running

	assert( Game::getGameState() == WTDS_GAME_INITIALIZED );

	Game& singleton = Game::getInstance();
	singleton.m_state = WTDS_GAME_NOT_INITIALIZED;

	return WTDS_ERROR_OK;
}

void Game::run(
	) {

	// assert game is initialized but not running

	assert( Game::getGameState() == WTDS_GAME_INITIALIZED );

	Game& singleton = Game::getInstance();
	singleton.m_state = WTDS_GAME_IS_RUNNING;
	window::mainLoop();
}

void Game::removeActor(
	IGameActor* t_gameActor
	) {

	// assert game is initialized

	assert( Game::getGameState() != WTDS_GAME_NOT_INITIALIZED );

	Game& singleton = Game::getInstance();

	// assert is in actors and is not dupplicate

	assert( singleton.m_actors.find( t_gameActor ) != singleton.m_actors.end() );
	assert( singleton.m_removeQueue.find( t_gameActor ) == singleton.m_removeQueue.end() );

	singleton.m_removeQueue.insert( t_gameActor );
	t_gameActor->setState( WTDS_ACTOR_DISABLED );
}

bool Game::isKeyDown(
	std::string t_key
	) {

	Game& singleton = Game::getInstance();
	return singleton.m_input.getKeyState( t_key ) == WTDS_INPUT_KEY_DOWN;
}
