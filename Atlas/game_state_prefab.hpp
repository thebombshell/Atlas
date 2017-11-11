
// game_state_prefab.hpp

#ifndef ATLAS_GAME_STATE_PREFAB_HPP
#define ATLAS_GAME_STATE_PREFAB_HPP

#include <pantheon.hpp>

namespace atlas {

	const int CHEAT_AWESOME{ 1 };
	const int CHEAT_MEOW{ 2 };
	const int CHEAT_STRONK{ 4 };

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