
// game_state_prefab.hpp

#ifndef ATLAS_GAME_STATE_PREFAB_HPP
#define ATLAS_GAME_STATE_PREFAB_HPP

#include <pantheon.hpp>

namespace atlas {

	const int CHEAT_AWESOME{ 1 };
	const int CHEAT_BOSS{ 2 };
	const int CHEAT_ESCAPE{ 4 };
	const int CHEAT_GTFO{ 8 };
	const int CHEAT_WHEELCHAIR{ 16 };

	const int CHEAT_THOR{ 32 };
	const int CHEAT_HAWKING{ 64 };
	const int CHEAT_JUGGERNAUGHT{ 128 };
	const int CHEAT_WAKKA{ 256 };
	const int CHEAT_360NOSCOPE{ 512 };
	const int CHEAT_NOEP{ 1024 };
	const int CHEAT_STARFOX{ 2048 };
	const int CHEAT_DISNEY{ 4096 };
	const int CHEAT_OHGODWHY{ 8192 };
	const int CHEAT_BEWORTHY{ 16384 };

	const int CHEAT_ZERO{ 32768 };

	bool isCheatActive( int t_cheatName );

	class GameStatePrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		GameStatePrefab( pthn::ConstructComponentPermit&, pthn::Actor& );
		~GameStatePrefab();

		void update( float t_delta ) override;

		void render() override;
	};
}

#endif