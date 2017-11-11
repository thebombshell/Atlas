
// game_state_prefab.cpp

#include "game_state_prefab.hpp"

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

void GameStatePrefab::update( float t_delta ) {

}

void GameStatePrefab::render() {

	const Input& input = Game::GetInput();
	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();
	std::string inputText = input.getText();
	for ( int i = 0; i < 256; i++ ) {

		inputText[i] = tolower( inputText[i] );
	}

	if ( endsWith( inputText, "awesome" ) ) {

		g_cheatCodes |= CHEAT_AWESOME;
		renderer.enableAwesome();
	}
	if ( endsWith( inputText, "reset" ) ) {

		g_cheatCodes = 0;
		renderer.disableAwesome();
	}
}
