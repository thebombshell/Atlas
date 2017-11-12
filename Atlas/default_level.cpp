
// default_level.cpp

// source file for the default level actor

#include "default_level.hpp"
#include "player.hpp"
#include "line_text_helpers.hpp"
#include "game_state_prefab.hpp"
#include "health_component.hpp"

#include <line_renderer.hpp>
#include <physics_component.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

const float PI2 = float( M_PI ) * 2;
float g_timer{ 0.0f };

using namespace pantheon;
using namespace atlas;

LineRendererMessage tutorialMessage = stringToLines( "up & down to thrust  left & right to rotate  bumper to shoot  trigger to boost" );
LineRendererMessage playAreaMessage = stringToLines( "enter the play area to begin" );

DefaultLevel::DefaultLevel( pantheon::ConstructComponentPermit& t_permit, pantheon::Actor& t_owner )
	: IActorComponent( t_permit, t_owner ) {

	Audio& audio = Game::GetAudio();
	m_zapSource = audio.createSource( "hum" );
	m_zapSource->setLooping();
	m_zapSource->setPitch( 2.0f );
	m_zapSource->setGain( 0.0f );
	m_zapSource->play();
}

DefaultLevel::~DefaultLevel() {

}

void DefaultLevel::update( float t_delta ) {

	bool canKill = isCheatActive( CHEAT_ESCAPE ) && g_timer < PI2 * 0.5f;
	Game::GetScene().forEach( [&](Actor* const t_actor) {

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
			else if ( canKill && (abs(position[0]) > 128.0f || abs(position[1]) > 128.0f) ) {

				auto& health = t_actor->getComponent<HealthComponent>();
				if ( health.isActive() ) {
					
					health.damage( 1 );
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

	g_timer += t_delta;
	if ( g_timer >= PI2 ) {

		g_timer -= PI2;
	}
}

void waveEffect( LineRendererVertex& t_vertex ) {

	float offset{ cos( g_timer * 2.0f + t_vertex.position[0] * 0.1f ) * 0.5f };

	t_vertex.position[1] += offset;
}

void DefaultLevel::render( void ) {

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}
	glm::vec3 color{ 1.0f, 1.0f, 1.0f };

	if ( isCheatActive( CHEAT_ESCAPE ) ) {

		if ( g_timer < PI2 * 0.5f ) {

			color = { 1.0f, 0.0f, 0.0f };
			m_zapSource->setGain( 1.0f );
		}
		else {

			m_zapSource->setGain( 0.0f );
		}
	}
	LineRendererVertex vertices[] = {
		{ -128.0f, -128.0f, 0.5f, color[0], color[1], color[2] },
		{ 128.0f, -128.0f, 0.5f, color[0], color[1], color[2] },
		{ 128.0f, 128.0f, 0.5f, color[0], color[1], color[2] },
		{ -128.0f, 128.0f, 0.5f, color[0], color[1], color[2] },
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
