
// player.cpp

// source file for player actor

#include "player.hpp"

#include "bullet.hpp"
#include "explode_component.hpp"
#include "health_component.hpp"
#include "line_text_helpers.hpp"
#include "ship_move_component.hpp"
#include "shoot_component.hpp"
#include "game_state_prefab.hpp"

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

glm::vec3 g_colors[4] = 
	{ { 1.0f, 1.0f, 0.0f }
	, { 0.0f, 1.0f, 1.0f }
	, { 1.0f, 0.0f, 1.0f }
	, { 0.0f, 1.0f, 0.0f } };
glm::vec2 g_spawns[4] =
	{ { -200.0f, 50.0f }
	, { 200.0f, 50.0f }
	, { -200.0f, -50.0f }
	, { 200.0f, -50.0f } };
float g_spawnRotations[4] =
	{ PI2 * 0.75f
	, PI2 * 0.25f
	, PI2 * 0.75f
	, PI2 * 0.25f };
glm::vec3 g_vertices[4] =
{ glm::vec3( -sin( PI2 * 0.0f ) * 5.0f, cos( PI2 * 0.0f ) * 5.0f, 0.0f )
, glm::vec3( -sin( PI2 * 0.33f ) * 5.0f, cos( PI2 * 0.33f ) * 5.0f, 0.0f )
, glm::vec3( 0.0f, 0.0f, 0.0f )
, glm::vec3( -sin( PI2 * 0.66f ) * 5.0f, cos( PI2 * 0.66f ) * 5.0f, 0.0f ) };

