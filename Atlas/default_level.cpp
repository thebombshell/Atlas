
// default_level.cpp

// source file for the default level actor

#include "default_level.hpp"
#include "player.hpp"
#include "line_text_helpers.hpp"
#include <line_renderer.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

const float PI2 = float( M_PI ) * 2;

using namespace pantheon;
using namespace atlas;

float m_timer{ 0.0f };

LineRendererMessage tutorialMessage = stringToLines( "up & down to thrust  left & right to rotate  bumper to shoot  trigger to boost" );
LineRendererMessage playAreaMessage = stringToLines( "enter the play area to begin" );

DefaultLevel::DefaultLevel( pantheon::ConstructComponentPermit& t_permit, pantheon::Actor& t_owner )
	: IActorComponent( t_permit, t_owner ) {

}

DefaultLevel::~DefaultLevel() {

}

void DefaultLevel::update( float t_delta ) {

	Game::GetScene().forEach( [](Actor* const t_actor) {

		glm::vec3& position = t_actor->getTransform().position;
		if ( t_actor->hasComponent<Player>() ) {

			Player& player = t_actor->getComponent<Player>();
			if ( !player.isKillable() ) {

				if ( position[0] > 240.0f ) {

					position[0] -= 480.0f;
				}
				if ( position[0] <= -240.0f ) {

					position[0] += 480.0f;
				}
				if ( position[1] > 140.0f ) {

					position[1] -= 280.0f;
				}
				if ( position[1] <= -140.0f ) {

					position[1] += 280.0f;
				}
				return;
			}
		}
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

	m_timer += t_delta;
	if ( m_timer >= PI2 ) {

		m_timer -= PI2;
	}
}

void waveEffect( LineRendererVertex& t_vertex ) {

	float offset{ cos( m_timer * 2.0f + t_vertex.position[0] * 0.1f ) * 0.5f };

	t_vertex.position[1] += offset;
}

void DefaultLevel::render( void ) {

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}
	LineRendererVertex vertices[] = {
		{ -128.0f, -128.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ 128.0f, -128.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ 128.0f, 128.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ -128.0f, 128.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		LineRendererVertex::separator(),
		LineRendererVertex::separator(),
		{ -240.0f, -140.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ 240.0f, -140.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ 240.0f, 140.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		{ -240.0f, 140.0f, 0.5f, 1.0f, 1.0f, 1.0f },
		LineRendererVertex::separator(),
		LineRendererVertex::separator(),
	};
	LineRendererMessage message{vertices, vertices + 12};

	renderer->queueDraw( message );
	LineRendererMessage tutMessage = tutorialMessage;
	tutMessage.transform = Transform2D( { -232.0f, -136.5f }, { 1.0f, 1.0f }, 0.0f ).findMatrix();
	tutMessage.applyEffect( waveEffect );
	renderer->queueDraw( tutMessage );
	LineRendererMessage playMessage = playAreaMessage;
	playMessage.transform = Transform2D( { -80.0f, 131.5f }, { 1.0f, 1.0f }, 0.0f ).findMatrix();
	playMessage.applyEffect( waveEffect );
	renderer->queueDraw( playMessage );
}
