
// boss_prefab.cpp

#include "boss_prefab.hpp"
#include "bullet.hpp"
#include "health_component.hpp"
#include "line_text_helpers.hpp"
#include "game_state_prefab.hpp"

#include <physics_component.hpp>
#include <line_renderer.hpp>

using namespace pantheon;
using namespace atlas;

BossPrefab::BossPrefab( ConstructComponentPermit& t_permit, Actor& t_owner )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

	t_owner.createComponent<Collision2DComponent>();
	t_owner.createComponent<PhysicsComponent2D>();
	t_owner.createComponent<HealthComponent>();

	std::vector<glm::vec2> vertices
		{ { 5.0f, 0.0f }, { 0.0f, 5.0f }, { -5.0f, 0.0f }, { 0.0f, -5.0f } };
	m_hull.points.insert( m_hull.points.begin(), vertices.begin(), vertices.end() );

	auto& collision = t_owner.getComponent<Collision2DComponent>();
	collision.addCollider( &m_hull );
	int collisionFlags = 16;
	collision.setCollisionFlags(collisionFlags);
	collision.setCollideWithFlags( 0xffffffff & ~collisionFlags );
	auto& physics = t_owner.getComponent<PhysicsComponent2D>();
	physics.setGravityDisabled();
	physics.setDynamic();
	physics.setSolid();
	physics.airResistance = 1.0f;
	auto& health = t_owner.getComponent<HealthComponent>();
	health.health = 20;
	getOwner().getTransform().position = { 0.0f, 0.0f, 0.0f };
	getOwner().getTransform().scale = glm::vec3( 1.0f, 1.0f, 1.0f )
		* (1.0f + health.health * 0.33f);

	Audio& audio = pantheon::Game::GetAudio();
	m_shootSource = audio.createSource( "shoot" );
	m_shootSource->setGain( 2.0f );
}

BossPrefab::~BossPrefab() {

}

void BossPrefab::update( float t_delta ) {

	m_timer += t_delta;
	if ( m_timer > 0.66f ) {

		m_timer = 0.0f;
		shoot();
	}
	getOwner().getTransform().rotation *= glm::angleAxis( 2.0f * t_delta
		, glm::vec3( 0.0f, 0.0f, 1.0f ) );

	if ( !isCheatActive( CHEAT_BOSS ) ) {

		Game::GetScene().destroyActor( &getOwner() );
	}
}

void BossPrefab::render() {

	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();

	std::vector<LineRendererVertex> vertices
		{ LineRendererVertex( 5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( -5.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex( 0.0f, -5.0f, 0.0f, 1.0f, 1.0f, 1.0f )
		, LineRendererVertex::separator()
		, LineRendererVertex::separator() };

	LineRendererMessage message{ vertices.begin(), vertices.end(), getOwner().getTransform2D().findMatrix() };
	renderer.queueDraw( message );

	std::string scoreString = std::to_string( getOwner().getComponent<HealthComponent>().health );
	LineRendererMessage messageB = stringToLines( scoreString
		, getOwner().getTransform().position + glm::vec3( scoreString.size() * -2.5f, 2.5f, 0.0f ), { 1.0f, 1.0f, 1.0f } );
	renderer.queueDraw( messageB );
}

void BossPrefab::shoot() {

	Actor& owner = getOwner();
	Transform2D transform = owner.getTransform();

	Game::GetScene().createPrefab<Bullet>( BulletInfo( owner
		, transform.position + transform.findUp() * transform.scale * 5.0f
		, transform.findUp() * 160.0f, { 1.0f, 1.0f, 1.0f }, 16 ) );
	Game::GetScene().createPrefab<Bullet>( BulletInfo( owner
		, transform.position + transform.findDown() * transform.scale * 5.0f
		, transform.findDown() * 160.0f, { 1.0f, 1.0f, 1.0f }, 16 ) );
	Game::GetScene().createPrefab<Bullet>( BulletInfo( owner
		, transform.position + transform.findRight() * transform.scale * 5.0f
		, transform.findRight() * 160.0f, { 1.0f, 1.0f, 1.0f }, 16 ) );
	Game::GetScene().createPrefab<Bullet>( BulletInfo( owner
		, transform.position + transform.findLeft() * transform.scale * 5.0f
		, transform.findLeft() * 160.0f, { 1.0f, 1.0f, 1.0f }, 16 ) );
	m_shootSource->play();
	shake( 0.5f );
}

void BossPrefab::onEventMessage( IActorEventMessage* const t_message ) {

	{
		PhysicsCollisionMessage2D* physMessage = t_message->as<PhysicsCollisionMessage2D>();
		if ( physMessage != nullptr ) {

			physMessage->other.velocity += 32.0f * glm::normalize(
				physMessage->other.getOwner().getTransform2D().position - getOwner().getTransform2D().position );
			shake( 1.0f );
		}
	}
	{
		HealMessage* healMessage = t_message->as<HealMessage>();
		if ( healMessage != nullptr ) {

			auto& healthComponent = getOwner().getComponent<HealthComponent>();
			healthComponent.health += 1;
			getOwner().getTransform().scale = glm::vec3( 1.0f, 1.0f, 1.0f )
				* (1.0f + healthComponent.health * 0.33f);
		}
	}
	{
		DamageMessage* damageMessage = t_message->as<DamageMessage>();
		if ( damageMessage != nullptr ) {

			auto& healthComponent = getOwner().getComponent<HealthComponent>();
			healthComponent.health = healthComponent.health < 0 ? 0 : healthComponent.health;
			getOwner().getTransform().scale = glm::vec3( 1.0f, 1.0f, 1.0f )
				* (1.0f + healthComponent.health * 0.33f);
			if ( healthComponent.health <= 0 ) {

				Game::GetScene().destroyActor( &getOwner() );
			}
		}
	}
}