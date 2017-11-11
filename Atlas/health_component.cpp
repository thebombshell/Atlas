
// health_component.cpp

#include "health_component.hpp"

using namespace pantheon;
using namespace atlas;

DamageMessage::DamageMessage( int t_damage ) : damage{ t_damage } {

}
HealMessage::HealMessage( int t_health ) : health{ t_health } {

}
HealthComponent::HealthComponent( ConstructComponentPermit& t_permit, Actor& t_owner )
	: IActorComponent{ t_permit, t_owner } {

}
HealthComponent::~HealthComponent() {

}

void HealthComponent::damage( int t_damage ) {

	health -= t_damage;
	DamageMessage message{ t_damage };
	getOwner().dispatchEvent( message );
}

void HealthComponent::heal( int t_heal ) {

	health += t_heal;
	HealMessage message{ t_heal };
	getOwner().dispatchEvent( message );
}