std::vector<LineRendererVertex> g_playerIcons[4] = {
	{ LineRendererVertex( g_vertices[0][0], g_vertices[0][1], g_vertices[0][2]
		, g_colors[0][0], g_colors[0][1], g_colors[0][2] )
	, LineRendererVertex( g_vertices[1][0], g_vertices[1][1], g_vertices[1][2]
		, g_colors[0][0], g_colors[0][1], g_colors[0][2] )
	, LineRendererVertex( g_vertices[2][0], g_vertices[2][1], g_vertices[2][2]
		, g_colors[0][0], g_colors[0][1], g_colors[0][2] )
	, LineRendererVertex( g_vertices[3][0], g_vertices[3][1], g_vertices[3][2]
		, g_colors[0][0], g_colors[0][1], g_colors[0][2] )
	, LineRendererVertex::separator()
	, LineRendererVertex::separator()
	, LineRendererVertex( -sin( PI2 * 0.47f ) * 4.0f, cos( PI2 * 0.47f ) * 4.0f, 0.0f
		, g_colors[0][0], g_colors[0][1], g_colors[0][2] )
	, LineRendererVertex( -sin( PI2 * 0.53f ) * 4.0f, cos( PI2 * 0.53f ) * 4.0f, 0.0f
		, g_colors[0][0], g_colors[0][1], g_colors[0][2] )
	, LineRendererVertex::separator()
	, LineRendererVertex( -sin( PI2 * 0.47f ) * 6.0f, cos( PI2 * 0.47f ) * 6.0f, 0.0f
		, g_colors[0][0], g_colors[0][1], g_colors[0][2] )
	, LineRendererVertex( -sin( PI2 * 0.53f ) * 6.0f, cos( PI2 * 0.53f ) * 6.0f, 0.0f
		, g_colors[0][0], g_colors[0][1], g_colors[0][2] )
	, LineRendererVertex::separator() }
	,
	{ LineRendererVertex( g_vertices[0][0], g_vertices[0][1], g_vertices[0][2]
		, g_colors[1][0], g_colors[1][1], g_colors[1][2] )
	, LineRendererVertex( g_vertices[1][0], g_vertices[1][1], g_vertices[1][2]
		, g_colors[1][0], g_colors[1][1], g_colors[1][2] )
	, LineRendererVertex( g_vertices[2][0], g_vertices[2][1], g_vertices[2][2]
		, g_colors[1][0], g_colors[1][1], g_colors[1][2] )
	, LineRendererVertex( g_vertices[3][0], g_vertices[3][1], g_vertices[3][2]
		, g_colors[1][0], g_colors[1][1], g_colors[1][2] )
	, LineRendererVertex::separator()
	, LineRendererVertex::separator()
	, LineRendererVertex( -sin( PI2 * 0.47f ) * 5.0f, cos( PI2 * 0.47f ) * 5.0f, 0.0f
		, g_colors[1][0], g_colors[1][1], g_colors[1][2] )
	, LineRendererVertex( -sin( PI2 * 0.53f ) * 5.0f, cos( PI2 * 0.53f ) * 5.0f, 0.0f
		, g_colors[1][0], g_colors[1][1], g_colors[1][2] )
	, LineRendererVertex::separator()
	, LineRendererVertex( -sin( PI2 * 0.5f ) * 4.0f, cos( PI2 * 0.5f ) * 4.0f, 0.0f
		, g_colors[1][0], g_colors[1][1], g_colors[1][2] )
	, LineRendererVertex( -sin( PI2 * 0.5f ) * 6.0f, cos( PI2 * 0.5f ) * 6.0f, 0.0f
		, g_colors[1][0], g_colors[1][1], g_colors[1][2] )
	, LineRendererVertex::separator() }
	,
	{ LineRendererVertex( g_vertices[0][0], g_vertices[0][1], g_vertices[0][2]
		, g_colors[2][0], g_colors[2][1], g_colors[2][2] )
	, LineRendererVertex( g_vertices[1][0], g_vertices[1][1], g_vertices[1][2]
		, g_colors[2][0], g_colors[2][1], g_colors[2][2] )
	, LineRendererVertex( g_vertices[2][0], g_vertices[2][1], g_vertices[2][2]
		, g_colors[2][0], g_colors[2][1], g_colors[2][2] )
	, LineRendererVertex( g_vertices[3][0], g_vertices[3][1], g_vertices[3][2]
		, g_colors[2][0], g_colors[2][1], g_colors[2][2] )
	, LineRendererVertex::separator()
	, LineRendererVertex::separator()
	, LineRendererVertex( -sin( PI2 * 0.47f ) * 4.0f, cos( PI2 * 0.47f ) * 4.0f, 0.0f
		, g_colors[2][0], g_colors[2][1], g_colors[2][2] )
	, LineRendererVertex( -sin( PI2 * 0.47f ) * 6.0f, cos( PI2 * 0.47f ) * 6.0f, 0.0f
		, g_colors[2][0], g_colors[2][1], g_colors[2][2] )
	, LineRendererVertex::separator()
	, LineRendererVertex( -sin( PI2 * 0.53f ) * 4.0f, cos( PI2 * 0.53f ) * 4.0f, 0.0f
		, g_colors[2][0], g_colors[2][1], g_colors[2][2] )
	, LineRendererVertex( -sin( PI2 * 0.53f ) * 6.0f, cos( PI2 * 0.53f ) * 6.0f, 0.0f
		, g_colors[2][0], g_colors[2][1], g_colors[2][2] )
	, LineRendererVertex::separator() }
	,
	{ LineRendererVertex( g_vertices[0][0], g_vertices[0][1], g_vertices[0][2]
		, g_colors[3][0], g_colors[3][1], g_colors[3][2] )
	, LineRendererVertex( g_vertices[1][0], g_vertices[1][1], g_vertices[1][2]
		, g_colors[3][0], g_colors[3][1], g_colors[3][2] )
	, LineRendererVertex( g_vertices[2][0], g_vertices[2][1], g_vertices[2][2]
		, g_colors[3][0], g_colors[3][1], g_colors[3][2] )
	, LineRendererVertex( g_vertices[3][0], g_vertices[3][1], g_vertices[3][2]
		, g_colors[3][0], g_colors[3][1], g_colors[3][2] )
	, LineRendererVertex::separator()
	, LineRendererVertex::separator()
	, LineRendererVertex( -sin( PI2 * 0.47f ) * 6.0f, cos( PI2 * 0.47f ) * 4.0f, 0.0f
		, g_colors[3][0], g_colors[3][1], g_colors[3][2] )
	, LineRendererVertex( -sin( PI2 * 0.53f ) * 4.0f, cos( PI2 * 0.53f ) * 6.0f, 0.0f
		, g_colors[3][0], g_colors[3][1], g_colors[3][2] )
	, LineRendererVertex::separator()
	, LineRendererVertex( -sin( PI2 * 0.53f ) * 6.0f, cos( PI2 * 0.53f ) * 4.0f, 0.0f
		, g_colors[3][0], g_colors[3][1], g_colors[3][2] )
	, LineRendererVertex( -sin( PI2 * 0.47f ) * 4.0f, cos( PI2 * 0.47f ) * 6.0f, 0.0f
		, g_colors[3][0], g_colors[3][1], g_colors[3][2] )
	, LineRendererVertex::separator() } };

Player* g_players[4]{ nullptr, nullptr, nullptr, nullptr };

