
// main.cpp

// source file for application entry

#include "main.hpp"

#include "game_state_prefab.hpp"
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

		Audio& audio = Game::GetAudio();
		audio.loadSound( "audio/bonk.wav", "bonk" );
		audio.loadSound( "audio/shoot.wav", "shoot" );
		audio.loadSound( "audio/explode.wav", "explode" );
		audio.loadSound( "audio/woosh.wav", "woosh" );
		audio.loadSound( "audio/hum.wav", "hum" );

		pantheon::Game::GetScene().createPrefab<GameStatePrefab>();

		int joystickCount = Game::GetInput().getJoystickCount();
		if ( joystickCount > 0 ) {
			pantheon::Game::GetScene().createPrefab<Player>( PlayerInfo( 0 ) );
			pantheon::Game::GetScene().createPrefab<Player>( PlayerInfo( 2 ) );
		}
		if ( joystickCount > 1 ) {
			pantheon::Game::GetScene().createPrefab<Player>( PlayerInfo( 1 ) );
			pantheon::Game::GetScene().createPrefab<Player>( PlayerInfo( 3 ) );
		}
		pantheon::Game::GetScene().createPrefab<DefaultLevel>();
	} );
	
	return 0;
}