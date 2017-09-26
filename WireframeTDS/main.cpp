
// main.cpp

// Source file for the application entry point.

#include "main.hpp"
#include "game.hpp"
#include "utils.hpp"
#include "error.hpp"
#include "player_ship.hpp"
#include "small_stage.hpp"

using namespace atlas;

int main(
	int argc,
	char **argv
	) {

	int isError = 0;
	std::string errorString{ "" };

	// initialize context

	isError = Game::initialize( &argc, argv, errorString );

	// if initialization returned an error report it through an error box.

	if ( isError ) {

		showMessage( getErrorMessage( isError ) + "\n" + errorString );
	}

	// set up actors

	Game::addActor<game::PlayerShip>( 0 );
	Game::addActor<game::PlayerShip>( 1 );
	Game::addActor<game::SmallStage>( NULL );

	Game::run();

	return 0;
}