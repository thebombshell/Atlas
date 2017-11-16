
// player_prefab.cpp

#include "player_prefab.hpp"
#include "blocks.hpp"

using namespace gaea;
using namespace pantheon;

bool g_hadJumped{ false };
float g_topHeight{ 0.0f };

PlayerPrefab::PlayerPrefab( ConstructComponentPermit& t_permit, Actor& t_owner )
	: IActorComponent{ t_permit, t_owner }, m_circle{ t_owner.getTransform(), 0.5f } {

	t_owner.createComponent<Collision2DComponent>();
	t_owner.createComponent<PhysicsComponent2D>();
	auto& collision = t_owner.getComponent<Collision2DComponent>();
	collision.setCollideWithFlags( 0xffff & ~(1) );
	collision.setCollisionFlags( 1 );
	collision.addCollider( &m_circle );
	auto& physics = t_owner.getComponent<PhysicsComponent2D>();
	physics.setDynamic();
	physics.setSolid();
	physics.setGravityEnabled();
	physics.bounce = 0.0f;
	physics.friction = 0.0f;
	physics.airResistance = 0.0f;
	physics.mass = 1.0f;
}
PlayerPrefab::~PlayerPrefab() {

}

void PlayerPrefab::update( float t_delta ) {

	auto& transform = getOwner().getTransform();

	// handle ground
	updateGround();

	// handle input
	updateInput( t_delta );

	// handle animation

	m_animTimer += t_delta;
	if ( m_animTimer > 0.48f ) {

		m_animTimer -= 0.48f;
	}
}

void PlayerPrefab::updateGround() {

	auto& transform = getOwner().getTransform();

	// get copy of transform and use it to make an offset collider

	Transform tempTransform = transform;
	tempTransform.position[1] -= 0.16f;
	Circle temp{ tempTransform, 0.4f };

	// use offset collider to find possible ground

	std::vector<CollisionGroup2D> results =
		Game::GetCollisionManagerAs<Collision2DManager>()->query( &temp );

	// check if is on ground

	m_isOnGround = checkGroundViability( results );

	// check if is still stuck to floor

	if ( m_stuckTo != nullptr ) {

		m_stuckTo = checkStickViability( results ) ? m_stuckTo : nullptr;
	}

	// handle moving platforms

	if ( m_stuckTo != nullptr ) {

		glm::vec2 difference = glm::vec2( m_stuckTo->position ) - m_stickPosition;
		m_stickPosition = m_stuckTo->position;
		transform.position += glm::vec3( difference, 0.0f );
	}
}
void PlayerPrefab::updateInput( float t_delta ) {

	auto& input = Game::GetInput();
	auto& physics = getOwner().getComponent<PhysicsComponent2D>();

	// handle movement input

	if ( input.isKeyDown( "Left" ) ) {

		m_state &= ~PLAYER_STATE_IS_LOOKING_RIGHT;
		m_state |= PLAYER_STATE_IS_RUNNING;
		if ( physics.velocity[0] > -m_topSpeed ) {

			if ( physics.velocity[0] > 0.0f ) {

				physics.velocity[0] -= m_decceleration * t_delta;
				physics.velocity[0] = physics.velocity[0] < 0.0f ? 0.0f : physics.velocity[0];
			}
			physics.velocity[0] -= m_acceleration * t_delta;
			physics.velocity[0] = physics.velocity[0] < -m_topSpeed
				? -m_topSpeed : physics.velocity[0];
		}
	}

	if ( input.isKeyDown( "Right" ) ) {

		m_state |= PLAYER_STATE_IS_LOOKING_RIGHT;
		m_state |= PLAYER_STATE_IS_RUNNING;
		if ( physics.velocity[0] < m_topSpeed ) {

			if ( physics.velocity[0] < 0.0f ) {

				physics.velocity[0] += m_decceleration * t_delta;
				physics.velocity[0] = physics.velocity[0] > 0.0f ? 0.0f : physics.velocity[0];
			}
			physics.velocity[0] += m_acceleration * t_delta;
			physics.velocity[0] = physics.velocity[0] > m_topSpeed
				? m_topSpeed : physics.velocity[0];
		}
	}

	if ( !input.isKeyDown( "Left" ) && !input.isKeyDown( "Right" ) ) {

		m_state &= ~PLAYER_STATE_IS_RUNNING;
		m_animTimer = 0.0f;
		if ( physics.velocity[0] > 0.0f ) {

			physics.velocity[0] -= m_decceleration * t_delta;
			physics.velocity[0] = physics.velocity[0] < 0.0f ? 0.0f : physics.velocity[0];
		}
		else if ( physics.velocity[0] < 0.0f ) {

			physics.velocity[0] += m_decceleration * t_delta;
			physics.velocity[0] = physics.velocity[0] > 0.0f ? 0.0f : physics.velocity[0];
		}
	}

	// handle jump

	if ( input.isKeyDown( "Space" ) && m_isOnGround ) {

		physics.velocity[1] = m_jumpingForce;
		m_state |= PLAYER_STATE_IS_JUMPING;
		m_isOnGround = false;
		g_hadJumped = true;
	}
	if ( g_hadJumped ) {

		float temp = getOwner().getTransform().position[1];
		g_topHeight = temp > g_topHeight ? temp : g_topHeight;
		if ( m_isOnGround ) {

			printf( (std::to_string( g_topHeight ) + "\n").c_str() );
			g_hadJumped = false;
			g_topHeight = 0.0f;
		}
	}
	if ( physics.velocity[1] < 0.0f ) {

		m_state &= ~PLAYER_STATE_IS_JUMPING;
	}
}

