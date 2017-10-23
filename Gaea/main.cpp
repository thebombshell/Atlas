
// main.cpp

// source file for application entry

#include "main.hpp"
#include "test_prefab.hpp"

#include <sprite_renderer.hpp>
#include <collision_2d.hpp>

using namespace pantheon;
using namespace gaea;

int main() {

	if ( Game::GetError() ) {

		return 1;
	}

	Game::Start<SpriteRenderer, Collision2DManager>( []() {


		PhysicsComponent2D::setGravity( { 0.0f, -8.0f } );
		Game::GetScene().createPrefab<TestPrefab>();
		BlockMessage messageA{ { 0.0f, -2.0f }, { 50.0f, 1.0f }, false };
		BlockMessage messageB{ { 2.0f, 1.0f }, { 2.0f, 2.0f }, true };
		BlockMessage messageC{ { 4.0f, 1.0f }, { 1.0f, 1.0f }, true };
		BlockMessage messageD{ { -4.0f, 1.0f }, { 1.0f, 1.0f }, true };
		Game::GetScene().createPrefab<BlockPrefab>( messageA );
		Game::GetScene().createPrefab<BlockPrefab>( messageB );
		Game::GetScene().createPrefab<BlockPrefab>( messageC );
		Game::GetScene().createPrefab<BlockPrefab>( messageD );
	} );

	return 0;
}