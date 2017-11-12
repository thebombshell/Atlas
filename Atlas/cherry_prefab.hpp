
// cherry_prefab.hpp

#ifndef ATLAS_CHERRY_PREFAB_HPP
#define ATLAS_CHERRY_PREFAB_HPP

#include <pantheon.hpp>
#include <shapes_2d.hpp>

namespace atlas {

	class CherryPrefab : public pthn::IRenderable {

	public:

		CherryPrefab( pthn::ConstructComponentPermit&, pthn::Actor& );
		~CherryPrefab();

		void render() override;

		void onEventMessage( pthn::IActorEventMessage* const t_message ) override;

	private:

		pthn::Circle m_circle;
	};
}
#endif