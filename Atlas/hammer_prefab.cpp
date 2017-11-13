
// hammer_prefab.cpp

#include "hammer_prefab.hpp"
#include "health_component.hpp"
#include "game_state_prefab.hpp"

#include <physics_component.hpp>
#include <line_renderer.hpp>

#define _USE_MATH_DEFINES

#include <math.h>
#include <random>

const float PI2{ float(M_PI) * 2.0f };

using namespace pantheon;
using namespace atlas;

HammerPrefab::HammerPrefab( ConstructComponentPermit& t_permit, Actor& t_owner )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

	t_owner.createComponent<Collision2DComponent>();

	std::vector<glm::vec2> vertices
	{ { -2.0f, -1.5f }, { 2.0f, -1.5f }, { 2.0f, 1.5f }, { -2.0f, 1.5f } };
	m_hull.points.insert( m_hull.points.begin(), vertices.begin(), vertices.end() );

	auto& collision = t_owner.getComponent<Collision2DComponent>();
	collision.addCollider( &m_hull );
	int collisionFlags = 512;
	collision.setCollisionFlags( collisionFlags );
	collision.setCollideWithFlags( 0xffffffff & ~collisionFlags );
	m_timer = 0.0f;
}

HammerPrefab::~HammerPrefab() {

}

void HammerPrefab::update( float t_delta ) {

	m_timer += t_delta;
	if ( m_timer > 3.0f ) {

		m_timer = 0.0f;
		float direction = PI2 * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
		m_direction = { sin( direction ), cos( direction ) };
	}
	Transform& transform = getOwner().getTransform();

	transform.rotation *= glm::angleAxis( 4.0f * t_delta, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	transform.position += glm::vec3( m_direction, 0.0f ) * 128.0f * t_delta;

	if ( !isCheatActive( CHEAT_THOR ) ) {

		Game::GetScene().destroyActor( &getOwner() );
	}
}

void HammerPrefab::render() {

	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();

	std::vector<LineRendererVertex> vertices
		{ LineRendererVertex( -2.0f, -1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 2.0f, -1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 2.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( -2.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex::separator()
		, LineRendererVertex::separator()
		, LineRendererVertex( 0.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 0.0f, 3.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex::separator() };

	LineRendererMessage message{ vertices.begin(), vertices.end(), getOwner().getTransform2D().findMatrix() };
	renderer.queueDraw( message );
}

void HammerPrefab::onEventMessage( IActorEventMessage* const t_message ) {

	{
		Collision2DMessage* collisionMessage = t_message->as<Collision2DMessage>();
		if ( collisionMessage != nullptr ) {
			
			if ( collisionMessage->other.hasComponent<HealthComponent>() ) {

				auto& health = collisionMessage->other.getComponent<HealthComponent>();
				if ( health.isActive() ) {

					health.damage( 1 );
				}
			}
		}
	}
}


SuperHammerPrefab::SuperHammerPrefab( ConstructComponentPermit& t_permit, Actor& t_owner )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

	t_owner.createComponent<Collision2DComponent>();

	std::vector<glm::vec2> vertices
	{ { -2.0f, -1.5f }, { 2.0f, -1.5f }, { 2.0f, 1.5f }, { -2.0f, 1.5f } };
	m_hull.points.insert( m_hull.points.begin(), vertices.begin(), vertices.end() );

	auto& collision = t_owner.getComponent<Collision2DComponent>();
	collision.addCollider( &m_hull );
	int collisionFlags = 512;
	collision.setCollisionFlags( collisionFlags );
	collision.setCollideWithFlags( 0xffffffff & ~collisionFlags );
	m_timer = 0.0f;
}

SuperHammerPrefab::~SuperHammerPrefab() {

}

void SuperHammerPrefab::update( float t_delta ) {

	m_timer += t_delta;
	if ( m_timer > 3.0f ) {

		m_timer = 0.0f;
		float direction = PI2 * ( static_cast<float>( rand() ) / static_cast<float>( RAND_MAX ) );
		m_direction = { sin( direction ), cos( direction ) };
	}
	Transform& transform = getOwner().getTransform();

	transform.rotation *= glm::angleAxis( 4.0f * t_delta, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	transform.position += glm::vec3( m_direction, 0.0f ) * 128.0f * t_delta;

	if ( !isCheatActive( CHEAT_DISNEY ) ) {

		Game::GetScene().destroyActor( &getOwner() );
	}
}

void SuperHammerPrefab::render() {

	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();

	std::vector<LineRendererVertex> vertices
	{ LineRendererVertex( -2.0f, -1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 2.0f, -1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 2.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( -2.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex::separator()
		, LineRendererVertex::separator()
		, LineRendererVertex( 0.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 0.0f, 3.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex::separator() };

	LineRendererMessage message{ vertices.begin(), vertices.end(), getOwner().getTransform2D().findMatrix() };
	renderer.queueDraw( message );
}

void SuperHammerPrefab::onEventMessage( IActorEventMessage* const t_message ) {

	{
		Collision2DMessage* collisionMessage = t_message->as<Collision2DMessage>();
		if ( collisionMessage != nullptr ) {

			if ( collisionMessage->other.hasComponent<HealthComponent>() ) {

				auto& health = collisionMessage->other.getComponent<HealthComponent>();
				if ( health.isActive() ) {

					health.damage( 1 );
					getOwner().getTransform().scale += glm::vec3( 1.0f, 1.0f, 1.0f ) * 0.33f;
				}
			}
		}
	}
}

ClockHammerPrefab::ClockHammerPrefab( ConstructComponentPermit& t_permit, Actor& t_owner )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

	t_owner.createComponent<Collision2DComponent>();

	std::vector<glm::vec2> vertices
	{ { -2.0f, -1.5f }, { 2.0f, -1.5f }, { 2.0f, 1.5f }, { -2.0f, 1.5f } };
	m_hull.points.insert( m_hull.points.begin(), vertices.begin(), vertices.end() );

	auto& collision = t_owner.getComponent<Collision2DComponent>();
	collision.addCollider( &m_hull );
	int collisionFlags = 512;
	collision.setCollisionFlags( collisionFlags );
	collision.setCollideWithFlags( 0xffffffff & ~collisionFlags );
	m_timer = 0.0f;
}

ClockHammerPrefab::~ClockHammerPrefab() {

}

void ClockHammerPrefab::update( float t_delta ) {

	m_timer += t_delta;
	if ( m_timer > 3.0f ) {

		m_timer = 0.0f;
		float direction = PI2 * ( static_cast<float>( rand() ) / static_cast<float>( RAND_MAX ) );
		m_direction = { sin( direction ), cos( direction ) };
		getOwner().getTransform().scale += glm::vec3( 1.0f, 1.0f, 1.0f ) * 0.33f;
	}
	Transform& transform = getOwner().getTransform();

	transform.rotation *= glm::angleAxis( 4.0f * t_delta, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	transform.position += glm::vec3( m_direction, 0.0f ) * 128.0f * t_delta;

	if ( !isCheatActive( CHEAT_BEWORTHY ) ) {

		Game::GetScene().destroyActor( &getOwner() );
	}
}

void ClockHammerPrefab::render() {

	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();

	std::vector<LineRendererVertex> vertices
	{ LineRendererVertex( -2.0f, -1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 2.0f, -1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 2.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( -2.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex::separator()
		, LineRendererVertex::separator()
		, LineRendererVertex( 0.0f, 1.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 0.0f, 3.5f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex::separator() };

	LineRendererMessage message{ vertices.begin(), vertices.end(), getOwner().getTransform2D().findMatrix() };
	renderer.queueDraw( message );
}

void ClockHammerPrefab::onEventMessage( IActorEventMessage* const t_message ) {

	{
		Collision2DMessage* collisionMessage = t_message->as<Collision2DMessage>();
		if ( collisionMessage != nullptr ) {

			if ( collisionMessage->other.hasComponent<HealthComponent>() ) {

				auto& health = collisionMessage->other.getComponent<HealthComponent>();
				if ( health.isActive() ) {

					health.damage( 1 );
				}
			}
		}
	}
}