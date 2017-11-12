
// force_pad_prefab.hpp

#include "force_pad_prefab.hpp"
#include "game_state_prefab.hpp"

#include <physics_component.hpp>
#include <line_renderer.hpp>

#define _USE_MATH_DEFINES

#include <math.h>

const float PI2 = float( M_PI ) * 2;

using namespace pantheon;
using namespace atlas;


ForcePadMessage::ForcePadMessage( int t_index ) : index{ t_index } {

}

ForcePadPrefab::ForcePadPrefab( ConstructComponentPermit& t_permit, Actor& t_owner, const ForcePadMessage& t_message )
	: IActorComponent{ t_permit, t_owner }, m_circle{ t_owner.getTransform(), 32.0f }, m_index{ t_message.index } {

	t_owner.createComponent<Collision2DComponent>();
	auto& collision = t_owner.getComponent<Collision2DComponent>();
	collision.setCollideWithFlags( 0xffff & ~(512) );
	collision.setCollisionFlags( 256 );
	collision.addCollider( &m_circle );
}
ForcePadPrefab::~ForcePadPrefab() {

}

void ForcePadPrefab::render() {

	float indexAlpha = static_cast<float>(m_index) * 0.25f * PI2 + getTimeAlpha() * 0.33f;

	if ( m_index == 1 || m_index == 3 ) {

		getOwner().getTransform().rotation = glm::angleAxis( getTimeAlpha(), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	}
	else {

		getOwner().getTransform().rotation = glm::angleAxis( -getTimeAlpha(), glm::vec3( 0.0f, 0.0f, 1.0f ) );
	}
	getOwner().getTransform().position = { sin( indexAlpha ) * 96.0f, cos( indexAlpha ) * 96.0f, 0.0f };

	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();

	LineRendererMessage message{ getOwner().getTransform2D().findMatrix() };

	for ( int i = 0; i < 8; ++i ) {

		float alpha = PI2 * (static_cast<float>(i) / 8.0f);
		message.pushVertex( { cos(alpha) * 32.0f , sin(alpha) * 32.0f, 0.0f, 0.2f, 0.8f, 0.8f } );
	}
	message.loop();

	message.pushVertex( { -16.0f, 0.0f, 0.0f, 0.2f, 0.8f, 0.8f } );
	message.pushVertex( { 0.0f, 24.0f, 0.0f, 0.2f, 0.8f, 0.8f } );
	message.pushVertex( { 16.0f, 0.0f, 0.0f, 0.2f, 0.8f, 0.8f } );

	message.separate();

	message.pushVertex( { -16.0f, -16.0f, 0.0f, 0.2f, 0.8f, 0.8f } );
	message.pushVertex( { 0.0f, 8.0f, 0.0f, 0.2f, 0.8f, 0.8f } );
	message.pushVertex( { 16.0f, -16.0f, 0.0f, 0.2f, 0.8f, 0.8f } );

	message.separate();

	renderer.queueDraw( message );

	if ( !isCheatActive( CHEAT_GTFO ) ) {

		Game::GetScene().destroyActor( &getOwner() );
	}
}

void ForcePadPrefab::onEventMessage( pthn::IActorEventMessage* const t_message ) {

	{
		Collision2DMessage* collisionMessage = t_message->as<Collision2DMessage>();
		if ( collisionMessage != nullptr ) {

			if ( collisionMessage->other.hasComponent<PhysicsComponent2D>() ) {

				auto& otherPhysics = collisionMessage->other.getComponent<PhysicsComponent2D>();
				otherPhysics.velocity += glm::vec2( getOwner().getTransform().findUp() ) * 256.0f * getTimeDelta();
			}
		}
	}
}