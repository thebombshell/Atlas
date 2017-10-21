
// player.cpp

// source file for player actor

#include "player.hpp"

#include "bullet.hpp"
#include "explode_component.hpp"
#include "ship_move_component.hpp"
#include "shoot_component.hpp"
#include "line_text_helpers.hpp"

#include <collision_2d.hpp>
#include <line_renderer.hpp>

#define _USE_MATH_DEFINES

#include <algorithm>
#include <iostream>
#include <math.h>

using namespace atlas;
using namespace pantheon;

const float PI2 = float( M_PI ) * 2;

Player::Player( pantheon::ConstructComponentPermit& t_permit, Actor& t_owner, const PlayerInfo& t_info )
	: IActorComponent( t_permit, t_owner ), m_index{ t_info.index }
	, m_input{ PlayerInputProfile::fromIndex( t_info.index ) }
	, m_colliders{ { t_owner.getTransform() }, { t_owner.getTransform() } } {

	t_owner.createComponent<Collision2DComponent>();
	t_owner.createComponent<ShipMoveComponent>();
	t_owner.createComponent<ShootComponent>();

	t_owner.getTransform().position = t_info.position;

	setupVertices();
	setupColliders();
}

Player::~Player() {

}

void Player::setupColliders() {

	// set up both triangles

	m_colliders[0].points.push_back( m_vertices[0] );
	m_colliders[0].points.push_back( m_vertices[1] );
	m_colliders[0].points.push_back( m_vertices[2] );

	m_colliders[1].points.push_back( m_vertices[2] );
	m_colliders[1].points.push_back( m_vertices[3] );
	m_colliders[1].points.push_back( m_vertices[0] );

	// add them to the collider

	Collision2DComponent& collisionComponent =
		getOwner().getComponent<Collision2DComponent>();
	collisionComponent.addCollider( &m_colliders[0] );
	collisionComponent.addCollider( &m_colliders[1] );
}

void Player::setupVertices() {

	// find triangle points and center point

	glm::vec3 vertices[] =
	{ glm::vec3
		( -sin( PI2 * 0.0f ) * 5.0f, cos( PI2 * 0.0f ) * 5.0f, 0.0f )
		, glm::vec3
		( -sin( PI2 * 0.33f ) * 5.0f, cos( PI2 * 0.33f ) * 5.0f, 0.0f )
		, glm::vec3( 0.0f, 0.0f, 0.0f )
		, glm::vec3
		( -sin( PI2 * 0.66f ) * 5.0f, cos( PI2 * 0.66f ) * 5.0f, 0.0f ) };
	std::copy( vertices, vertices + 4, m_vertices );
}

void Player::update( float t_delta ) {

	const Input& input = Game::GetInput();

	// handle death

	if ( m_respawnTimer > 0.0f ) {

		m_respawnTimer -= t_delta;
		m_shieldTimer = 3.0f;
		if ( m_respawnTimer <= 0.0f ) {

			getOwner().getComponent<Collision2DComponent>().enable();
		}
	}
	else if ( m_shieldTimer > 0.0f ) {

		m_shieldTimer -= t_delta;
	}

	// handle movement

	getOwner().getComponent<ShipMoveComponent>().update
		( t_delta
			, input.getAxisValue( m_input.rotateCCW, m_input.rotateCW, 0.1f )
			, input.getAxisValue( m_input.backward, m_input.forward, 0.1f ) );

	// handle shooting

	if ( input.isKeyDown( m_input.fire ) && m_shieldTimer <= 0.0f ) {

		getOwner().getComponent<ShootComponent>().shoot();
	}
}

void Player::render() {

	if ( m_respawnTimer > 0.0f ) {

		return;
	}

	// get renderer and check it is compatible with player

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}

	// handle colour

	glm::vec3 colour = { 1.0f, 1.0f, 1.0f };

	if ( m_shieldTimer > 0.0f ) {

		colour = glm::vec3(1.0f, 1.0f, 1.0f) * cos( m_shieldTimer * PI2 * 2.0f );
	}

	// create render message and queue to draw

	LineRendererVertex vertices[4]
	{ { m_vertices[0][0], m_vertices[0][1], m_vertices[0][2]
		, colour[0], colour[1], colour[2] }
		, { m_vertices[1][0], m_vertices[1][1], m_vertices[1][2]
		, colour[0], colour[1], colour[2] }
		, { m_vertices[2][0], m_vertices[2][1], m_vertices[2][2]
		, colour[0], colour[1], colour[2] }
		, { m_vertices[3][0], m_vertices[3][1], m_vertices[3][2]
		, colour[0], colour[1], colour[2] } };
	LineRendererMessage message
	{ vertices, vertices + 4, getOwner().getTransform2D().findMatrix() };
	message.loop();

	renderer->queueDraw( message );
}

void Player::onEventMessage( IActorEventMessage* const t_message ) {

	{
		auto collisionMessage = t_message->as<Collision2DMessage>();
		if ( collisionMessage != nullptr
			&& collisionMessage->other.hasComponent<Player>()) {

			collisionMessage->other.getComponent<Player>().kill();
			kill();
		}
	}
}

bool Player::kill() {

	if ( m_shieldTimer <= 0.0f ) {

		m_respawnTimer = 5.0f;
		Actor& owner = getOwner();
		ExplodeInfo info = ExplodeInfo( owner.getTransform().position
			, owner.getComponent<ShipMoveComponent>().getVelocity()
			, owner.getTransform().scale[0] );
		Game::GetScene().createPrefab<ExplodeComponent>( info );
		owner.getTransform().reset();
		float offset = (PI2 / 8) * m_index;
		owner.getTransform().position = { cos( offset ) * 50.0f, sin( offset ) * 50.0f, 0.0f };
		owner.getComponent<ShipMoveComponent>().reset();
		owner.getComponent<Collision2DComponent>().disable();
		return true;
	}
	return false;
}

void Player::grow() {

	getOwner().getTransform().scale += glm::vec3( 1.0f, 1.0f, 1.0f );
}