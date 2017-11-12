
// cherry_prefab.cpp

#include "cherry_prefab.hpp"
#include "game_state_prefab.hpp"
#include "player.hpp"
#include "health_component.hpp"

#include <line_renderer.hpp>
#include <collision_2d.hpp>

#define _USE_MATH_DEFINES

#include <math.h>
#include <random>

const float PI2 = float( M_PI ) * 2;

using namespace pantheon;
using namespace atlas;

CherryPrefab::CherryPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner )
	: IActorComponent{ t_permit, t_owner }, m_circle{ t_owner.getTransform(), 2.5f } {

	t_owner.createComponent<Collision2DComponent>();
	auto& collision = t_owner.getComponent<Collision2DComponent>();
	collision.addCollider( &m_circle );
	collision.setCollisionFlags( 512 );
	collision.setCollideWithFlags( 0xffff & ~(512) );

	float randX = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 256.0f - 128.0f;
	float randY = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 256.0f - 128.0f;

	t_owner.getTransform().position = { randX, randY, 0.0f };
}

CherryPrefab::~CherryPrefab() {

}

void CherryPrefab::render() {

	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();

	LineRendererMessage message{ getOwner().getTransform2D().findMatrix() };

	for ( int i = 0; i < 8; ++i ) {

		float alpha = PI2 * (static_cast<float>(i) / 8.0f);
		message.pushVertex( { cos( alpha ) * 2.5f, sin( alpha ) * 2.5f, 0.0f, 1.0f, 0.2f, 0.2f } );
	}
	message.loop();

	message.pushVertex( { 0.0f, 2.5f, 0.0f, 0.2f, 1.0f, 0.2f } );
	message.pushVertex( { -0.5f, 3.5f, 0.0f, 0.2f, 1.0f, 0.2f } );
	message.pushVertex( { 0.5f, 4.5f, 0.0f, 0.2f, 1.0f, 0.2f } );

	message.separate();

	renderer.queueDraw( message );

	if ( !isCheatActive( CHEAT_WAKKA ) ) {

		Game::GetScene().destroyActor( &getOwner() );
	}
}

void CherryPrefab::onEventMessage( pthn::IActorEventMessage* const t_message ) {

	{
		auto collisionMessage = t_message->as<Collision2DMessage>();
		if ( collisionMessage != nullptr ) {

			if ( collisionMessage->other.hasComponent<Player>() ) {

				collisionMessage->other.getComponent<HealthComponent>().heal( 1 );

				float randX = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 256.0f - 128.0f;
				float randY = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 256.0f - 128.0f;

				getOwner().getTransform().position = { randX, randY, 0.0f };
			}
		}
	}
}