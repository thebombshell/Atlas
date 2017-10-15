
// audio.cpp

// source file for the audio management class

#include "audio.hpp"

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alure.h>
#include <map>
#include <mutex>
#include <thread>

using namespace pantheon;

class Audio::AudioImpl {

	friend class Audio;

	AudioImpl() {

		bool isSuccess = alureInitDevice( NULL, NULL );
		assert( isSuccess && "Audio device could not be initialized." );
		alGenSources( 4, m_sources );
		for ( int i = 0; i < 4; ++i ) {
			alSourcef( m_sources[i], AL_GAIN, 1.0f );
			alSourcef( m_sources[i], AL_PITCH, 1.0f );
			alSource3f( m_sources[1], AL_POSITION, 0.0f, 0.0f, -1.0f );
			alSource3f( m_sources[1], AL_VELOCITY, 0.0f, 0.0f, 0.0f );
		}
	}

	~AudioImpl() {

		bool isSuccess = alureShutdownDevice();
		assert( isSuccess && "Audio device could not be finalized." );
	}


	void loadSound( std::string t_path ) {
		
		assert( !hasSound( t_path ) && "Sound already exists." );
		
		ALuint sound = alureCreateBufferFromFile( t_path.c_str() );

		if ( sound == AL_NONE ) {

			printf( (alureGetErrorString() + std::string( "\n" )).c_str() );
			assert( false && "Could not create sound from path." );
		}
		
		m_sounds.insert( { t_path, sound } );
	}

	void loadSound( std::string t_path, std::string t_name ) {

		assert( !hasSound( t_name ) && "Sound already exists." );
		
		ALuint sound = alureCreateBufferFromFile( t_path.c_str() );

		if ( sound == AL_NONE ) {

			printf( (alureGetErrorString() + std::string( "\n" )).c_str() );
			assert( false && "Could not create sound from path." );
		}

		m_sounds.insert( { t_name, sound } );
	}

	void unloadSound( std::string t_name ) {

		assert( hasSound( t_name ) && "Sound does not exist." );

		ALuint buffer = m_sounds.find( t_name )->second;
		alDeleteBuffers( 1, &buffer );
		m_sounds.erase( t_name );
	}

	bool hasSound( std::string t_name ) {

		return m_sounds.find( t_name ) != m_sounds.end();
	}

	void playSound( std::string t_name ) {

		assert( hasSound( t_name ) && "Sound does not exist." );

		std::lock_guard<std::mutex> sourceGuard{ m_sourceMutex };

		ALuint buffer = m_sounds.find( t_name )->second;

		for ( int i = 0; i < 4; ++i ) {

			int sourceState = 0;
			alGetSourcei( m_sources[i], AL_SOURCE_STATE, &sourceState );
			if ( sourceState != AL_PLAYING ) {

				alSourcei( m_sources[i], AL_BUFFER, buffer );
				alSourcePlay( m_sources[i] );
				break;
			}
		}
	}

	ALCdevice* m_device{ nullptr };
	ALuint m_sources[4]{ 0, 0, 0, 0 };
	std::mutex m_sourceMutex;
	bool m_sourcesInUse[4]{ false, false, false, false };
	std::map<std::string, ALuint> m_sounds;
};

Audio::Audio() {

	m_audio = new AudioImpl();
}

Audio::~Audio() {

	delete m_audio;
}

void Audio::loadSound( std::string t_path ) {

	m_audio->loadSound( t_path );
}

void Audio::loadSound( std::string t_path, std::string t_name ) {

	m_audio->loadSound( t_path, t_name );
}

void Audio::unloadSound( std::string t_name ) {

	m_audio->unloadSound( t_name );
}

bool Audio::hasSound( std::string t_name ) {

	return m_audio->hasSound( t_name );
}

void Audio::playSound( std::string t_name ) {

	m_audio->playSound( t_name );
}