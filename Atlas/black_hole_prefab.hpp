
// black_hole_prefab.hpp

#ifndef ATLAS_BLACK_HOLE_PREFAB_HPP
#define ATLAS_BLACK_HOLE_PREFAB_HPP

#include <pantheon.hpp>

namespace atlas {

	class BlackHolePrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		BlackHolePrefab( pthn::ConstructComponentPermit&, pthn::Actor& );
		~BlackHolePrefab();

		void update( float t_delta ) override;
		void render() override;

	private:

		float m_timer;
	};
}

#endif