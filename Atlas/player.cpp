
// player.cpp

// source file for player actor

#include "player.hpp"

#include "bullet.hpp"
#include "explode_component.hpp"
#include "ship_move_component.hpp"
#include "shoot_component.hpp"
#include "line_text_helpers.hpp"

#include <collision_2d.hpp>
#include <physics_component.hpp>
#include <line_renderer.hpp>

#define _USE_MATH_DEFINES

#include <algorithm>
#include <iostream>
#include <math.h>

using namespace atlas;
using namespace pantheon;

const float PI2 = float( M_PI ) * 2;

glm::vec3 cols[4] = 
	{ { 1.0f, 1.0f, 0.0f }
	, { 0.0f, 1.0f, 1.0f }
	, { 1.0f, 0.0f, 1.0f }
	, { 0.0f, 1.0f, 0.0f } };
glm::vec2 spawns[4] =
	{ { -200.0f, 50.0f }
	, { 200.0f, 50.0f }
	, { -200.0f, -50.0f }
	, { 200.0f, -50.0f } };

Player::Player( pantheon::ConstructComponentPermit& t_permit, Actor& t_owner, const PlayerInfo& t_info )
	: IActorComponent( t_permit, t_owner ), m_index{ t_info.index }
	, m_input{ PlayerInputProfile::fromIndex( t_info.index ) }
	, m_colliders{ { t_owner.getTransform() }, { t_owner.getTransform() } } {

	t_owner.createComponent<Collision2DComponent>();
	t_owner.createComponent<PhysicsComponent2D>();
	t_owner.createComponent<ShipMoveComponent>();
	t_owner.createComponent<ShootComponent>();

	Transform& transform = t_owner.getTransform();
	transform.position = glm::vec3{ spawns[t_info.index], 0.0f };

	setupVertices();
	setupColliders();
	setupPhysics();
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
	unsigned int collisionFlags = 1 << m_index;
	collisionComponent.setCollisionFlags( collisionFlags );
	collisionComponent.setCollideWithFlags( 0xffffffff & ~collisionFlags );
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

void Player::setupPhysics() {

	PhysicsComponent2D& physics = getOwner().getComponent<PhysicsComponent2D>();

	physics.setSolid();
	physics.setDynamic();
	physics.setGravityDisabled();
	physics.bounce = 1.0f;
	physics.friction = 0.0f;
	physics.mass = 1.0f;
}

void Player::update( float t_delta ) {

	const Input& input = Game::GetInput();
	PhysicsComponent2D& physics = getOwner().getComponent<PhysicsComponent2D>();
	Transform& transform = getOwner().getTransform();

	// handle death

	if ( m_respawnTimer > 0.0f ) {

		m_respawnTimer -= t_delta;
		m_shieldTimer = 1.0f;
		if ( m_respawnTimer <= 0.0f ) {

			getOwner().getComponent<Collision2DComponent>().enable();
		}
		return;
	}
	else if ( m_shieldTimer > 0.0f ) {

		m_shieldTimer -= t_delta;

		if ( transform.position[0] > 128.0f || transform.position[0] < -128.0f
			|| transform.position[1] > 127.0f || transform.position[1] < -128.0f ) {
			m_shieldTimer = 1.0f;
		}
	}

	// handle movement

	getOwner().getComponent<ShipMoveComponent>().update
		( t_delta
			, input.getAxisValue( m_input.rotateCCW, m_input.rotateCW, 0.1f )
			, input.getAxisValue( m_input.backward, m_input.forward, 0.1f ) );

	// handle boost

	if ( m_boostTimer > 0.0f ) {
	
		m_boostTimer -= t_delta;
	}
	if ( input.getAxisValue( m_input.boost ) > 1.0f && m_boostTimer <= 0.0f ) {

		physics.velocity += glm::vec2( transform.findUp() ) * 128.0f;
		m_boostTimer = 1.0f;
	}

	// handle shooting

	if ( input.isKeyDown( m_input.fire ) && m_shieldTimer <= 0.0f ) {

		getOwner().getComponent<ShootComponent>().shoot( cols[m_index], 1 << m_index );
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

	glm::vec3 colour = cols[m_index];

	if ( m_shieldTimer > 0.0f ) {

		colour = cols[m_index] * cos( m_shieldTimer * PI2 * 2.0f );
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
		PhysicsCollisionMessage2D* physMessage = t_message->as<PhysicsCollisionMessage2D>();
		if ( physMessage != nullptr ) {

			physMessage->other.velocity += 32.0f * glm::normalize(
				physMessage->other.getOwner().getTransform2D().position - getOwner().getTransform2D().position);
		}
	}
}

bool Player::kill() {

	if ( m_shieldTimer <= 0.0f ) {

		m_respawnTimer = 1.0f;
		Actor& owner = getOwner();
		ExplodeInfo info = ExplodeInfo( owner.getTransform()
			, owner.getComponent<ShipMoveComponent>().getVelocity() );
		Game::GetScene().createPrefab<ExplodeComponent>( info );
		owner.getTransform().reset();
		owner.getTransform().position = { spawns[m_index][0], spawns[m_index][1], 0.0f };
		owner.getComponent<ShipMoveComponent>().reset();
		owner.getComponent<Collision2DComponent>().disable();
		score( -1 );
		return true;
	}
	return false;
}

bool Player::isKillable() {

	return m_shieldTimer <= 0.0f;
}

void Player::score(int t_ammount) {

	m_score += t_ammount;
	m_score = m_score < 0 ? 0 : m_score;
	getOwner().getTransform().scale = glm::vec3( 1.0f, 1.0f, 1.0f ) * (1.0f + m_score * 0.33f);
}

int Player::getScore() {

	return m_score;
}