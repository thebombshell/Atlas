
// audio.hpp

// header file of the public implementation of the audio manager

#ifndef PANTHEON_AUDIO_HPP
#define PANTHEON_AUDIO_HPP

#include "pantheon.hpp"

#include <string>

namespace pantheon {

	class PANTHEON_API Audio {

	public:

		Audio();
		~Audio();

		// load wav file as sound

		void loadSound( std::string t_path );

		// load wav file as sound with explicit name

		void loadSound( std::string t_path, std::string t_name );

		// unload sound of name

		void unloadSound( std::string t_name );

		// returns true if sound of name exists

		bool hasSound( std::string t_name );

		// play sound of name

		void playSound( std::string t_name );

	private:

		class AudioImpl;
		AudioImpl* m_audio;
	};
}

#endif