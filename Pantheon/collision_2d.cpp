
// collision_2d.cpp

// source file for 2d collision manager

#include "collision_2d.hpp"

#include "physics_component.hpp"
#include "spatial_hash_map.hpp"

#include <algorithm>

using namespace pantheon;

Collision2DComponent::Collision2DComponent
	( ConstructComponentPermit& t_permit, Actor& t_owner )
	: IActorComponent( t_permit, t_owner ) {

	Collision2DManager* const collision
		= dynamic_cast<Collision2DManager*>(&Game::GetCollisionManager());
	assert( collision != nullptr
		&& "Collision2DComponent requires a Collision2D manager." );
	Collision2DRegistryPermit registryPermit;
	collision->registerComponent( registryPermit, this );
}

Collision2DComponent::~Collision2DComponent() {

	Collision2DManager* const collision
		= dynamic_cast<Collision2DManager*>(&Game::GetCollisionManager());
	assert( collision != nullptr 
		&& "Collision2DComponent requires a Collision2D manager." );
	Collision2DRegistryPermit registryPermit;
	collision->unregisterComponent( registryPermit, this );
}

void Collision2DComponent::addCollider( ICollider2D * const t_collider ) {

	assert( std::find( m_colliders.begin(), m_colliders.end(), t_collider )
		== m_colliders.end() && "Collider is already added to component" );
	m_colliders.push_back( t_collider );
}

void Collision2DComponent::removeCollider( ICollider2D * const t_collider ) {

	auto iter = std::find( m_colliders.begin(), m_colliders.end(), t_collider );
	assert( iter != m_colliders.end() && "Collider does not exist in component" );
	m_colliders.erase( iter );
}

void Collision2DComponent::setCollisionFlags( unsigned int t_flags ) {

	m_collisionFlags = t_flags;
}
unsigned int Collision2DComponent::getCollissionFlags() {

	return m_collisionFlags;
}

void Collision2DComponent::setCollideWithFlags( unsigned int t_flags ) {

	m_collideWithFlags = t_flags;
}
unsigned int Collision2DComponent::getCollideWithFlags() {

	return m_collideWithFlags;
}

const std::vector<ICollider2D*>& Collision2DComponent::getColliders() const {

	return m_colliders;
}

void Collision2DComponent::prepare() {

	getOwner().getTransform().calculateMatrix();
	for ( auto& collider : m_colliders ) {

		collider->prepare();
	}
	m_collisions.clear();
}

void Collision2DComponent::queueCollision( const CollisionGroup2D& t_collision ) {

	m_collisions.push_back( t_collision );
}

void Collision2DComponent::processCollisions() {

	for ( auto& group : m_collisions ) {

		getOwner().dispatchEvent( Collision2DMessage( group.collisions, group.other ) );
	}
	if ( getOwner().hasComponent<PhysicsComponent2D>() ) {
	
		PhysicsComponent2D& physics = getOwner().getComponent<PhysicsComponent2D>();
		physics.processCollisions( m_collisions );
	}
}

const std::vector<CollisionGroup2D>& Collision2DComponent::getRecentCollisions() {

	return m_collisions;
}

glm::vec4 Collision2DComponent::getBounds() const {

	assert( m_colliders.size() > 0 && "Component has no colliders" );
	glm::vec2 x{ m_colliders[0]->getBounds( { 1.0f, 0.0f } ) };
	glm::vec2 y{ m_colliders[0]->getBounds( { 0.0f, 1.0f } ) };

	for ( auto collider : m_colliders ) {

		glm::vec2 newX{ collider->getBounds( { 1.0f, 0.0f } ) };
		glm::vec2 newY{ collider->getBounds( { 0.0f, 1.0f } ) };
		if ( newX[0] < x[0] ) {

			x[0] = newX[0];
		}
		if ( newX[1] > x[1] ) {

			x[1] = newX[1];
		}
		if ( newY[0] < y[0] ) {

			y[0] = newY[0];
		}
		if ( newY[1] > y[1] ) {

			y[1] = newY[1];
		}
	}

	return{ x[0], y[0], x[1], y[1] };
}

class Collision2DManager::CollisionImpl {

	friend class Collision2DManager;

	CollisionImpl() {

	}

	~CollisionImpl() {

	}

	void registerComponent( Collision2DComponent* const t_component ) {

		m_collidables.push_back( t_component );
	}

	void unregisterComponent( Collision2DComponent* const t_component ) {

		auto iter = std::find( m_collidables.begin(), m_collidables.end()
			, t_component );
		assert( iter != m_collidables.end() && "Collidable not registered." );
		m_collidables.erase( iter );
		if ( m_map.has( t_component ) ) {

			m_map.remove( t_component );
		}
	}

