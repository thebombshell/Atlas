
// boss_prefab.hpp

#ifndef ATLAS_BOSS_PREFAB
#define ATLAS_BOSS_PREFAB

#include <pantheon.hpp>

namespace atlas {

	class BossPrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		BossPrefab( pthn::ConstructComponentPermit&, pthn::Actor& );
		~BossPrefab();

		void update( float t_delta ) override;

		void render() override;

		void shoot();

	private:

		float m_timer;
	};
}

#endif