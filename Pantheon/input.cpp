
// input.cpp

// source file for the input manager

#include "input.hpp"

#include <algorithm>
#include <map>
#include <SDL.h>
#include <vector>
#include <limits>

using namespace pantheon;

class Input::InputImpl {

	friend class Input;

	InputImpl() {

		// for each joystick try to open, if open is successful store under joystick id

		int joystickCount = SDL_NumJoysticks();
		for ( int i = 0; i < joystickCount; i++ ) {

			SDL_Joystick* joystick = SDL_JoystickOpen( i );
			if ( joystick != NULL ) {

				SDL_JoystickID id = SDL_JoystickInstanceID( joystick );
				m_joysticks.insert( { id, joystick } );
				std::string name = SDL_JoystickName( joystick );
				if ( name.find( "XInput" ) == 0 ) {

					updateJoystickAxis( id, 2 );
					updateJoystickAxis( id, 5 );
				}
				
			}
		}
	}

	~InputImpl() {

		// close all open joysticks

		for ( auto pair : m_joysticks ) {

			SDL_JoystickClose( pair.second );
		}
	}

	void updateJoystickAxis( int t_joystickIndex, int t_axisIndex ) {

		// get joystick and axis value

		SDL_Joystick* joystick = m_joysticks[t_joystickIndex];
		int rawValue = SDL_JoystickGetAxis( joystick, t_axisIndex );

		// convert value to range -1.0 to 1.0

		float value = static_cast<float>(rawValue) / 32768.0f;

		// find axis name and store value 

		std::string name = "joystick" + std::to_string( t_joystickIndex ) + "_axis" + std::to_string( t_axisIndex );
		m_values[name] = -value;
	}

	void updateJoystickButton( int t_joystickIndex, int t_buttonIndex ) {

		// get joystick and button value

		SDL_Joystick* joystick = m_joysticks[t_joystickIndex];
		char rawValue = SDL_JoystickGetButton( joystick, t_buttonIndex );

		// convert value to range 0.0 to 1.0

		float value = rawValue ? 1.0f : 0.0f;

		// find axis name and store value 

		std::string name = "joystick" + std::to_string( t_joystickIndex ) + "_button" + std::to_string( t_buttonIndex );
		m_values[name] = value;

	}

	float getAxisValue( const std::string& t_key ) const {

		auto value = m_values.find( t_key );
		if ( value == m_values.end() ) {

			return 0.0f;
		}
		return value->second;
	}

	std::map<std::string, float> m_values;
	std::map<SDL_JoystickID, SDL_Joystick*> m_joysticks;
};

Input::Input() {

	m_input = new InputImpl();
}

Input::~Input() {

	delete m_input;
}

void Input::pressKey( const std::string& m_key ) {

	m_input->m_values[m_key] = 1.0f;
}

void Input::releaseKey( const std::string& m_key ) {

	m_input->m_values[m_key] = 0.0f;
}

void Input::updateJoystickAxis( int t_joystickIndex, int t_axisIndex ) {

	m_input->updateJoystickAxis( t_joystickIndex, t_axisIndex );
}
void Input::updateJoystickButton( int t_joystickIndex, int t_buttonIndex ) {

	m_input->updateJoystickButton( t_joystickIndex, t_buttonIndex );
}
void Input::updateMousePosition( int t_x, int t_y ) {

	m_input->m_values["mouse_x"] = t_x;
	m_input->m_values["mouse_y"] = t_y;
}

void Input::updateMouseButton( int t_index, bool t_isPressed ) {

	m_input->m_values["mouse_button" + std::to_string( t_index )] = t_isPressed ? 1.0f : 0.0f;
}

bool Input::isKeyDown( const std::string& t_key ) const {

	return abs( m_input->getAxisValue( t_key ) ) > 0.001f;
}

bool Input::isKeyDown( const std::string& t_key, float t_tollerance ) const {

	return abs( m_input->getAxisValue( t_key ) ) > t_tollerance;
}

float Input::getAxisValue( const std::string& t_key ) const {

	return m_input->getAxisValue( t_key );
}

float Input::getAxisValue( const std::string& t_key, float t_tollerance ) const {

	float value = m_input->getAxisValue( t_key );
	if ( abs( value ) < t_tollerance ) {

		return 0.0f;
	}
	return value;
}

float Input::getAxisValue( const std::string& t_negative, const std::string& t_positive ) const {

	return m_input->getAxisValue( t_positive ) - m_input->getAxisValue( t_negative );
}

float Input::getAxisValue( const std::string& t_negative, const std::string& t_positive, float t_tollerance ) const {

	float value = m_input->getAxisValue( t_positive ) - m_input->getAxisValue( t_negative );
	if ( abs( value ) < t_tollerance ) {

		return 0.0f;
	}
	return value;
}

glm::vec2 Input::getMousePosition() const {

	return{ getAxisValue( "mouse_x" ), getAxisValue( "mouse_y" ) };
}

int Input::getJoystickCount() const {

	return m_input->m_joysticks.size();
}