
// input_manager.hpp

// Header file for the input manager.

#ifndef WIREFRAMETDS_INPUT_MANAGER_HPP
#define WIREFRAMETDS_INPUT_MANAGER_HPP

#include "wireframe_tds.hpp"
#include <map>
#include <string>

namespace wireframe_tds {

	const char WTDS_INPUT_KEY_UP{ 0 };
	const char WTDS_INPUT_KEY_DOWN{ 1 };

	class InputManager {
	private:

		std::map<std::string, unsigned long long> m_keyDownTimes{ };
		std::map<std::string, unsigned long long> m_keyUpTimes{ };
	public:

		// constructor and deconstructor

		InputManager(
			);
		~InputManager(
			);

		// on key pressed callback function

		void onKeyPressed(
			std::string t_key,
			int t_x,
			int t_y
			);

		// on key pressed callback function

		void onKeyReleased(
			std::string t_key,
			int t_x,
			int t_y
			);

		// get time since key down function

		unsigned long long getTimeSinceDown(
			std::string t_key
			) const;

		// get time since key up function

		unsigned long long getTimeSinceUp(
			std::string t_key
			) const;

		// get if key is down function

		int getKeyState(
			std::string t_key
			) const;
	};
}

#endif