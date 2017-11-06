
// bullet.cpp

// source file for bullet class

#include "bullet.hpp"

#include "player.hpp"

#include <collision_2d.hpp>
#include <line_renderer.hpp>

using namespace atlas;
using namespace pantheon;

Bullet::Bullet( ConstructComponentPermit& t_permit, pantheon::Actor& t_owner
	, const BulletInfo& t_info ) : IActorComponent( t_permit, t_owner )
	, m_firer{ t_info.firer }, m_velocity{ t_info.velocity }, m_colour{ t_info.colour }
	, m_collider{ t_owner.getTransform(), 1.0f } {

	t_owner.createComponent<Collision2DComponent>();
	Collision2DComponent& collision = t_owner.getComponent<Collision2DComponent>();
	collision.addCollider( &m_collider );
	collision.setCollisionFlags(1 << 5);
	collision.setCollideWithFlags( (1 | 2 | 4 | 8) & ~t_info.flag );
	t_owner.getTransform().position = glm::vec3( t_info.position, 0.5f );
}

Bullet::~Bullet() {

}

void Bullet::update( float t_delta ) {

	m_timer += t_delta;
	if ( m_timer > 1.0f ) {

		Game::GetScene().destroyActor( &getOwner() );
	}
	getOwner().getTransform().position += glm::vec3( m_velocity, 0.0f ) * t_delta;
}

void Bullet::render() {

	// get renderer and check it is compatible with player

	LineRenderer* const renderer = Game::GetRendererAs<LineRenderer>();
	if ( renderer == nullptr ) {

		return;
	}

	// create render message and queue to draw

	Transform& transform = getOwner().getTransform();

	LineRendererVertex vertices[2]
		{ { transform.position[0], transform.position[1], 0.5f, m_colour[0] + 0.2f, m_colour[1] + 0.2f, m_colour[2] + 0.2f }
		, { transform.position[0] - m_velocity[0] * 0.08f
		, transform.position[1] - m_velocity[1] * 0.08f, 0.5f
		, 0.0f, 0.0f, 0.0f } };
	LineRendererMessage message{ vertices, vertices + 2 };
	message.separate();
	renderer->queueDraw( message );
}

void Bullet::onEventMessage( IActorEventMessage* const t_message ) {

	{
		auto collisionMessage = t_message->as<Collision2DMessage>();
		if ( collisionMessage != nullptr ) {

			if ( &collisionMessage->other != &m_firer
				&& collisionMessage->other.hasComponent<Player>() ) {

				Player& otherPlayer = collisionMessage->other.getComponent<Player>();
				if ( otherPlayer.kill()	&& m_firer.hasComponent<Player>() ) {

					int score = otherPlayer.getScore();
					m_firer.getComponent<Player>().score( (score > 1 ? score - 1 : 0)  + 1);
				}
				Game::GetScene().destroyActor( &getOwner() );
			}
		}
	}
}