
// boss_prefab.hpp

#ifndef ATLAS_BOSS_PREFAB
#define ATLAS_BOSS_PREFAB

#include <pantheon.hpp>
#include <shapes_2d.hpp>

namespace atlas {

	class BossPrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		BossPrefab( pthn::ConstructComponentPermit&, pthn::Actor& );
		~BossPrefab();

		void update( float t_delta ) override;

		void render() override;

		void shoot();

		void onEventMessage( pthn::IActorEventMessage* const t_message ) override;

	private:

		float m_timer;
		pthn::ConvexHull m_hull;
		pthn::Source* m_shootSource;
	};
}

#endif