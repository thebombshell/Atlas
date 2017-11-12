
// iactor_component.cpp

#include "iactor_component.hpp"

#include "static_subscriber_event.hpp"

using namespace pantheon;

///////////////////////////////////////////////////////////////////////////////
// IActorComponent
///////////////////////////////////////////////////////////////////////////////

void IActorComponent::disable() {

	m_isActive = false;
	onDisable();
}

void IActorComponent::enable() {

	m_isActive = true;
	onEnable();
}

void IActorComponent::onEnable() {
}

void IActorComponent::onDisable() {
}

bool IActorComponent::isActive() {

	return m_isActive && m_owner.isActive();
}

Actor& IActorComponent::getOwner() {

	return m_owner;
}

///////////////////////////////////////////////////////////////////////////////
// IUpdatable
///////////////////////////////////////////////////////////////////////////////

typedef StaticSubscriptionEvent<IUpdatable> Updatables;

IUpdatable::IUpdatable() {

	Updatables::add( this );
}

IUpdatable::~IUpdatable() {

	Updatables::remove( this );
}

void IUpdatable::call( CallUpdatablesPermit& t_permit, float t_delta ) {

	t_permit.use();

	Updatables::forEach( [&t_delta]( IUpdatable* const t_updatable ) {
		if ( t_updatable->isActive() ) {

			t_updatable->update( t_delta );
		}
	} );
}

///////////////////////////////////////////////////////////////////////////////
// IRenderable
///////////////////////////////////////////////////////////////////////////////

typedef StaticSubscriptionEvent<IRenderable> Renderables;

IRenderable::IRenderable() {

	Renderables::add( this );
}

IRenderable::~IRenderable() {

	Renderables::remove( this );
}

void IRenderable::call( CallRenderablesPermit& t_permit ) {

	t_permit.use();

	Renderables::forEach( []( IRenderable* const t_renderable ) {
		if ( t_renderable->isActive() ) {

			t_renderable->render();
		}
	} );
}
