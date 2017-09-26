
// forward_decleration.hpp

// Header file for forward declaration of most classes.

#ifndef WIREFRAMETDS_FORWARD_DECLARATION_HPP
#define WIREFRAMETDS_FORWARD_DECLARATION_HPP

#include "wireframe_tds.hpp"

namespace atlas {

	namespace collisions {

		struct ICollider;
		struct Circle;
		struct IConvexHull;
	}

	namespace game {

		class Bullet;
		class PlayerShip;
		class SmallStage;
	}

	class IGameActor;
	class IGameCollidable;
	class IGameRenderable;
	class IGameUpdatable;
	class InputManager;
	class Game;
	class RenderHelper;
	class Transform;

}

#endif