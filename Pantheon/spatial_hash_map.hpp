
// spatial_hash_map.hpp

#ifndef PANTHEON_SPATIAL_HASH_MAP_HPP
#define PANTHEON_SPATIAL_HASH_MAP_HPP

#include "pantheon.hpp"

#include <vector>

namespace pantheon {

	class SpatialHashMap2D {

	public:

		SpatialHashMap2D(); 
		SpatialHashMap2D( unsigned int t_resolution );

		~SpatialHashMap2D();

		unsigned int hash( const glm::vec2& t_position );

		unsigned int makeHash( unsigned int t_x, unsigned int t_y );

		unsigned int getHashX( unsigned int t_hash );

		unsigned int getHashY( unsigned int t_hash );

		bool has( void* t_entry );

		void add( void* t_entry, const glm::vec2& t_min, const glm::vec2& t_max );

		void move( void* t_entry, const glm::vec2& t_min, const glm::vec2& t_max );

		void remove( void* t_entry );

		std::vector<void*> query( const glm::vec2& t_min, const glm::vec2& t_max );

	private:

		class SpatialHashMap2DImpl;
		SpatialHashMap2DImpl* m_map;
	};
}

#endif