Player::Player( pantheon::ConstructComponentPermit& t_permit, Actor& t_owner, const PlayerInfo& t_info )
	: IActorComponent( t_permit, t_owner ), m_index{ t_info.index }
	, m_input{ PlayerInputProfile::fromIndex( t_info.index ) }
	, m_colliders{ { t_owner.getTransform() }, { t_owner.getTransform() } } {

	g_players[t_info.index] = this;

	t_owner.createComponent<Collision2DComponent>();
	t_owner.createComponent<PhysicsComponent2D>();
	t_owner.createComponent<ShipMoveComponent>();
	t_owner.createComponent<ShootComponent>();
	t_owner.createComponent<HealthComponent>();

	Transform& transform = t_owner.getTransform();
	transform.position = glm::vec3{ g_spawns[t_info.index], 0.0f };
	transform.rotation = glm::angleAxis( g_spawnRotations[m_index], glm::vec3( 0.0f, 0.0f, 1.0f ) );

	setupColliders();
	setupPhysics();
	setupSounds();
}

Player::~Player() {

	g_players[m_index] = nullptr;

	Game::GetAudio().deleteSource( m_bonkSource );
	Game::GetAudio().deleteSource( m_wooshSource );
}

void Player::setupColliders() {

	// set up both triangles

	m_colliders[0].points.push_back( g_vertices[0] );
	m_colliders[0].points.push_back( g_vertices[1] );
	m_colliders[0].points.push_back( g_vertices[2] );

	m_colliders[1].points.push_back( g_vertices[2] );
	m_colliders[1].points.push_back( g_vertices[3] );
	m_colliders[1].points.push_back( g_vertices[0] );

	// add them to the collider

	Collision2DComponent& collisionComponent =
		getOwner().getComponent<Collision2DComponent>();
	collisionComponent.addCollider( &m_colliders[0] );
	collisionComponent.addCollider( &m_colliders[1] );
	unsigned int collisionFlags = 1 << m_index;
	collisionComponent.setCollisionFlags( collisionFlags );
	collisionComponent.setCollideWithFlags( 0xffffffff & ~collisionFlags );
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

void Player::setupSounds() {

	Audio& audio = Game::GetAudio();
	m_bonkSource = audio.createSource( "bonk" );
	m_wooshSource = audio.createSource( "woosh" );
	m_humSource = audio.createSource( "hum" );
	m_humSource->setGain( 0.1f );
	m_humSource->setPitch( 0.5f );
	m_humSource->setLooping();
	m_humSource->play();
}

void Player::update( float t_delta ) {

	const Input& input = Game::GetInput();
	PhysicsComponent2D& physics = getOwner().getComponent<PhysicsComponent2D>();
	Transform& transform = getOwner().getTransform();

	if ( !isCheatActive( CHEAT_JUGGERNAUGHT ) && m_isJuggernaught ) {
		
		m_isJuggernaught = false;
		findSize();
	}

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
		else if ( m_shieldTimer <= 0.0f ) {

			getOwner().getComponent<HealthComponent>().enable();
		}
	}

	// handle movement

	getOwner().getComponent<ShipMoveComponent>().update
		( t_delta
			, input.getAxisValue( m_input.rotateCCW, m_input.rotateCW, 0.2f )
			, input.getAxisValue( m_input.backward, m_input.forward, 0.2f ) );

	// handle boost

	if ( m_boostTimer > 0.0f ) {
	
		m_boostTimer -= t_delta;
	}
	if ( input.getAxisValue( m_input.boost ) < 0.5f && m_boostTimer <= 0.0f ) {

		physics.velocity += glm::vec2( transform.findUp() ) * ( m_shieldTimer > 0.0f ? 200.0f : 160.0f );
		shake( 1.0f );
		m_wooshSource->play();
		m_boostTimer = 2.0f;
	}

	// handle shooting

	if ( input.isKeyDown( m_input.fire ) && m_shieldTimer <= 0.0f ) {

		if ( getOwner().getComponent<ShootComponent>().shoot( g_colors[m_index], 1 << m_index ) ) {

			physics.velocity += glm::vec2( transform.findDown() ) * (isCheatActive( CHEAT_WHEELCHAIR ) ? 64.0f : 4.0f);

			if ( isJuggernaught() ) {

				shake( 0.33f );
			}
			else {

				shake( 0.1f );
			}
		}
	}

	// handle audio

	float speed = glm::length( physics.velocity );
	m_humSource->setGain( speed / 80.0f );
	m_humSource->setPitch( speed / 130.0f );
}

