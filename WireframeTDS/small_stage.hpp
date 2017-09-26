
// small_stage.hpp

// Header file for the small stage class

#ifndef WIREFRAMETDS_SMALL_STAGE_HPP
#define WIREFRAMETDS_SMALL_STAGE_HPP

#include "wireframe_tds.hpp"
#include "game_actor.hpp"

namespace atlas {

	namespace game {

		class SmallStage : public IGameRenderable {
		private:

		public:
			SmallStage(
				int
				);
			~SmallStage(
				) override;
			void onRender(
				float delta
				) const override;
		};
	}

}

#endif