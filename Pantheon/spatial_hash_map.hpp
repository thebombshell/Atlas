
// spatial_hash_map.hpp

#ifndef PANTHEON_SPATIAL_HASH_MAP_HPP
#define PANTHEON_SPATIAL_HASH_MAP_HPP

#include "pantheon.hpp"

#include <map>
#include <vector>

namespace pantheon {

	template<typename T>
	class SpatialHashMap2D {

	public:

		SpatialHashMap2D() {

			m_map.resize( 0xff );
		}

		~SpatialHashMap2D() {

		}

		unsigned int hash( const glm::vec2& t_position ) {

			unsigned int x = t_position[0] <= -1024.0f
				? 0 : (t_position[0] >= 1024.0f
					? 15 : static_cast<unsigned int>(floor( (t_position[0] + 1024.0f) / 128.0f )));
			unsigned int y = t_position[1] <= -1024.0f
				? 0 : (t_position[1] >= 1024.0f
					? 15 : static_cast<unsigned int>(floor( (t_position[1] + 1024.0f) / 128.0f )));
			return x | (y << 4);
		}

		unsigned int makeHash( unsigned int t_x, unsigned int t_y ) {

			return t_x | (t_y << 4);
		}

		unsigned int getHashX( unsigned int t_hash ) {

			return t_hash & 0xf;
		}
		unsigned int getHashY( unsigned int t_hash ) {

			return (t_hash & 0xf0) >> 4;
		}

		bool has( T* t_entry ) {

			return m_entries.find( t_entry ) != m_entries.end();
		}

		void add( T* t_entry, const glm::vec2& t_min, const glm::vec2& t_max ) {

			assert( !has( t_entry ) && "Entry does not exist in hash map" );
			unsigned int min = hash( t_min );
			unsigned int max = hash( t_max );
			if ( min == max ) {

				m_entries.insert( { t_entry, { min, max } } );
				m_map[min].push_back( t_entry );
				return;
			}
			glm::u32vec2 begin{ getHashX( min ), getHashY( min ) };
			glm::u32vec2 end{ getHashX( max ), getHashY( max ) };
			m_entries.insert( { t_entry, { min, max } } );
			for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
				for ( unsigned int j = begin[1]; j <= end[1]; ++j ) {

					m_map[makeHash( i, j )].push_back( t_entry );
				}
			}
		}

		void move(T* t_entry, const glm::vec2& t_min, const glm::vec2& t_max ) {

			assert( has( t_entry ) && "Entry does not exist in hash map" );
			auto pair = m_entries.find( t_entry );
			unsigned int oldMin = m_entries[t_entry][0];
			unsigned int oldMax = m_entries[t_entry][1];
			unsigned int min = hash( t_min );
			unsigned int max = hash( t_max );
			pair->second = { min, max };
			glm::u32vec2 begin{ getHashX( oldMin ), getHashY( oldMin ) };
			glm::u32vec2 end{ getHashX( oldMax ), getHashY( oldMax ) };
			for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
				for ( unsigned int j = begin[1]; j <= end[1]; ++j ) {
					std::vector<T*>& sector = m_map[makeHash( i, j )];
					auto iter = std::find( sector.begin(), sector.end(), t_entry );
					sector.erase( iter );
				}
			}
			begin = { getHashX( min ), getHashY( min ) };
			end = { getHashX( max ), getHashY( max ) };
			for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
				for ( unsigned int j = begin[1]; j <= end[1]; ++j ) {

					m_map[makeHash( i, j )].push_back( t_entry );
				}
			}
		}

		void remove( T* t_entry ) {

			assert( has( t_entry ) && "Entry does not exist in hash map" );
			unsigned int min = m_entries[t_entry][0];
			unsigned int max = m_entries[t_entry][1];
			if ( min == max ) {

				m_entries.erase( t_entry );
				std::vector<T*>& sector = m_map[min];
				auto iter = std::find( sector.begin(), sector.end(), t_entry );
				sector.erase( iter );
				return;
			}
			glm::u32vec2 begin{ getHashX( min ), getHashY( min ) };
			glm::u32vec2 end{ getHashX( max ), getHashY( max ) };
			m_entries.erase( t_entry );
			for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
				for ( unsigned int j = begin[1]; i <= end[1]; ++j ) {

					std::vector<T*>& sector = m_map[makeHash( i, j )];
					auto iter = std::find( sector.begin(), sector.end(), t_entry );
					sector.erase( iter );
				}
			}
		}

		std::vector<T*> query( const glm::vec2& t_min, const glm::vec2& t_max ) {

			unsigned int min = hash( t_min );
			unsigned int max = hash( t_max );
			glm::u32vec2 begin{ getHashX( min ), getHashY( min ) };
			glm::u32vec2 end{ getHashX( max ), getHashY( max ) };
			std::vector<T*> output;
			for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
				for ( unsigned int j = begin[1]; j <= end[1]; ++j ) {

					std::vector<T*>& sector = m_map[makeHash( i, j )];
					output.insert( output.begin(), sector.begin(), sector.end() );
				}
			}
			std::sort( output.begin(), output.end() );
			auto last = std::unique( output.begin(), output.end() );
			output.erase( last, output.end() );
			return output;
		}
	private:

		std::map<T*, glm::u32vec2> m_entries;
		std::vector<std::vector<T*>> m_map;
	};
}

#endif