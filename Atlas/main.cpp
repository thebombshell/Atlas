
// main.cpp

// source file for application entry

#include "main.hpp"

#include "default_level.hpp"
#include "player.hpp"

#include <line_renderer.hpp>
#include <collision_2d.hpp>

using namespace pantheon;
using namespace atlas;

int main() {

	if ( Game::GetError() ) {

		return 1;
	}

	Game::Start<LineRenderer, Collision2DManager>( []() {

		pantheon::Game::GetScene().createPrefab<Player>(
			PlayerInfo( 0 ) );
		pantheon::Game::GetScene().createPrefab<Player>(
			PlayerInfo( 1 ) );
		pantheon::Game::GetScene().createPrefab<Player>(
			PlayerInfo( 2 ) );
		pantheon::Game::GetScene().createPrefab<Player>(
			PlayerInfo( 3 ) );
		pantheon::Game::GetScene().createPrefab<DefaultLevel>();
	} );
	
	return 0;
}