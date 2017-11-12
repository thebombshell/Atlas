
// force_pad_prefab.hpp

#ifndef ATLAS_FORCE_PAD_PREFAB_HPP
#define ATLAS_FORCE_PAD_PREFAB_HPP

#include <pantheon.hpp>
#include <shapes_2d.hpp>

namespace atlas {

	class ForcePadMessage {

	public:

		ForcePadMessage( int t_index );

		int index;
	};
	class ForcePadPrefab : public pthn::IRenderable {

	public:

		ForcePadPrefab(pthn::ConstructComponentPermit&, pthn::Actor&, const ForcePadMessage& );
		~ForcePadPrefab();

		void render() override;

		void onEventMessage( pthn::IActorEventMessage* const t_message ) override;

	private:

		pthn::Circle m_circle;
		int m_index;
	};
}

#endif