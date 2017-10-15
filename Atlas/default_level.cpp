
// default_level.cpp

// source file for the default level actor

#include "default_level.hpp"
#include "player.hpp"
#include <line_renderer.hpp>

using namespace pantheon;
using namespace atlas;

DefaultLevel::DefaultLevel( pantheon::ConstructComponentPermit& t_permit, pantheon::Actor& t_owner )
	: IActorComponent( t_permit, t_owner ) {

}

DefaultLevel::~DefaultLevel() {

}

void atlas::DefaultLevel::update( float ) {

	Game::GetScene().forEach( [](Actor* const t_actor) {

		glm::vec3& position = t_actor->getTransform().position;
		if ( position[0] > 128.0f ) {

			position[0] -= 256.0f;
		}
		if ( position[0] <= -128.0f ) {

			position[0] += 256.0f;
		}
		if ( position[1] > 128.0f ) {

			position[1] -= 256.0f;
		}
		if ( position[1] <= -128.0f ) {

			position[1] += 256.0f;
		}
	} );
}

void atlas::DefaultLevel::render( void ) {

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}
	LineRendererVertex vertices[] = {
		{ -128.0f, -128.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ 128.0f, -128.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ 128.0f, 128.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ -128.0f, 128.0f, 0.5f, 1.0f, 1.0f, 1.0f }
	};
	LineRendererMessage message{vertices, vertices + 4};
	message.loop();
	renderer->queueDraw( message );
}
