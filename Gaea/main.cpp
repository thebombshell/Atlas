
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

		Game::GetScene().createPrefab<TestPrefab>();
		auto& floor = Game::GetScene().createPrefab<FloorPrefab>();
		floor.getTransform().position = { 0.0f, -2.0f, 0.0f };
	} );

	return 0;
}