	bool findCollisionsBetweenActors
		( Collision2DComponent* t_componentA
		, Collision2DComponent* t_componentB
		, std::vector<Collision2D>& t_collisions ) {

		// get colliders

		const std::vector<ICollider2D*>& collidersA 
			= t_componentA->getColliders();
		const std::vector<ICollider2D*>& collidersB 
			= t_componentB->getColliders();

		for ( auto colliderA : collidersA ) {

			colliderA->prepare();
		}
		for ( auto colliderB : collidersB ) {

			colliderB->prepare();
		}
		// find all collisions between all shapes and return them

		for( auto colliderA : collidersA ) { 

			for ( auto colliderB : collidersB ) {

				Collision2D collision =
					Collision2D::fromColliders( colliderA, colliderB );
				if ( collision.hasCollided() ) {

					t_collisions.push_back( collision );
				}
			}
		}
		return t_collisions.size() > 0;
	}

	void findAndHandleCollisions( Collision2DComponent* t_component ) {

		glm::vec4 bounds = t_component->getBounds();
		std::vector<void*> candidates = m_map.query( { bounds }, { bounds[2], bounds[3] } );
		for ( auto pointer : candidates ) {

			Collision2DComponent* other = static_cast<Collision2DComponent*>(pointer);

			std::vector<Collision2D> results;
			if ( other->isActive() && t_component != other
				&& t_component->getCollideWithFlags() & other->getCollissionFlags()
				&& other->getCollideWithFlags() & t_component->getCollissionFlags()
				&& findCollisionsBetweenActors( t_component, other, results ) ) {

				t_component->queueCollision( { other->getOwner(), results } );
			}
		}
	}

	std::vector<CollisionGroup2D> findCollisionsWithActors( ICollider2D* const t_collider ) {

		glm::vec2 x{ t_collider->getBounds( { 1.0f, 0.0f } ) };
		glm::vec2 y{ t_collider->getBounds( { 0.0f, 1.0f } ) };
		glm::vec4 bounds = { x[0], y[0], x[1], y[1] };
		std::vector<void*> candidates = m_map.query( { bounds }, { bounds[2], bounds[3] } );
		t_collider->prepare();
		std::vector<CollisionGroup2D> output;
		for ( auto pointer : candidates ) {

			Collision2DComponent* other = static_cast<Collision2DComponent*>(pointer);
			if ( !other->isActive() ) {

				continue;
			}
			std::vector<Collision2D> results;

			// get colliders

			const std::vector<ICollider2D*>& colliders
				= other->getColliders();

			for ( auto collider : colliders ) {

				collider->prepare();
			}

			// find all collisions between all shapes and return them

			for ( auto collider : colliders ) {
				Collision2D collision =
					Collision2D::fromColliders( t_collider, collider );
				if ( collision.hasCollided() ) {

					results.push_back( collision );
				}
			}

			if ( results.size() > 0 ) {
			
				output.push_back( { other->getOwner(), results } );
			}
		}

		return output;
	}

	void simulate( float t_delta ) {

		CallPhysics2DIncrementPermit permit;
		PhysicsComponent2D::call( permit, t_delta );

		for ( auto component : m_collidables ) {

			if ( component->isActive() ) {

				component->prepare();
				glm::vec4 bounds = component->getBounds();
				if ( m_map.has( component ) ) {

					m_map.move( component, { bounds }, { bounds[2], bounds[3] } );
				}
				else {

					m_map.add( component, { bounds }, { bounds[2], bounds[3] } );
				}
			}
		}

		for ( auto component : m_collidables ) {

			if ( component->isActive() ) {
			
				findAndHandleCollisions( component );
				component->processCollisions();
			}
		}
	}

	std::vector<CollisionGroup2D> query( ICollider2D* const t_collider ) {

		return findCollisionsWithActors( t_collider );
	}

	std::vector<Collision2DComponent*> m_collidables;
	SpatialHashMap2D m_map;
};

Collision2DManager::Collision2DManager( ConstructCollisionPermit& t_permit ) 
	: IGameCollision( t_permit ) {

	m_collision = new CollisionImpl();
}

Collision2DManager::~Collision2DManager() {

	delete m_collision;
}

IGameCollision* Collision2DManager::createInstance( ConstructCollisionPermit& t_permit ) {
	
	return new Collision2DManager( t_permit );
}

void Collision2DManager::simulate( float t_delta ) {

	m_collision->simulate( t_delta );
}

std::vector<CollisionGroup2D> Collision2DManager::query( ICollider2D* const t_collider ) {

	return m_collision->query( t_collider );
}

void Collision2DManager::registerComponent( Collision2DRegistryPermit& t_permit
	, Collision2DComponent* const t_component ) {

	t_permit.use();

	m_collision->registerComponent( t_component );
}

void Collision2DManager::unregisterComponent( Collision2DRegistryPermit& t_permit
	, Collision2DComponent* const t_component ) {

	t_permit.use();

	m_collision->unregisterComponent( t_component );
}