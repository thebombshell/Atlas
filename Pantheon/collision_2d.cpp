
// collision_2d.cpp

// source file for 2d collision manager

#include "collision_2d.hpp"

#include "physics_component.hpp"

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

void pantheon::Collision2DComponent::addCollider( ICollider2D * const t_collider ) {

	assert( std::find( m_colliders.begin(), m_colliders.end(), t_collider )
		== m_colliders.end() && "Collider is already added to component" );
	m_colliders.push_back( t_collider );
}

void pantheon::Collision2DComponent::removeCollider( ICollider2D * const t_collider ) {

	auto iter = std::find( m_colliders.begin(), m_colliders.end(), t_collider );
	assert( iter != m_colliders.end() && "Collider does not exist in component" );
	m_colliders.erase( iter );
}

const std::vector<ICollider2D*>& pantheon::Collision2DComponent::getColliders() const {

	return m_colliders;
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

		for( auto other : m_collidables ) {

			std::vector<Collision2D> results;
			if ( other->isActive() && t_component != other 
				&& findCollisionsBetweenActors( t_component, other, results ) ) {

				t_component->getOwner().dispatchEvent<Collision2DMessage>
					( { results, other->getOwner() } );
			}
		}
	}

	void simulate( float t_delta ) {

		CallPhysics2DIncrementPermit permit;
		PhysicsComponent2D::call( permit, t_delta );
		for( auto component : m_collidables ) {

			if ( component->isActive() ) {
			
				findAndHandleCollisions( component );
			}
		}
	}

	std::vector<Collision2DComponent*> m_collidables;
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