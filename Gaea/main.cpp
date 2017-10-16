
// main.cpp

// source file for application entry

#include "main.hpp"

#include <sprite_renderer.hpp>
#include <collision_2d.hpp>

using namespace pantheon;
using namespace gaea;

int main() {

	if ( Game::GetError() ) {

		return 1;
	}

	Game::Start<SpriteRenderer, Collision2DManager>( []() {

	} );

	return 0;
}