bool PlayerPrefab::checkGroundViability(const std::vector<CollisionGroup2D>& t_possibleGround) {

	// ensure possible ground is viable ground

	for ( auto& group : t_possibleGround ) {

		if ( &group.other != &getOwner()
			&& group.other.hasComponent<PhysicsComponent2D>() 
			&& !group.other.hasComponent<HazardPrefab>() ) {

			auto& otherPhysics = group.other.getComponent<PhysicsComponent2D>();
			if ( otherPhysics.isSolid() && otherPhysics.isActive() ) {

				// ground is viable

				return true;
			}
		}
	}

	// ground is not viable

	return false;
}

bool PlayerPrefab::checkStickViability( const std::vector<CollisionGroup2D>& t_possibleGround ) {

	// ensure sticky ground is near

	for ( auto& group : t_possibleGround ) {

		if ( &group.other.getTransform() == m_stuckTo ) {

			// still stuck to ground

			return true;
		}
	}

	// no longer stuck to ground

	return false;
}

void PlayerPrefab::render() {

	SpriteRenderer* const renderer = Game::GetRendererAs<SpriteRenderer>();
	Transform2D transform = getOwner().getTransform2D();
	Transform2D transformb{ { 0.0f, 4.0f } };

	glm::vec2 source{ 0.0f, 0.0f };
	if ( m_state & PLAYER_STATE_IS_JUMPING ) {

		source = { 48, 0 };
	}
	else if ( !m_isOnGround ) {

		source = { 64, 0 };
	}
	else if ( m_state & PLAYER_STATE_IS_RUNNING ) {

		if ( m_animTimer < 0.24f ) {

			source = { 16, 0 };
		}
		else {

			source = { 32, 0 };
		}
	}
	if ( m_state & PLAYER_STATE_IS_LOOKING_RIGHT ) {

		transform.scale[0] = 1.0f;
	}
	else {

		transform.scale[0] = -1.0f;
	}
	SpriteRendererMessage messageA{ "sprites", { 0.5f, 0.5f }, source
		, { 16, 16 }, { 1.0f, 1.0f, 1.0f }, 0.5f, transform.findMatrix() };
	SpriteRendererMessage messageB{ "sprites", { 0.5f, 0.5f }, source
		, { 16, 16 }, { 1.0f, 1.0f, 1.0f }, 0.5f, transformb.findMatrix() };
	renderer->queueDraw( messageA );
	renderer->queueDraw( messageB );
}

void PlayerPrefab::onEventMessage( IActorEventMessage* const t_message ) {

	Collision2DMessage* const collisionMessage = t_message->as<Collision2DMessage>();
	if ( collisionMessage != nullptr ) {

		if ( collisionMessage->other.hasComponent<HazardPrefab>() ) {

			kill();
		}
	}
	PhysicsCollisionMessage2D* const physicsMessage = t_message->as<PhysicsCollisionMessage2D>();
	if ( physicsMessage != nullptr ) {

		if ( glm::dot( glm::normalize( physicsMessage->resolve ), { 0.0f, 1.0f } ) > 0.5f ) {

			if ( physicsMessage->other.getOwner().hasComponent<MovingBlockPrefab>() ) {

				if ( m_stuckTo == nullptr ) {

					m_stuckTo = &physicsMessage->other.getOwner().getTransform();
					m_stickPosition = m_stuckTo->position;
				}

			}
			m_isOnGround = true;
		}
	}
}

void PlayerPrefab::kill() {


	auto& physics = getOwner().getComponent<PhysicsComponent2D>();
	physics.velocity = { 0.0f, 0.0f };
	auto& transform = getOwner().getTransform();
	transform.position = { 0.0f, 2.0f, 0.0f };
}