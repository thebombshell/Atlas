
// main.cpp

// source file for application entry

#include "main.hpp"
#include "player_prefab.hpp"
#include "blocks.hpp"

#include <sprite_renderer.hpp>
#include <collision_2d.hpp>

using namespace pantheon;
using namespace gaea;

int main() {

	if ( Game::GetError() ) {

		return 1;
	}

	Game::Start<SpriteRenderer, Collision2DManager>( []() {

		Game::GetRendererAs<SpriteRenderer>()->loadTexture( "images/sprites.png", "sprites" );

		PhysicsComponent2D::setGravity( { 0.0f, -14.0f } );
		Game::GetScene().createPrefab<PlayerPrefab>();
		BlockMessage messageA{ { 0.0f, -2.0f }, { 50.0f, 1.0f } };
		BlockMessage messageB{ { 4.0f, 1.0f }, { 2.0f, 2.0f } };
		BlockMessage messageC{ { 2.0f, -1.0f }, { 1.0f, 1.0f } };
		MovingBlockMessage messageD{ { -8.0f, 2.0f }, { -6.0f, -1.0f }, { 2.0f, 1.0f } };
		Game::GetScene().createPrefab<BlockPrefab>( messageA );
		Game::GetScene().createPrefab<BlockPrefab>( messageB );
		Game::GetScene().createPrefab<HazardPrefab>( messageC );
		Game::GetScene().createPrefab<MovingBlockPrefab>( messageD );
	} );

	return 0;
}