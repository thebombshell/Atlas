
// input.hpp

// header file for the public implementation of the input manager class

#ifndef PANTHEON_INPUT_HPP
#define PANTHEON_INPUT_HPP

#include "pantheon.hpp"

#include <string>

namespace pantheon {

	class PANTHEON_API Input {

	private:

		class InputImpl;
		InputImpl* m_input{ nullptr };

	public:

		Input();
		~Input();

		// simulates key changes

		void pressKey( const std::string& t_name );
		void releaseKey( const std::string& t_name );
		void updateJoystickAxis( int t_joystickIndex, int t_axisIndex );
		void updateJoystickButton( int t_joystickIndex, int t_buttonIndex );
		void updateMousePosition( int t_x, int t_y );
		void updateMouseButton( int t_index, bool t_isPressed );
		// check key or axis is not 0

		bool isKeyDown( const std::string& t_name ) const;

		// get key or axis value in the range of -1.0 to 1.0, 0 being inactive

		float getAxisValue( const std::string& t_name ) const;

		// get axis value given a positive and negative contributor in the range of -1.0 to 1.0

		float getAxisValue
			( const std::string& t_negative
			, const std::string& t_possitive ) const;

		// similar fucntions with deadzone tollerances

		bool isKeyDown( const std::string& t_name, float t_tollerance ) const;
		float getAxisValue
			( const std::string& t_name, float t_tollerance ) const;
		float getAxisValue
			( const std::string& t_negative
			, const std::string& t_possitive, float t_tollerance ) const;
		glm::vec2 getMousePosition() const;

		int getJoystickCount() const;
	};
}

#endif