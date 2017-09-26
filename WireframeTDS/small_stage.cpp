
// player_ship.cpp

// Source file for the player ship class

#include "small_stage.hpp"
#include "game.hpp"
const float pi = 3.14159265358979323846f;
const float pi2 = pi * 2;

using namespace atlas;

game::SmallStage::SmallStage(
	int
	) {

}

game::SmallStage::~SmallStage(
	) {

}

void game::SmallStage::onRender(
	float
	) const {

	// set up debug triangle

	float lines[]{
		50.0f, 50.0f, 0.2f, 1.0f, 1.0f, 1.0f,
		50.0f, -50.0f, 0.2f, 1.0f, 1.0f, 1.0f,
		50.0f, -50.0f, 0.2f, 1.0f, 1.0f, 1.0f,
		-50.0f, -50.0f, 0.2f, 1.0f, 1.0f, 1.0f,
		-50.0f, -50.0f, 0.2f, 1.0f, 1.0f, 1.0f,
		-50.0f, 50.0f, 0.2f, 1.0f, 1.0f, 1.0f,
		-50.0f, 50.0f, 0.2f, 1.0f, 1.0f, 1.0f,
		50.0f, 50.0f, 0.2f, 1.0f, 1.0f, 1.0f,
	};

	Game::drawLines( 8, lines );
}
