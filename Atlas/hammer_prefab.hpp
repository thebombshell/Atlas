
// hammer_prefab.hpp

#ifndef ATLAS_HAMMER_PREFAB_HPP
#define ATLAS_HAMMER_PREFAB_HPP

#include <pantheon.hpp>
#include <shapes_2d.hpp>

namespace atlas {

	class HammerPrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		HammerPrefab( pthn::ConstructComponentPermit&, pthn::Actor& );
		~HammerPrefab();

		void update( float t_delta ) override;

		void render() override;

		void onEventMessage( pthn::IActorEventMessage* const t_message ) override;

	private:

		float m_timer{ 0.0f };
		pthn::ConvexHull m_hull;
		glm::vec2 m_direction{ 0.0f, 0.0f };
	};

	class SuperHammerPrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		SuperHammerPrefab( pthn::ConstructComponentPermit&, pthn::Actor& );
		~SuperHammerPrefab();

		void update( float t_delta ) override;

		void render() override;

		void onEventMessage( pthn::IActorEventMessage* const t_message ) override;

	private:

		float m_timer{ 0.0f };
		pthn::ConvexHull m_hull;
		glm::vec2 m_direction{ 0.0f, 0.0f };
	};

	class ClockHammerPrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		ClockHammerPrefab( pthn::ConstructComponentPermit&, pthn::Actor& );
		~ClockHammerPrefab();

		void update( float t_delta ) override;

		void render() override;

		void onEventMessage( pthn::IActorEventMessage* const t_message ) override;

	private:

		float m_timer{ 0.0f };
		pthn::ConvexHull m_hull;
		glm::vec2 m_direction{ 0.0f, 0.0f };
	};
}

#endif