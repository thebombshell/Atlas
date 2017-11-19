
// blocks.cpp

#include "blocks.hpp"

using namespace gaea;
using namespace pantheon;

///////////////////////////////////////////////////////////////////////////////
// BlockPrefab
///////////////////////////////////////////////////////////////////////////////

BlockPrefab::BlockPrefab( ConstructComponentPermit& t_permit, Actor& t_owner
	, const glm::vec2& t_scale )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

	t_owner.getTransform().scale = { t_scale, 1.0f };
	setupComponents();
	m_autoSetupMessages = false;
}

void BlockPrefab::setupComponents() {

	Actor& owner = getOwner();

	owner.createComponent<Collision2DComponent>();
	owner.createComponent<PhysicsComponent2D>();

	// set up collisions

	auto& collision = owner.getComponent<Collision2DComponent>();
	collision.setCollideWithFlags( 1 );
	collision.setCollisionFlags( 2 );
	glm::vec2 points[]{
		{ 0.0f, 0.0f },
		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },
		{ 0.0f, 1.0f }
	};
	m_hull.points.insert( m_hull.points.end(), points, points + 4 );
	collision.addCollider( &m_hull );

	// set up physics

	auto& physics = owner.getComponent<PhysicsComponent2D>();
	physics.setKinematic();
	physics.setSolid();
	physics.bounce = 0.5f;
	physics.friction = 0.02f;
}

void BlockPrefab::setupMessages() {

	Transform& transform = getOwner().getTransform();

	m_messages.clear();
	for ( int x = 0; x < transform.scale[0]; ++x ) {

		int edgeType = 2;
		if ( x == 0 && (x + 1) < transform.scale[0] ) {

			edgeType = 0;
		}
		else if ( (x + 1) < transform.scale[0] ) {

			edgeType = 1;
		}
		else if ( x == 0 ) {

			edgeType = 3;
		}

		for ( int y = 0; y < transform.scale[1]; ++y ) {

			glm::vec2 source = { 80.0f, 16.0f };
			switch ( y ) {

				case 0:
					source = { static_cast<float>(edgeType) * 16.0f, 16.0f };
					break;
				case 1:
					source = { 64.0f, 16.0f };
					break;
			}
			m_messages.push_back( { "sprites", -(glm::vec2( x, y ) + glm::vec2( transform.position )), source, glm::vec2( 16.0f, 16.0f )
				, { 1.0f, 1.0f, 1.0f }, 0.5f, { } } );
		}
	}
}

BlockPrefab::~BlockPrefab() {

}

void BlockPrefab::render() {

	SpriteRenderer* const renderer = Game::GetRendererAs<SpriteRenderer>();

	if ( m_autoSetupMessages ) {

		setupMessages();
	}

	for ( auto& message : m_messages ) {

		renderer->queueDraw( message );
	}

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
	: IActorComponent{ t_permit, t_owner }, start{ t_message.start }, end{ t_message.end } {

	t_owner.createComponent<BlockPrefab>( t_message.scale );
	t_owner.getComponent<BlockPrefab>().m_autoSetupMessages = true;

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
	physics.velocity = { 0.0f, 0.0f };
	auto& transform = getOwner().getTransform();
	float alpha = cos( getTimeAlpha() ) * 0.5f + 0.5f;
	transform.position = glm::vec3( start * alpha + end * (1.0f - alpha), 0.0f );
}

///////////////////////////////////////////////////////////////////////////////
// HazardPrefab
///////////////////////////////////////////////////////////////////////////////

HazardPrefab::HazardPrefab( ConstructComponentPermit& t_permit, Actor& t_owner
	, const glm::vec2& t_scale )
	: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

	t_owner.createComponent<Collision2DComponent>();
	auto& collision = t_owner.getComponent<Collision2DComponent>();
	collision.setCollideWithFlags( 1 );
	collision.setCollisionFlags( 2 );
	glm::vec2 points[]{
		{ -0.5f, -0.5f },
		{ 0.5f, -0.5f },
		{ 0.5f, 0.5f },
		{ -0.5f, 0.5f }
	};
	m_hull.points.insert( m_hull.points.end(), points, points + 4 );
	collision.addCollider( &m_hull );
	Transform& transform = t_owner.getTransform();
	transform.scale = { t_scale, 1.0f };
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