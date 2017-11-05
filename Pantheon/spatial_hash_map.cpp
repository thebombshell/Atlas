
// spatial_hash_map.cpp

#include "spatial_hash_map.hpp"

#include <algorithm>
#include <map>

using namespace pantheon;

class SpatialHashMap2D::SpatialHashMap2DImpl {

	friend class SpatialHashMap2D;

	SpatialHashMap2DImpl() : m_resolution{ 64 * 0x0000ffff }, m_sectorSize{ 64 } {

	}

	SpatialHashMap2DImpl( unsigned int t_resolution ) : m_resolution{ t_resolution }, m_sectorSize{ (m_resolution * 2) / 0x0000ffff } {

	}

	~SpatialHashMap2DImpl() {

	}

	unsigned int hash( const glm::vec2& t_position ) {

		unsigned int x = t_position[0] <= -static_cast<int>(m_resolution)
			? 0 : (t_position[0] >= m_resolution
				? 0x0000ffff : static_cast<unsigned int>(floor( (t_position[0] + m_resolution) / m_sectorSize )));
		unsigned int y = t_position[1] <= -static_cast<int>(m_resolution)
			? 0 : (t_position[1] >= m_resolution
				? 0x0000ffff : static_cast<unsigned int>(floor( (t_position[1] + m_resolution) / m_sectorSize )));
		return x | (y << 16);
	}

	unsigned int makeHash( unsigned int t_x, unsigned int t_y ) {

		return t_x | (t_y << 16);
	}

	unsigned int getHashX( unsigned int t_hash ) {

		return t_hash & 0x0000ffff;
	}
	unsigned int getHashY( unsigned int t_hash ) {

		return (t_hash & 0xffff0000) >> 16;
	}

	bool has( void* t_entry ) {

		return m_entries.find( t_entry ) != m_entries.end();
	}

