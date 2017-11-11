
// audio.hpp

// header file of the public implementation of the audio manager

#ifndef PANTHEON_AUDIO_HPP
#define PANTHEON_AUDIO_HPP

#include "pantheon.hpp"

#include "permission.hpp"

#include <string>

namespace pantheon {

	class P_ConstructSource;
	typedef Permission<Audio, P_ConstructSource> ConstructSourcePermit;

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

		// creates and returns a source object for a given sound

		Source* const createSource( std::string t_soundName );

		// deletes a source object

		void deleteSource( Source* const t_source );

	private:

		class AudioImpl;
		AudioImpl* m_audio;
	};

	class PANTHEON_API Source {

	public:

		Source( ConstructSourcePermit, unsigned int t_buffer );
		~Source();

		void play();

		void replay();

		void setLooping();

		void setSingle();

		void pause();

		void stop();

		bool isPlaying() const;

		bool isPaused() const;

		bool isStopped() const;

		void setGain( float t_value );

		void setPitch( float t_value );

	private:

		unsigned int m_id;
		unsigned int const m_buffer;
	};
}

#endif