
// pantheon.hpp

// header file for the pantheon

#ifndef PANTHEON_PANTHEON_HPP
#define PANTHEON_PANTHEON_HPP

#include "no_silly_warnings.hpp"

#define GLM_FORCE_CXX03
#include <glm\glm.hpp>

namespace pantheon {

	const int PANTHEON_OK{ 0 };
	const int PANTHEON_ERROR{ 1 };

	class Game;
	class Actor;
	class IActorComponent;
}

#ifdef PANTHEON_EXPORTS

#define PANTHEON_API __declspec(dllexport) 

#else

#define PANTHEON_API __declspec(dllimport)

#include <actor.hpp>
#include <game.hpp>
#include <iactor_component.hpp>
#include <transform.hpp>

#endif //PANTHEON_EXPORT

#endif //PANTHEON_PANTHEON_HPP