
// blocks.cpp

#include "blocks.hpp"

using namespace gaea;
using namespace pantheon;

///////////////////////////////////////////////////////////////////////////////
// BlockMessage
///////////////////////////////////////////////////////////////////////////////

BlockMessage::BlockMessage( const glm::vec2& t_position, const glm::vec2& t_scale )
	: position{ t_position }, scale{ t_scale } {

}

///////////////////////////////////////////////////////////////////////////////
// BlockPrefab
///////////////////////////////////////////////////////////////////////////////

BlockPrefab::BlockPrefab( ConstructComponentPermit& t_permit, Actor& t_owner
	, const BlockMessage& t_message )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

	t_owner.createComponent<Collision2DComponent>();
	t_owner.createComponent<PhysicsComponent2D>();
	auto& collisions = t_owner.getComponent<Collision2DComponent>();
	glm::vec2 points[]{
		{ -0.5f, -0.5f },
		{ 0.5f, -0.5f },
		{ 0.5f, 0.5f },
		{ -0.5f, 0.5f }
	};
	m_hull.points.insert( m_hull.points.end(), points, points + 4 );
	collisions.addCollider( &m_hull );
	auto& physics = t_owner.getComponent<PhysicsComponent2D>();
	physics.setKinematic();
	physics.setSolid();
	physics.bounce = 0.5f;
	physics.friction = 0.02f;

	Transform& transform = t_owner.getTransform();
	transform.position = { t_message.position, 0.0f };
	transform.scale = { t_message.scale, 1.0f };
}

BlockPrefab::~BlockPrefab() {

}

void BlockPrefab::render() {

	SpriteRenderer* const renderer = Game::GetRendererAs<SpriteRenderer>();
	Transform2D transform = getOwner().getTransform2D();
	SpriteRendererMessage messageA{ "sprites", { 0.5f, 0.5f }, { 0, 224 }
		, glm::vec2( 16, 16 ) * transform.scale, { 1.0f, 1.0f, 1.0f }, 0.5f
		, transform.findMatrix() };
	renderer->queueDraw( messageA );
}

///////////////////////////////////////////////////////////////////////////////
// MovingBlockMessage
///////////////////////////////////////////////////////////////////////////////

MovingBlockMessage::MovingBlockMessage( const glm::vec2& t_start
	, const glm::vec2& t_end, const glm::vec2& t_scale )
	: start{ t_start }, end{ t_end }, scale{ t_scale } {

}

//////////////////////////////////////////////////////////////////////////////
// MovingBlockPrefab
///////////////////////////////////////////////////////////////////////////////

MovingBlockPrefab::MovingBlockPrefab( ConstructComponentPermit& t_permit
	, Actor& t_owner, const MovingBlockMessage& t_message )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() }
	, start{ t_message.start }, end{ t_message.end } {

	t_owner.createComponent<Collision2DComponent>();
	t_owner.createComponent<PhysicsComponent2D>();
	auto& collisions = t_owner.getComponent<Collision2DComponent>();
	glm::vec2 points[]{
		{ -0.5f, -0.5f },
		{ 0.5f, -0.5f },
		{ 0.5f, 0.5f },
		{ -0.5f, 0.5f }
	};
	m_hull.points.insert( m_hull.points.end(), points, points + 4 );
	collisions.addCollider( &m_hull );
	auto& physics = t_owner.getComponent<PhysicsComponent2D>();
	physics.setKinematic();
	physics.setSolid();
	physics.bounce = 0.0f;
	physics.friction = 0.0f;
	Transform& transform = t_owner.getTransform();
	transform.position = { start, 0.0f };
	transform.scale = { t_message.scale, 1.0f };
}

MovingBlockPrefab::~MovingBlockPrefab() {

}

void MovingBlockPrefab::update( float t_delta ) {

	auto& physics = getOwner().getComponent<PhysicsComponent2D>();
	if ( isMovingToEnd ) {

		physics.velocity = end - start;
		if ( glm::dot( getOwner().getTransform2D().position, glm::normalize( physics.velocity ) )
			>= glm::dot( end, glm::normalize( physics.velocity ) ) ) {
			isMovingToEnd = false;
		}
	}
	else {

		physics.velocity = start - end;
		if ( glm::dot( getOwner().getTransform2D().position, glm::normalize( physics.velocity ) )
			>= glm::dot( start, glm::normalize( physics.velocity ) ) ) {
			isMovingToEnd = true;
		}
	}
}

void MovingBlockPrefab::render() {

	SpriteRenderer* const renderer = Game::GetRendererAs<SpriteRenderer>();
	Transform2D transform = getOwner().getTransform2D();
	SpriteRendererMessage messageA{ "sprites", { 0.5f, 0.5f }, { 0, 224 }
		, glm::vec2( 16, 16 ) * transform.scale, { 1.0f, 1.0f, 1.0f }, 0.5f
		, transform.findMatrix() };
	renderer->queueDraw( messageA );
}

///////////////////////////////////////////////////////////////////////////////
// HazardPrefab
///////////////////////////////////////////////////////////////////////////////

HazardPrefab::HazardPrefab( ConstructComponentPermit& t_permit, Actor& t_owner
	, const BlockMessage& t_message )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

	t_owner.createComponent<Collision2DComponent>();
	auto& collisions = t_owner.getComponent<Collision2DComponent>();
	glm::vec2 points[]{
		{ -0.5f, -0.5f },
		{ 0.5f, -0.5f },
		{ 0.5f, 0.5f },
		{ -0.5f, 0.5f }
	};
	m_hull.points.insert( m_hull.points.end(), points, points + 4 );
	collisions.addCollider( &m_hull );
	Transform& transform = t_owner.getTransform();
	transform.position = { t_message.position, 0.0f };
	transform.scale = { t_message.scale, 1.0f };
}

HazardPrefab::~HazardPrefab() {

}

void HazardPrefab::render() {

	SpriteRenderer* const renderer = Game::GetRendererAs<SpriteRenderer>();
	Transform2D transform = getOwner().getTransform2D();
	SpriteRendererMessage messageA{ "sprites", { 0.5f, 0.5f }, { 0, 208 }
		, glm::vec2( 16, 16 ) * transform.scale, { 1.0f, 1.0f, 1.0f }, 0.5f
		, transform.findMatrix() };
	renderer->queueDraw( messageA );
}