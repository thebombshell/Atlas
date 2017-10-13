
// scene.cpp

// source file for the actor collection

#include "scene.hpp"

using namespace pantheon;

void Scene::update() {

	for( auto actor : m_addQueue ) {

		actor->enable();
		m_actors.push_back( actor );
	}
	m_addQueue.clear();
	for( auto actor : m_removeQueue ) {

		auto iter = std::find( m_actors.begin(), m_actors.end(), actor );
		m_actors.erase( iter );
	}
	m_removeQueue.clear();
}

Actor& Scene::createActor() {

	Actor* actor = new Actor();
	m_addQueue.push_back( actor );
	return *actor;
}

void Scene::destroyActor( Actor* const t_actor ) {

	assert( std::find( m_actors.begin(), m_actors.end(), t_actor )
		!= m_actors.end() && "Actor is not in scene." );
	assert( std::find( m_removeQueue.begin(), m_removeQueue.end(), t_actor )
		== m_removeQueue.end() && "Actor is already queued for removal." );

	t_actor->disable();
	m_removeQueue.push_back( t_actor );
}