	void add( void* t_entry, const glm::vec2& t_min, const glm::vec2& t_max ) {

		assert( !has( t_entry ) && "Entry does not exist in hash map" );
		unsigned int min = hash( t_min );
		unsigned int max = hash( t_max );
		if ( min == max ) {

			m_entries.insert( { t_entry, { min, max } } );
			if ( m_map.find( min ) == m_map.end() ) {

				m_map.insert( { min, { } } );
			}
			m_map[min].push_back( t_entry );
			return;
		}
		glm::u32vec2 begin{ getHashX( min ), getHashY( min ) };
		glm::u32vec2 end{ getHashX( max ), getHashY( max ) };
		m_entries.insert( { t_entry, { min, max } } );
		for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
			for ( unsigned int j = begin[1]; j <= end[1]; ++j ) {

				unsigned int hashValue = makeHash( i, j );
				if ( m_map.find( hashValue ) == m_map.end() ) {

					m_map.insert( { hashValue, { } } );
				}
				m_map[hashValue].push_back( t_entry );
			}
		}
	}

	void move( void* t_entry, const glm::vec2& t_min, const glm::vec2& t_max ) {

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

				unsigned int hashValue = makeHash( i, j );
				std::vector<void*>& sector = m_map[hashValue];
				auto iter = std::find( sector.begin(), sector.end(), t_entry );
				sector.erase( iter );
				if ( sector.size() <= 0 ) {

					m_map.erase( hashValue );
				}
			}
		}
		begin = { getHashX( min ), getHashY( min ) };
		end = { getHashX( max ), getHashY( max ) };
		for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
			for ( unsigned int j = begin[1]; j <= end[1]; ++j ) {

				unsigned int hashValue = makeHash( i, j );
				if ( m_map.find( hashValue ) == m_map.end() ) {

					m_map.insert( { hashValue, { } } );
				}
				m_map[hashValue].push_back( t_entry );
			}
		}
	}

	void remove( void* t_entry ) {

		assert( has( t_entry ) && "Entry does not exist in hash map" );
		unsigned int min = m_entries[t_entry][0];
		unsigned int max = m_entries[t_entry][1];
		if ( min == max ) {

			m_entries.erase( t_entry );
			std::vector<void*>& sector = m_map[min];
			auto iter = std::find( sector.begin(), sector.end(), t_entry );
			sector.erase( iter );
			if ( sector.size() <= 0 ) {

				m_map.erase( min );
			}
			return;
		}
		glm::u32vec2 begin{ getHashX( min ), getHashY( min ) };
		glm::u32vec2 end{ getHashX( max ), getHashY( max ) };
		m_entries.erase( t_entry );
		for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
			for ( unsigned int j = begin[1]; i <= end[1]; ++j ) {

				unsigned int hashValue = makeHash( i, j );
				std::vector<void*>& sector = m_map[hashValue];
				auto iter = std::find( sector.begin(), sector.end(), t_entry );
				sector.erase( iter );
				if ( sector.size() <= 0 ) {

					m_map.erase( hashValue );
				}
			}
		}
	}

	std::vector<void*> query( const glm::vec2& t_min, const glm::vec2& t_max ) {

		unsigned int min = hash( t_min );
		unsigned int max = hash( t_max );
		glm::u32vec2 begin{ getHashX( min ), getHashY( min ) };
		glm::u32vec2 end{ getHashX( max ), getHashY( max ) };
		std::vector<void*> output;
		for ( unsigned int i = begin[0]; i <= end[0]; ++i ) {
			for ( unsigned int j = begin[1]; j <= end[1]; ++j ) {

				unsigned int hashValue = makeHash( i, j );
				auto sector = m_map.find( hashValue );
				if ( sector != m_map.end() ) {

					output.insert( output.begin(), sector->second.begin(), sector->second.end() );
				}
			}
		}
		std::sort( output.begin(), output.end() );
		auto last = std::unique( output.begin(), output.end() );
		output.erase( last, output.end() );
		return output;
	}
	
	const unsigned int m_resolution;
	const unsigned int m_sectorSize;
	std::map<void*, glm::u32vec2> m_entries;
	std::map<unsigned int, std::vector<void*>> m_map;
};

///////////////////////////////////////////////////////////////////////////////
// SpatialHashMap2D
///////////////////////////////////////////////////////////////////////////////

SpatialHashMap2D::SpatialHashMap2D() {

	m_map = new SpatialHashMap2DImpl();
}

SpatialHashMap2D::SpatialHashMap2D(unsigned int t_resolution) {

	m_map = new SpatialHashMap2DImpl( t_resolution );
}

SpatialHashMap2D::~SpatialHashMap2D() {

	delete m_map;
}

unsigned int SpatialHashMap2D::hash( const glm::vec2& t_position ) {

	return m_map->hash( t_position );
}

unsigned int SpatialHashMap2D::makeHash( unsigned int t_x, unsigned int t_y ) {

	return m_map->makeHash( t_x, t_y );
}

unsigned int SpatialHashMap2D::getHashX( unsigned int t_hash ) {

	return m_map->getHashX( t_hash );
}

unsigned int SpatialHashMap2D::getHashY( unsigned int t_hash ) {

	return m_map->getHashY( t_hash );
}

bool SpatialHashMap2D::has( void* t_entry ) {

	return m_map->has( t_entry );
}

void SpatialHashMap2D::add( void* t_entry, const glm::vec2& t_min, const glm::vec2& t_max ) {

	m_map->add( t_entry, t_min, t_max );
}

void SpatialHashMap2D::move( void* t_entry, const glm::vec2& t_min, const glm::vec2& t_max ) {

	m_map->move( t_entry, t_min, t_max );
}


void SpatialHashMap2D::remove( void* t_entry ) {

	m_map->remove( t_entry );
}


std::vector<void*> SpatialHashMap2D::query( const glm::vec2& t_min, const glm::vec2& t_max ) {

	return m_map->query( t_min, t_max );
}
