
// bullet.cpp

// source file for bullet class

#include "bullet.hpp"

#include "player.hpp"
#include "health_component.hpp"
#include "game_state_prefab.hpp"

#include <collision_2d.hpp>
#include <line_renderer.hpp>

using namespace atlas;
using namespace pantheon;

Bullet::Bullet( ConstructComponentPermit& t_permit, pantheon::Actor& t_owner, const BulletInfo& t_info )
	: IActorComponent( t_permit, t_owner ), m_firer{ nullptr }, m_collider{ t_owner.getTransform(), 1.0f } {

	getOwner().getTransform().position = glm::vec3( t_info.position, 0.5f );
	m_flag = t_info.flag;
	m_timer = 0.0f;
	m_firer = &t_info.firer;
	m_velocity = t_info.velocity;
	m_colour = t_info.colour;
}

Bullet::~Bullet() {

}

void Bullet::update( float t_delta ) {

	if ( !getOwner().hasComponent<Collision2DComponent>() && m_timer > 0.016f ) {

		getOwner().createComponent<Collision2DComponent>();
		Collision2DComponent& collision = getOwner().getComponent<Collision2DComponent>();
		collision.addCollider( &m_collider );
		collision.setCollisionFlags( 512 );
		collision.setCollideWithFlags( 511 & ~m_flag );
	}
	m_timer += t_delta;
	if ( m_timer > 1.0f ) {

		destroy();
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
		{ { transform.position[0], transform.position[1], 0.5f, m_colour[0], m_colour[1], m_colour[2]}
		, { transform.position[0] - m_velocity[0] * 0.032f, transform.position[1] - m_velocity[1] * 0.032f, 0.5f, m_colour[0] * 0.5f, m_colour[1] * 0.5f, m_colour[2] * 0.5f } };
	LineRendererMessage message{ vertices, vertices + 2 };
	message.separate();
	renderer->queueDraw( message );
}

void Bullet::onEventMessage( IActorEventMessage* const t_message ) {

	{
		auto collisionMessage = t_message->as<Collision2DMessage>();
		if ( collisionMessage != nullptr ) {

			if ( &collisionMessage->other != m_firer
				&& collisionMessage->other.hasComponent<HealthComponent>() ) {

				HealthComponent& otherHealth = collisionMessage->other.getComponent<HealthComponent>();

				if ( otherHealth.isActive() ) {

					// handle juggernaught cheat

					if ( isCheatActive( CHEAT_JUGGERNAUGHT )
						&& m_firer->hasComponent<Player>()
						&& collisionMessage->other.hasComponent<Player>() ) {

						Player& player = m_firer->getComponent<Player>();
						Player& otherPlayer = collisionMessage->other.getComponent<Player>();

						if ( otherPlayer.isJuggernaught() ) {

							player.makeJuggernaught();
							otherPlayer.takeJuggernaught();
						}

					}

					otherHealth.damage( 1 );
					if ( m_firer->hasComponent<HealthComponent>() ) {

						m_firer->getComponent<HealthComponent>().heal( 1 );
					}
				}
				destroy();
			}
		}
	}
}

void Bullet::destroy() {

	Game::GetScene().destroyActor( &getOwner() );
}