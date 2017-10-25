
// spatial_hash_map.hpp

#ifndef PANTHEON_SPATIAL_HASH_MAP
#define PANTHEON_SPATIAL_HASH_MAP

#include "pantheon.hpp"

#include <map>
#include <vector>

namespace pantheon {

	typedef glm::tvec2<short, glm::precision::mediump> shvec2;
	typedef glm::tvec4<short, glm::precision::mediump> shvec4;
	
	template<typename T>
	class PANTHEON_API SpatialHashMap2D {

	public:

		SpatialHashMap2D( int resolution ) : m_resolution { resolution } {

		}
		~SpatialHashMap2D() {

		}

		void add( T* t_element, const glm::vec2& t_start, const glm::vec2& t_end ) {

			assert( m_elements.find( t_element ) == m_elements.end()
				&& "element already exists in spatial hash map" );

			shvec2 start = glm::floor( t_start / static_cast<float>(m_resolution) );
			shvec2 end = glm::floor( t_start / static_cast<float>(m_resolution) );
			shvec4 key = { start, end };
			addMapping( t_element, key );
			m_elements.insert( { t_element, key } );
		}

		void remove( T* t_element ) {

			auto pair = m_elements.find( t_element );
			assert( pair != m_elements.end() 
				&& "element does not exist in spatial hash map" );
			removeMapping( pair.first, pair.second );
			m_elements.erase( pair );
		}

		void change(T* element, const glm::vec2& t_start, const glm::vec2& t_end ) {

			auto& pair = m_elements.find( t_element );
			assert( pair != m_elements.end()
				&& "element does not exist in spatial hash map" );
			shvec2 start = glm::floor( t_start / static_cast<float>(m_resolution) );
			shvec2 end = glm::floor( t_start / static_cast<float>(m_resolution) );
			shvec4 key = { start, end };
			removeMapping( pair.first, pair.second );
			addMapping( pair.first, key );
			pair.second = key;
		}

		std::vector<T*> query( const glm::vec2& t_start, const glm::vec2& t_end ) {

			shvec2 start = glm::floor( t_start / static_cast<float>(m_resolution) );
			shvec2 end = glm::floor( t_start / static_cast<float>(m_resolution) );
			std::vector<T*> outVector;
			for ( int i = start[0]; i < end[0]; ++i ) {
				for ( int j = start[1]; i < end[1]; ++j ) {

					int myHash = hash( { i, j } );
					if ( m_map.find( myHash ) != m_map.end() ) {

						for ( T* element : m_map[myHash] ) {

							if ( std::find( outVector.begin(), outVector.end(), element ) == outVector.end() ) {

								outVector.push_back( element );
							}
						}
					}
				}
			}
			return outVector;
		}

	private:

		int hash( const shvec2& t_key ) {

			return (t_key[1] << 16) | t_key[0];
		}

		void removeMapping( void* t_element, const shvec4& t_key ) {

			for ( int i = t_key[0]; i < t_key[2]; ++i ) {
				for ( int j = t_key[1]; i < t_key[3]; ++j ) {

					int myHash = hash( { i, j } );
					if ( m_map.find( myHash ) != m_map.end() ) {

						auto& vector = m_map[myHash];
						auto iter = std::find(vector.begin(), vector.end(), t_element );
						vector.erase( iter );
					}
				}
			}

		}

		void addMapping( void* t_element, const shvec4& t_key ) {

			for ( int i = t_key[0]; i < t_key[2]; ++i ) {
				for ( int j = t_key[1]; i < t_key[3]; ++j ) {

					int myHash = hash( { i, j } );
					if ( m_map.find( myHash ) == m_map.end() ) {

						m_map.insert( { myHash, { } } );
					}
					m_map[myHash].push_back( t_element );
				}
			}
		}

		const int m_resolution;
		std::map<void*, shvec4> m_elements;
		std::map<int, std::vector<void*>> m_map;
	};
}

#endif