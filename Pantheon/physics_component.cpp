
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
	transform.position += glm::vec3(velocity, 0.0f) * t_delta;
	transform.rotation *= glm::angleAxis( angularVelocity * t_delta, glm::vec3( 0.0f, 0.0f, 1.0f ) );
	m_velocitySnapshot = velocity;
	
}

void PhysicsComponent2D::onCollisionWithKinematic( const glm::vec2& t_resolve, const glm::vec2& t_axis, PhysicsComponent2D& t_other ) {

	assert( mass > 0.0f  && "mass must be more than zero" );

	float iFriction = 1.0f - friction;
	glm::vec2 bounceVelocity = t_axis * glm::dot( velocity, t_axis );
	glm::vec2 frictionVelocity = velocity - t_axis * glm::dot( velocity, t_axis );
	velocity = bounceVelocity * bounce + frictionVelocity * iFriction;

	Transform& transform = getOwner().getTransform();
	transform.position += glm::vec3( t_resolve, 0.0f );
	ConstructPhysicsCollisionMessage2DPermit permit{ };
	PhysicsCollisionMessage2D message{ permit, t_resolve, -bounceVelocity * mass, t_other };
	getOwner().dispatchEvent( message );
}


void PhysicsComponent2D::onCollisionWithDynamic( const glm::vec2& t_resolve, PhysicsComponent2D& t_other ) {

	assert( mass > 0.0f  && "mass must be more than zero" );

	glm::vec2 axis = glm::normalize( t_resolve );
	velocity = (m_velocitySnapshot * (mass - t_other.mass) + (2.0f * t_other.mass * t_other.m_velocitySnapshot)) / (mass + t_other.mass);
	
	Transform& transform = getOwner().getTransform();
	transform.position += glm::vec3( t_resolve, 0.0f );
	ConstructPhysicsCollisionMessage2DPermit permit{ };
	PhysicsCollisionMessage2D message{ permit, t_resolve, velocity - m_velocitySnapshot, t_other };
	getOwner().dispatchEvent( message );
}

void PhysicsComponent2D::processCollisions( const std::vector<CollisionGroup2D>& t_collisions ) {

	if ( !isSolid() || isKinematic() ) {

		return;
	}

	for ( auto& group : t_collisions ) {

		float minimum{ std::numeric_limits<float>::max() };
		Actor* minActor{ nullptr };
		const Collision2D* minCollision{ nullptr };
		glm::vec2 velocityNormal = glm::normalize( -m_velocitySnapshot );
		glm::vec2 step = m_velocitySnapshot * getTimeDelta();
		for ( auto& collision : group.collisions ) {

			float dot = glm::dot( collision.axis * collision.separation, step );
			if ( dot < minimum ) {

				minimum = dot;
				minActor = &group.other;
				minCollision = &collision;
			}
		}

		if ( minActor != nullptr && minActor->hasComponent<PhysicsComponent2D>()) {

			PhysicsComponent2D& other = minActor->getComponent<PhysicsComponent2D>();

			if ( other.isKinematic() ) {

				glm::vec2 resolve = -minCollision->axis * minCollision->separation;
				onCollisionWithKinematic( resolve, minCollision->axis, other );
			}
			else {

				glm::vec2 resolve = -minCollision->axis * minCollision->separation * 0.5f;
				onCollisionWithDynamic( resolve, other );
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