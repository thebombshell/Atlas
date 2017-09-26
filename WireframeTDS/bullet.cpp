
// bullet.cpp

// Source file for the bullet class

#include "bullet.hpp"
#include "game.hpp"
#include "player_ship.hpp"

using namespace atlas;

game::Bullet::~Bullet(
	) {


}
const collisions::ICollider* game::Bullet::getCollider(
	) const {

	return &m_collider;
}
void game::Bullet::onCollision(
	const glm::vec3&,
	IGameActor& t_other
	) {

	PlayerShip* otherShip = dynamic_cast<PlayerShip*>(&t_other);
	if ( otherShip != nullptr && &t_other != m_owner ) {

		if ( otherShip->getPlayerState() == WTDS_PLAYER_ALIVE ) {
			
			otherShip->killPlayer();
			PlayerShip* ownerShip = dynamic_cast<PlayerShip*>(m_owner);
			if ( ownerShip != nullptr ) {

				ownerShip->attributeKill();
			}
		}
		Game::removeActor( this );
	}
}

void game::Bullet::onRender(
	float
	) const {

	float length = 1.0f - m_age;
	glm::vec2 pos1 = m_position - m_direction * length;
	glm::vec2 pos2 = m_position + m_direction * length;
	float lines[] = {
		pos1[0], pos1[1], 0.5f, 1.0f, 0.0f, 0.0f,
		pos2[0], pos2[1], 0.5f, 1.0f, 0.0f, 0.0f
	};

	Game::drawLines( 2, lines );
}

void game::Bullet::onUpdate(
	float t_delta
	) {

	// track age and self destruction

	m_age += t_delta;
	if ( m_age > 1.0f ) {
		Game::removeActor( this );
	}

	// update position

	m_position += m_direction * t_delta * 60.0f;

	// handle screen wrap

	if ( m_position[0] > 50.0f ) {

		m_position[0] -= 100.0f;
	}
	if ( m_position[0] < -50.0f ) {

		m_position[0] += 100.0f;
	}
	if ( m_position[1] > 50.0f ) {

		m_position[1] -= 100.0f;
	}
	if ( m_position[1] < -50.0f ) {

		m_position[1] += 100.0f;
	}

	// update collider

	m_collider.offset = m_position;
}