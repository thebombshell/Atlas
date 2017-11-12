
// black_hole_refab.cpp

#include "black_hole_prefab.hpp"
#include "game_state_prefab.hpp"

#include <physics_component.hpp>
#include <line_renderer.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

const float PI2{ float( M_PI ) * 2.0f };

using namespace pantheon;
using namespace atlas;

LineRendererMessage* g_message;

BlackHolePrefab::BlackHolePrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner )
	: IActorComponent{ t_permit, t_owner } {

	g_message = new LineRendererMessage();
	g_message->pushVertex( { 128.0f, sin( 0.000f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { 96.0f, sin( 0.125f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { 64.0f, sin( 0.250f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { 32.0f, sin( 0.375f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { 0.00f, sin( 0.500f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -32.0f, sin( 0.625f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -64.0f, sin( 0.750f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -96.0f, sin( 0.875f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -128.0f, sin( 1.000f * PI2 ) * 16.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->separate();
	g_message->pushVertex( { -sin( 0.000f * PI2 ) * 16.0f, 128.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -sin( 0.125f * PI2 ) * 16.0f, 96.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -sin( 0.250f * PI2 ) * 16.0f, 64.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -sin( 0.375f * PI2 ) * 16.0f, 32.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -sin( 0.500f * PI2 ) * 16.0f, 0.00f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -sin( 0.625f * PI2 ) * 16.0f, -32.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -sin( 0.750f * PI2 ) * 16.0f, -64.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -sin( 0.875f * PI2 ) * 16.0f, -96.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->pushVertex( { -sin( 1.000f * PI2 ) * 16.0f, -128.0f, 0.0f, 0.4f, 0.2f, 0.4f } );
	g_message->separate();

	m_timer = 0.0f;
}

BlackHolePrefab::~BlackHolePrefab() {

	delete g_message;
}

void BlackHolePrefab::update( float t_delta ) {

	Transform& transform = getOwner().getTransform();

	m_timer += t_delta;
	transform.rotation = glm::angleAxis( m_timer, glm::vec3( 0.0f, 0.0f, 1.0f ) );

	Game::GetScene().forEach( [&](Actor* const t_actor) {

		glm::vec3 position = t_actor->getTransform().position;

		if ( t_actor->hasComponent<PhysicsComponent2D>() ) {

			auto& physics = t_actor->getComponent<PhysicsComponent2D>();

			if ( (abs( position[0] ) < 128.0f || abs( position[1] ) < 128.0f) && glm::length2(position) > 0.001f ) {

				physics.velocity += glm::normalize( -glm::vec2( position ) ) * 48.0f * t_delta;
			}
		}
	} );

	if ( !isCheatActive( CHEAT_HAWKING ) ) {

		Game::GetScene().destroyActor( &getOwner() );
	}
}

void BlackHolePrefab::render() {

	auto& renderer = *Game::GetRendererAs<LineRenderer>();

	g_message->transform = getOwner().getTransform2D().findMatrix();

	LineRendererMessage message = *g_message;
	renderer.queueDraw( message );
}