void Player::render() {

	// get renderer and check it is compatible with player

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}

	// create score message and queue for rendering

	std::string scoreString = std::to_string( getOwner().getComponent<HealthComponent>().health );
	LineRendererMessage messageB = stringToLines( scoreString
		, glm::vec3( g_spawns[m_index], 0.0f ) + glm::vec3( scoreString.size() * -2.5f, -20.0f, 0.0f ), g_colors[m_index] );
	renderer->queueDraw( messageB );

	// if dead don't render player

	if ( m_respawnTimer > 0.0f ) {

		return;
	}

	// handle colour

	glm::vec3 colour = g_colors[m_index];

	if ( m_shieldTimer > 0.0f ) {

		colour = g_colors[m_index] * cos( m_shieldTimer * PI2 * 2.0f );
	}

	// create render message and queue to draw

	Transform2D transform = getOwner().getTransform2D();
	LineRendererMessage message{ g_playerIcons[m_index].begin(), g_playerIcons[m_index].end(), transform.findMatrix() };
	renderer->queueDraw( message );
}

void Player::onEventMessage( IActorEventMessage* const t_message ) {
	
	{
		PhysicsCollisionMessage2D* physMessage = t_message->as<PhysicsCollisionMessage2D>();
		if ( physMessage != nullptr ) {

			physMessage->other.velocity += 32.0f * glm::normalize(
				physMessage->other.getOwner().getTransform2D().position - getOwner().getTransform2D().position);
			shake( 1.0f );
			m_bonkSource->play();
		}
	}
	{
		HealMessage* healMessage = t_message->as<HealMessage>();
		if ( healMessage != nullptr ) {

			findSize();
		}
	}
	{
		DamageMessage* damageMessage = t_message->as<DamageMessage>();
		if ( damageMessage != nullptr ) {

			auto& healthComponent = getOwner().getComponent<HealthComponent>();
			
			if ( isKillable() ) {

				kill();
				healthComponent.health = healthComponent.health < 0 ? 0 : healthComponent.health;
				findSize();
			}
			else {

				healthComponent.health += damageMessage->damage;
			}
		}
	}
}

bool Player::kill() {

	if ( m_shieldTimer <= 0.0f ) {

		m_respawnTimer = 1.0f;
		m_boostTimer = 0.0f;
		Actor& owner = getOwner();
		ExplodeInfo info = ExplodeInfo( owner.getTransform()
			, owner.getComponent<ShipMoveComponent>().getVelocity() );
		Game::GetScene().createPrefab<ExplodeComponent>( info );
		owner.getTransform().reset();
		owner.getTransform().position = { g_spawns[m_index][0], g_spawns[m_index][1], 0.0f };
		owner.getTransform().rotation = glm::angleAxis( g_spawnRotations[m_index], glm::vec3( 0.0f, 0.0f, 1.0f ) );
		owner.getComponent<ShipMoveComponent>().reset();
		owner.getComponent<Collision2DComponent>().disable();
		owner.getComponent<HealthComponent>().disable();
		shake( 2.5f );
		return true;
	}
	return false;
}

bool Player::isKillable() {

	return m_shieldTimer <= 0.0f;
}

void Player::makeJuggernaught() {

	m_isJuggernaught = true;
}

void Player::takeJuggernaught() {

	m_isJuggernaught = false;
}

bool Player::isJuggernaught() {

	return m_isJuggernaught;
}

void Player::findSize() {

	auto& health = getOwner().getComponent<HealthComponent>();
	if ( isJuggernaught() ) {

		getOwner().getTransform().scale = glm::vec3( 1.0f, 1.0f, 1.0f )
			* (2.0f + health.health * 0.66f);
	}
	else {

		getOwner().getTransform().scale = glm::vec3( 1.0f, 1.0f, 1.0f )
			* (1.0f + health.health * 0.33f);
	}
}

void Player::beginJuggernaught() {

	int index = rand() % 4;

	if ( g_players[index] != nullptr ) {

		g_players[index]->m_isJuggernaught = true;
		g_players[index]->findSize();
	}
	else {

		g_players[0]->m_isJuggernaught = true;
		g_players[0]->findSize();
	}
}

void Player::reset() {

	for ( int i = 0; i < 4; ++i ) {

		if ( g_players[i] != nullptr ) {

			Player& player = *g_players[i];
			auto& health = player.getOwner().getComponent<HealthComponent>();
			health.damage( 999 );
			health.health = 0;
			player.findSize();

		}
	}
}