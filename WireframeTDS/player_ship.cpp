
// player_ship.cpp

// Source file for the player ship class

#include "player_ship.hpp"
#include "bullet.hpp"
#include "collisions.hpp"
#include "game.hpp"
#include <assert.h>
#include <iostream>
const float pi = 3.14159265358979323846f;
const float pi2 = pi * 2;

using namespace wireframe_tds;

game::PlayerShip::PlayerShip(
	const int& t_index
	) {

	// assert player index is in range

	assert( t_index >= 0 );
	assert( t_index < 16 );

	switch ( t_index ) {
		case 0:
			m_forwardInput = "w";
			m_backwardInput = "s";
			m_clockwiseInput = "d";
			m_counterClockwiseInput = "a";
			m_fireInput = "e";
			m_transform.position = { -25.0f, 0.0f };

			break;
		case 1:
			m_forwardInput = "i";
			m_backwardInput = "k";
			m_clockwiseInput = "l";
			m_counterClockwiseInput = "j";
			m_fireInput = "o";
			m_transform.position = { 25.0f, 0.0f };
			break;
	}
}

game::PlayerShip::~PlayerShip(
	) {

}

const collisions::ICollider* game::PlayerShip::getCollider(
	) const {

	return &m_collider;
}

void game::PlayerShip::onCollision(
	const glm::vec3&,
	IGameActor& t_other
	) {

	PlayerShip* otherShip = dynamic_cast<PlayerShip*>(&t_other);
	if ( otherShip != nullptr 
		&& otherShip->getPlayerState() == WTDS_PLAYER_ALIVE 
		&& m_playerState == WTDS_PLAYER_ALIVE) {

		otherShip->killPlayer();
		killPlayer();
	}
}

void game::PlayerShip::onRender(
	float 
	) const {

	// if dead dont draw

	if ( m_playerState == WTDS_PLAYER_DEAD ) {

		return;
	}

	// if respawning apply strobing effect

	glm::vec3 color = { 1.0f, 1.0f, 1.0f };
	if ( m_playerState == WTDS_PLAYER_RESPAWNING ) {

		color *= cos( m_respawnTimer * pi2 ) * 0.5f + 0.5f;
	}

	float elements[] = {
		m_transform.position[0], m_transform.position[1], 0.5f, color[0], color[1], color[2],
		m_points[0][0], m_points[0][1], 0.5f, color[0], color[1], color[2],

		m_points[0][0], m_points[0][1], 0.5f, color[0], color[1], color[2],
		m_points[1][0], m_points[1][1], 0.5f, color[0], color[1], color[2],

		m_points[1][0], m_points[1][1], 0.5f, color[0], color[1], color[2],
		m_points[2][0], m_points[2][1], 0.5f, color[0], color[1], color[2],

		m_points[2][0], m_points[2][1], 0.5f, color[0], color[1], color[2],
		m_transform.position[0], m_transform.position[1], 0.5f, color[0], color[1], color[2]
	};
	Game::drawLines( 8, elements );
}

void game::PlayerShip::onUpdate(
	float t_delta
	) {

	// handle timed states

	if ( m_playerState == WTDS_PLAYER_DEAD ) {

		m_deathTimer -= t_delta;
		if ( m_deathTimer > 0.0f ) {
			
			return;
		}
		m_playerState = WTDS_PLAYER_RESPAWNING;
		m_respawnTimer = 3.0f;
	}

	if ( m_respawnTimer > 0.0f ) {

		m_respawnTimer -= t_delta;
		if ( m_respawnTimer <= 0.0f ) {

			m_playerState = WTDS_PLAYER_ALIVE;
		}
	}

	// handle rotation

	m_rotationSpeed -= m_rotationSpeed * 4.0f * t_delta;
	if ( Game::isKeyDown( m_clockwiseInput ) ) {

		m_rotationSpeed -= 16.0f * t_delta;
	}
	if ( Game::isKeyDown( m_counterClockwiseInput ) ) {

		m_rotationSpeed += 16.0f * t_delta;
	}
	m_transform.rotation += m_rotationSpeed * t_delta;
	m_transform.rotation = fmod(m_transform.rotation, pi2);

	// handle thrust
	
	glm::vec2 up = m_transform.findUp();

	m_thrust -= m_thrust * t_delta;
	if ( Game::isKeyDown( m_forwardInput ) ) {
		
		m_thrust += up * m_transform.scale[0] * 8.0f * t_delta;
	}
	if ( Game::isKeyDown( m_backwardInput ) ) {

		m_thrust -= up * m_transform.scale[0] * 8.0f * t_delta;
	}
	m_transform.position += m_thrust * t_delta;

	// handle cooldown

	if ( m_cooldown > 0.0f ) {

		m_cooldown -= t_delta;
	}

	// handle firing
	
	if ( Game::isKeyDown( m_fireInput ) && m_cooldown <= 0.0f) {

		Game::addActor<game::Bullet>( game::BulletInfo( m_transform.position , up, this ) );
		m_cooldown = 0.32f;
	}
	// handle screen wrap

	if ( m_transform.position[0] > 50.0f ) {

		m_transform.position[0] -= 100.0f;
	}
	if ( m_transform.position[0] < -50.0f ) {

		m_transform.position[0] += 100.0f;
	}
	if ( m_transform.position[1] > 50.0f ) {

		m_transform.position[1] -= 100.0f;
	}
	if ( m_transform.position[1] < -50.0f ) {

		m_transform.position[1] += 100.0f;
	}

	// update points

	m_points[2] = {
		m_transform.position[0] + sin( pi2 * 0.666f - m_transform.rotation ) * m_transform.scale[0],
		m_transform.position[1] + cos( pi2 * 0.666f - m_transform.rotation ) * m_transform.scale[1]
	};

	m_points[1] = {
		m_transform.position[0] + sin( pi2 * 1.0f - m_transform.rotation ) * m_transform.scale[0],
		m_transform.position[1] + cos( pi2 * 1.0f - m_transform.rotation ) * m_transform.scale[1]
	};

	m_points[0] = {
		m_transform.position[0] + sin( pi2 * 0.333f - m_transform.rotation ) * m_transform.scale[0],
		m_transform.position[1] + cos( pi2 * 0.333f - m_transform.rotation ) * m_transform.scale[1]
	};

	// update collider

	m_collider.points[0] = m_points[0];
	m_collider.points[1] = m_points[1];
	m_collider.points[2] = m_points[2];
	m_collider.findAxes();
}

int game::PlayerShip::getPlayerState(
	) {

	return m_playerState;
}

// temporary

float secretCounter{ 0.0f };

void game::PlayerShip::killPlayer(
	) {

	m_transform.position = { sin( secretCounter ) * 25.0f, cos( secretCounter ) * 25.0f };
	++secretCounter;
	m_playerState = WTDS_PLAYER_DEAD;
	m_deathTimer = 1.0f;
	m_transform.scale = { 1.0f, 1.0f };
	m_transform.rotation = 0.0f;
}

void game::PlayerShip::attributeKill() {

	m_transform.scale += glm::vec2( 1.0f, 1.0f );
}