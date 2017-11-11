
// health_component.hpp

#ifndef ATLAS_HEALTH_COMPONENT_HPP
#define ATLAS_HEALTH_COMPONENT_HPP

#include <pantheon.hpp>

namespace atlas {

	class DamageMessage : public pthn::IActorEventMessage {

	public:

		DamageMessage( int t_damage );

		int damage;
	};
	class HealMessage : public pthn::IActorEventMessage {

	public:

		HealMessage( int t_health );

		int health;
	};

	class HealthComponent : public pthn::IActorComponent {

	public:
		HealthComponent( pthn::ConstructComponentPermit&, pthn::Actor& );
		~HealthComponent();

		void damage(int t_damage);
		void heal(int t_heal);

		int health{ 0 };
	};
}

#endif