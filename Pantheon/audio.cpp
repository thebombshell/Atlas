
// audio.cpp

// source file for the audio management class

#include "audio.hpp"

#include <AL\al.h>
#include <AL\alc.h>
#include <AL\alure.h>
#include <map>
#include <vector>

using namespace pantheon;

class Audio::AudioImpl {

	friend class Audio;

	AudioImpl() {
		
		ALCint attribs[]
			{ ALC_REFRESH, 20
			, ALC_SYNC, 0
			, 0 };
		bool isSuccess = alureInitDevice( NULL, attribs );
		assert( isSuccess && "Audio device could not be initialized." );

		alListener3f( AL_POSITION, 0.0f, 0.0f, 0.0f );
		alListener3f( AL_VELOCITY, 0.0f, 0.0f, 0.0f );

	}

	~AudioImpl() {

		while ( m_sources.size() > 0 ) {

			deleteSource( m_sources[0] );
		}

		while ( m_sounds.size() > 0 ) {

			unloadSound( m_sounds.begin()->first );
		}

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

	Source* const createSource( std::string t_soundName ) {

		assert( hasSound( t_soundName ) && "Sound does not exist." );
		unsigned int buffer = m_sounds.find( t_soundName )->second;
		Source* source = new Source( { }, buffer );
		m_sources.push_back( source );
		return source;
	}

	void deleteSource( Source* const t_source ) {

		auto iter = std::find( m_sources.begin(), m_sources.end(), t_source );
		assert( iter != m_sources.end() && "Source does not exist." );
		delete *iter;
		m_sources.erase( iter );
	}

	ALCdevice* m_device{ nullptr };
	std::map<std::string, ALuint> m_sounds;
	std::vector<Source*> m_sources;
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

Source* const Audio::createSource( std::string t_soundName ) {

	return m_audio->createSource( t_soundName );
}

void Audio::deleteSource( Source* const t_source ) {

	m_audio->deleteSource( t_source );
}

Source::Source( ConstructSourcePermit, unsigned int t_buffer ) : m_buffer{ t_buffer } {

	alGenSources( 1, &m_id );
	alSourcef( m_id, AL_GAIN, 1.0f );
	alSourcef( m_id, AL_PITCH, 1.0f );
	alSource3f( m_id, AL_POSITION, 0.0f, 0.0f, -1.0f );
	alSource3f( m_id, AL_VELOCITY, 0.0f, 0.0f, 0.0f );
	alSourcei( m_id, AL_BUFFER, m_buffer );

}

Source::~Source() {

	alSourceStop( m_id );
	alDeleteSources( 1, &m_id );
}

void Source::play() {

	alSourcePlay( m_id );
}

void Source::replay() {

	alSourceRewind( m_id );
	alSourcePlay( m_id );
}

void Source::setLooping() {

	alSourcei( m_id, AL_LOOPING, AL_TRUE );
}

void Source::setSingle() {

	alSourcei( m_id, AL_LOOPING, AL_FALSE );
}

void Source::pause() {

	alSourcePause( m_id );
}

void Source::stop() {

	alSourceStop( m_id );
}

bool Source::isPlaying() const {

	int state;
	alGetSourcei( state, AL_SOURCE_STATE, &state );
	return state == AL_PLAYING;
}

bool Source::isPaused() const {

	int state;
	alGetSourcei( state, AL_SOURCE_STATE, &state );
	return state == AL_PAUSED;
}

bool Source::isStopped() const {

	int state;
	alGetSourcei( state, AL_SOURCE_STATE, &state );
	return state == AL_STOPPED || state == AL_INITIAL;
}