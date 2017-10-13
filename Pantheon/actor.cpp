
// actor.cpp

// source file for actor class

#include "actor.hpp"

#include "iactor_component.hpp"

#include <assert.h>

using namespace pantheon;

class Actor::ActorImpl {

	friend class Actor;

	ActorImpl() {


	}

	~ActorImpl() {

		for ( const auto& iter : m_components ) {
			
			delete iter.second;
		}
	}

	void addComponent( std::type_index t_typeId
		, IActorComponent* t_component ) {

		assert( m_components.find( t_typeId ) == m_components.end()
			&& "Component of type already found in actor." );
		m_components.insert( std::pair<std::type_index
			, IActorComponent*>{ t_typeId, t_component } );
	}

	bool hasComponent( std::type_index t_typeId ) const {

		return m_components.find( t_typeId ) != m_components.end();
	}

	IActorComponent& getComponent( std::type_index t_typeId ) const {

		auto component = m_components.find( t_typeId );
		assert( component != m_components.end()
			&& "Component was not found in actor." );
		return *component->second;
	}


	IActorComponent* removeComponent( std::type_index t_typeId ) {

		auto component = m_components.find( t_typeId );
		IActorComponent* value = component->second;
		assert( component != m_components.end()
			&& "Component was not found in actor." );
		m_components.erase( component );
		return value;
	}

	std::map<std::type_index, IActorComponent*> m_components;
};

Actor::Actor() {

	m_actor = new ActorImpl();
}

Actor::~Actor() {

	delete m_actor;
}

void Actor::addComponent( std::type_index t_typeId
	, IActorComponent* t_component ) {

	m_actor->addComponent( t_typeId, t_component );
}

bool Actor::hasComponent( std::type_index t_typeId ) const {

	return m_actor->hasComponent( t_typeId );
}

IActorComponent& pantheon::Actor::getComponent( std::type_index t_typeId )
	const {

	return m_actor->getComponent( t_typeId );
}

IActorComponent* Actor::removeComponent( std::type_index t_typeId ) {

	return m_actor->removeComponent( t_typeId );
}

ActorComponentIterator Actor::begin() const {

	return m_actor->m_components.begin();
}
ActorComponentIterator Actor::end() const {

	return m_actor->m_components.end();
}