
// game_state_prefab.cpp

#include "game_state_prefab.hpp"
#include "boss_prefab.hpp"
#include "force_pad_prefab.hpp"
#include "player.hpp"
#include "hammer_prefab.hpp"
#include "black_hole_prefab.hpp"
#include "cherry_prefab.hpp"

#include <line_renderer.hpp>

using namespace pantheon;
using namespace atlas;

int g_cheatCodes;


bool atlas::isCheatActive( int t_cheatName ) {

	return g_cheatCodes & t_cheatName;
}

GameStatePrefab::GameStatePrefab( ConstructComponentPermit& t_permit, Actor& t_owner ) 
	: IActorComponent{ t_permit, t_owner } {

}

GameStatePrefab::~GameStatePrefab() {

}

bool endsWith( std::string t_stringToCheck, std::string t_endsWith ) {

	return t_stringToCheck.compare( t_stringToCheck.length() - t_endsWith.length(), t_endsWith.length(), t_endsWith ) == 0;
}

void GameStatePrefab::update( float ) {

}

void GameStatePrefab::render() {

	const Input& input = Game::GetInput();
	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();
	std::string inputText = input.getText();
	for ( int i = 0; i < 256; i++ ) {

		inputText[i] = static_cast<char>(tolower( inputText[i] ));
	}

	if ( !isCheatActive( CHEAT_AWESOME ) && endsWith( inputText, "awesome" ) ) {

		g_cheatCodes |= CHEAT_AWESOME;
		renderer.enableAwesome();
	}
	else if ( !isCheatActive( CHEAT_BOSS ) && endsWith( inputText, "boss" ) ) {

		g_cheatCodes |= CHEAT_BOSS;
		Game::GetScene().createPrefab<BossPrefab>();
	}
	else if ( !isCheatActive( CHEAT_ESCAPE ) && endsWith( inputText, "escape" ) ) {

		g_cheatCodes |= CHEAT_ESCAPE;
	}
	else if ( !isCheatActive( CHEAT_GTFO ) && endsWith( inputText, "gtfo" ) ) {

		g_cheatCodes |= CHEAT_GTFO;

		Game::GetScene().createPrefab<ForcePadPrefab>( ForcePadMessage{ 0 } );
		Game::GetScene().createPrefab<ForcePadPrefab>( ForcePadMessage{ 1 } );
		Game::GetScene().createPrefab<ForcePadPrefab>( ForcePadMessage{ 2 } );
		Game::GetScene().createPrefab<ForcePadPrefab>( ForcePadMessage{ 3 } );
	}
	else if ( !isCheatActive( CHEAT_WHEELCHAIR ) && endsWith( inputText, "wheelchair" ) ) {

		g_cheatCodes |= CHEAT_WHEELCHAIR;
	}
	else if ( !isCheatActive( CHEAT_THOR ) && endsWith( inputText, "thor" ) ) {

		g_cheatCodes |= CHEAT_THOR;

		Game::GetScene().createPrefab<HammerPrefab>();
	}
	else if ( !isCheatActive( CHEAT_HAWKING ) && endsWith( inputText, "hawking" ) ) {

		g_cheatCodes |= CHEAT_HAWKING;

		Game::GetScene().createPrefab<BlackHolePrefab>();
	}
	else if ( !isCheatActive( CHEAT_JUGGERNAUGHT ) && endsWith( inputText, "juggernaught" ) ) {

		g_cheatCodes |= CHEAT_JUGGERNAUGHT;

		Player::beginJuggernaught();
	}
	else if ( !isCheatActive( CHEAT_WAKKA ) && endsWith( inputText, "wakka" ) ) {

		g_cheatCodes |= CHEAT_WAKKA;

		Game::GetScene().createPrefab<CherryPrefab>();
	}
	else if ( endsWith( inputText, "zero" ) ) {

		if ( isCheatActive( CHEAT_AWESOME ) ) {

			renderer.disableAwesome();
		}
		g_cheatCodes = 0;
	}
	else if ( !isCheatActive( CHEAT_ZERO ) && endsWith( inputText, "reset" ) ) {

		g_cheatCodes |= CHEAT_ZERO;
		Player::reset();
	}
	if ( !endsWith( inputText, "reset" ) ) {

		g_cheatCodes &= ~CHEAT_ZERO;
	}
}
