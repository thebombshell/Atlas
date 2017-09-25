
// window_helper.cpp

// Source file for window helper namespace.

#include "window.hpp"
#include "error.hpp"
#include <freeglut.h>
#include <assert.h>

using namespace wireframe_tds;

// static window state check function

int window::getInitializedState(
	) {

	if ( glutGet( GLUT_INIT_STATE ) != 1 ) {
		return WTDS_WINDOW_NOT_INITIALIZED;
	}

	return WTDS_WINDOW_INITIALIZED;
}

// static window initialize function

int window::initializeWindow(
	int* t_argc,
	char** t_argv
	) {

	// assert window has not already been initialized

	assert( window::getInitializedState() == WTDS_WINDOW_NOT_INITIALIZED );

	// set up glut

	glutInit( t_argc, t_argv );

	// set up window

	glutInitWindowPosition( 200, 200 );
	glutInitWindowSize( 1280, 720 );
	glutInitDisplayMode( GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE );

	// create window

	glutCreateWindow( "Wireframe Top Down Shooter" );

	return WTDS_ERROR_OK;
}

// static window finalize function

int window::finalizeWindow(
	) {

	// assert window has been initialized

	assert( window::getInitializedState() != WTDS_WINDOW_NOT_INITIALIZED );

	return WTDS_ERROR_OK;
}

//

void window::mainLoop(
	) {

	glutMainLoop();
}