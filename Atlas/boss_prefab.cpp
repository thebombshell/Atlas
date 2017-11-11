
// boss_prefab.cpp

#include "boss_prefab.hpp"
#include "bullet.hpp"
#include "health_component.hpp"

#include <physics_component.hpp>
#include <line_renderer.hpp>

using namespace pantheon;
using namespace atlas;

BossPrefab::BossPrefab( ConstructComponentPermit& t_permit, Actor& t_owner )
	: IActorComponent{ t_permit, t_owner } {

	t_owner.createComponent<Collision2DComponent>();
	t_owner.createComponent<PhysicsComponent2D>();
	t_owner.createComponent<HealthComponent>();

	auto& collision = t_owner.getComponent<Collision2DComponent>();
	auto& physics = t_owner.getComponent<PhysicsComponent2D>();
	auto& health = t_owner.getComponent<HealthComponent>();
	health.health = 20;
}

BossPrefab::~BossPrefab() {

}

void BossPrefab::update( float t_delta ) {

	m_timer += t_delta;
	if ( m_timer > 0.33f ) {

		m_timer -= 0.33f;
	}
	getOwner().getTransform().rotation *= glm::angleAxis( 20.0f * t_delta
		, glm::vec3( 0.0f, 0.0f, 1.0f ) );
}

void BossPrefab::render() {

	LineRenderer& renderer = *Game::GetRendererAs<LineRenderer>();

	std::vector<LineRendererVertex> vertices;

	LineRendererMessage message{ vertices.begin(), vertices.end() };
	renderer.queueDraw( message );
}

void BossPrefab::shoot() {

	Actor& owner = getOwner();
	Transform2D transform = owner.getTransform();

	pantheon::Game::GetScene().createPrefab<Bullet>(
		BulletInfo( owner
			, transform.position + transform.findUp() * transform.scale * 5.0f
			, transform.findUp() * 160.0f, { 1.0f, 1.0f, 1.0f }, 0 ) );
	pantheon::Game::GetScene().createPrefab<Bullet>(
		BulletInfo( owner
			, transform.position + transform.findDown() * transform.scale * 5.0f
			, transform.findDown() * 160.0f, { 1.0f, 1.0f, 1.0f }, 0 ) );
	pantheon::Game::GetScene().createPrefab<Bullet>(
		BulletInfo( owner
			, transform.position + transform.findRight() * transform.scale * 5.0f
			, transform.findRight() * 160.0f, { 1.0f, 1.0f, 1.0f }, 0 ) );
	pantheon::Game::GetScene().createPrefab<Bullet>(
		BulletInfo( owner
			, transform.position + transform.findLeft() * transform.scale * 5.0f
			, transform.findLeft() * 160.0f, { 1.0f, 1.0f, 1.0f }, 0 ) );
}