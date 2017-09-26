
// input_manager.cpp

// Source file for the input manager class

#include "input_manager.hpp"

#include <chrono>

using namespace atlas;

InputManager::InputManager(
	) {

}

InputManager::~InputManager(
	) {

}

void InputManager::onKeyPressed(
	std::string t_key,
	int,
	int
	) {

	m_keyDownTimes[t_key] =
		std::chrono::system_clock::now().time_since_epoch()
		/ std::chrono::milliseconds( 1 );
}

// on key pressed callback function

void InputManager::onKeyReleased(
	std::string t_key,
	int,
	int
	) {

	m_keyUpTimes[t_key] =
		std::chrono::system_clock::now().time_since_epoch()
		/ std::chrono::milliseconds( 1 );
}

// get time since key down function

unsigned long long InputManager::getTimeSinceDown(
	std::string t_key
	) const {
	
	auto iter = m_keyDownTimes.find(t_key);
	if ( iter == m_keyDownTimes.end() )
		return 0;
	return (*iter).second;
}

// get time since key up function

unsigned long long InputManager::getTimeSinceUp(
	std::string t_key
	) const {

	auto iter = m_keyUpTimes.find( t_key );
	if ( iter == m_keyUpTimes.end() )
		return 0;
	return (*iter).second;
}

// get if key is down function

int InputManager::getKeyState(
	std::string t_key
	) const {

	unsigned long long downTime = getTimeSinceDown( t_key );
	unsigned long long upTime = getTimeSinceUp( t_key );

	if ( downTime > upTime ) {

		return WTDS_INPUT_KEY_DOWN;
	}
	return WTDS_INPUT_KEY_UP;
}