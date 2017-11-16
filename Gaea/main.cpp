
// main.cpp

// source file for application entry

#include "main.hpp"
#include "procedural_level_generator.hpp"

#include <collision_2d.hpp>
#include <physics_component.hpp>
#include <sprite_renderer.hpp>

using namespace pantheon;
using namespace gaea;

int main() {

	if ( Game::GetError() ) {

		return 1;
	}

	Game::Start<SpriteRenderer, Collision2DManager>( []() {

		Game::GetRendererAs<SpriteRenderer>()->loadTexture( "images/sprites.png", "sprites" );

		PhysicsComponent2D::setGravity( { 0.0f, -14.0f } );

		Game::GetScene().createPrefab<ProceduralLevelGenerator>();
	} );

	return 0;
}