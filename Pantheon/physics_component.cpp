
// physics_component.cpp

#include "physics_component.hpp"

#include "static_subscriber_event.hpp"

using namespace pantheon;


///////////////////////////////////////////////////////////////////////////////
// PhysicsCollisionMessage2D
///////////////////////////////////////////////////////////////////////////////

PhysicsCollisionMessage2D::PhysicsCollisionMessage2D( ConstructPhysicsCollisionMessage2DPermit& t_permit,
	const glm::vec2& t_resolve, const glm::vec2& t_force, PhysicsComponent2D& t_other )
	: resolve{ t_resolve }, force{ t_force }, other{ t_other } {
	t_permit.use();

}

///////////////////////////////////////////////////////////////////////////////
// PhysicsComponent2D
///////////////////////////////////////////////////////////////////////////////

typedef StaticSubscriptionEvent<PhysicsComponent2D> PhysicsComponents2D;
glm::vec2 PhysicsComponent2D::g_gravity{ 0.0f, -1.0f };

PhysicsComponent2D::PhysicsComponent2D( ConstructComponentPermit& t_permit, Actor& t_owner ) : IActorComponent{ t_permit, t_owner } {

	PhysicsComponents2D::add( this );
}

PhysicsComponent2D::~PhysicsComponent2D() {

	PhysicsComponents2D::remove( this );
}

void PhysicsComponent2D::increment( float t_delta ) {

	if ( isGravityEnabled() ) {

		velocity += PhysicsComponent2D::g_gravity * t_delta;
	}
	velocity -= velocity * airResistance * t_delta;

	Transform& transform = getOwner().getTransform();
	assert( mass > 0.0f  && "mass must be more than zero" );
	float imass = 1.0f / mass;
	transform.position += glm::vec3(velocity, 0.0f) * t_delta;
	transform.rotation *= glm::angleAxis( angularVelocity * t_delta, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	
}

void PhysicsComponent2D::onCollision( const glm::vec2& t_resolve, const glm::vec2& t_force, PhysicsComponent2D& t_other  ) {

	if ( glm::length2( t_force ) >= 0.001f ) {

		glm::vec2 axis = glm::normalize( t_force );
		assert( mass > 0.0f  && "mass must be more than zero" );
		float ifriction = 1.0f - friction;
		float iMass = 1.0f / mass;
		glm::vec2 velAxis = glm::normalize( velocity );
		glm::vec2 bounceVelocity = velocity * -glm::dot( velocity, axis );
		glm::vec2 frictionVelocity = velocity + bounceVelocity;
		velocity = bounceVelocity * bounce + frictionVelocity * ifriction + t_force * iMass;
	}
	Transform& transform = getOwner().getTransform();
	transform.position += glm::vec3( t_resolve, 0.0f );
	ConstructPhysicsCollisionMessage2DPermit permit{};
	PhysicsCollisionMessage2D message{ permit, t_resolve, t_force, t_other };
	getOwner().dispatchEvent( message );
}

void PhysicsComponent2D::onEventMessage( pantheon::IActorEventMessage* const t_message ) {

	Collision2DMessage* const collisionMessage = t_message->as<Collision2DMessage>();
	if ( collisionMessage != nullptr && isSolid() ) {

		auto collision = collisionMessage->collisions[0];
		if ( collisionMessage->other.hasComponent<PhysicsComponent2D>() ) {

			auto& other = collisionMessage->other.getComponent<PhysicsComponent2D>();
			if ( !other.isActive() || !other.isSolid() ) {

				return;
			}

			if ( !isKinematic() && !other.isKinematic() ) {

				float force = glm::dot( velocity, -collision.axis );
				if ( force > 0.0f ) {

					velocity += force * collision.axis;
					force = mass * force;
				}
				else {

					force = 0.0f;
				}
				float otherForce = glm::dot( other.velocity, collision.axis );
				if ( otherForce > 0.0f ) {

					other.velocity += otherForce * collision.axis;
					otherForce = other.mass * otherForce;
				}
				else {

					otherForce = 0.0f;
				}
				otherForce = otherForce > 0.0f ? otherForce : 0.0f;
				float totalForce = force + otherForce;
				float totalMass = mass + other.mass;
				float massFactor = mass / totalMass;
				onCollision( -collision.axis * collision.separation * massFactor
					, collision.axis * massFactor * totalForce, other );
				other.onCollision( collision.axis * collision.separation * ( 1.0f - massFactor )
					, -collision.axis * ( 1.0f - massFactor ) * totalForce, *this );
			}
			else if (!isKinematic()) {

				onCollision( -collision.axis * collision.separation
					, { 0.0f, 0.0f }, other );
			}
		}
	}
}

void PhysicsComponent2D::setSolid() {

	m_flags |= PHYSICS_IS_SOLID;
}
void PhysicsComponent2D::setSoft() {

	m_flags &= ~PHYSICS_IS_SOLID;
}
void PhysicsComponent2D::setKinematic() {

	m_flags |= PHYSICS_IS_KINEMATIC;
}
void PhysicsComponent2D::setDynamic() {

	m_flags &= ~PHYSICS_IS_KINEMATIC;
}
void PhysicsComponent2D::setGravityEnabled() {

	m_flags |= PHYSICS_IS_GRAVITY_ENABLED;
}
void PhysicsComponent2D::setGravityDisabled() {

	m_flags &= ~PHYSICS_IS_GRAVITY_ENABLED;
}
bool PhysicsComponent2D::isSolid() {

	return (m_flags & PHYSICS_IS_SOLID) == PHYSICS_IS_SOLID;
}
bool PhysicsComponent2D::isKinematic() {

	return (m_flags & PHYSICS_IS_KINEMATIC) == PHYSICS_IS_KINEMATIC;
}
bool PhysicsComponent2D::isGravityEnabled() {

	return (m_flags & PHYSICS_IS_GRAVITY_ENABLED) == PHYSICS_IS_GRAVITY_ENABLED;
}

void PhysicsComponent2D::call( CallPhysics2DIncrementPermit& t_permit, float t_delta ) {

	t_permit.use();

	PhysicsComponents2D::forEach( [&t_delta]( PhysicsComponent2D* const t_component ) {
		if ( t_component->isActive() ) {

			t_component->increment( t_delta );
		}
	} );
}

void PhysicsComponent2D::setGravity( const glm::vec2& t_newGravity ) {

	PhysicsComponent2D::g_gravity = t_newGravity;
}
glm::vec2 PhysicsComponent2D::getGravity() {

	return PhysicsComponent2D::g_gravity;
}