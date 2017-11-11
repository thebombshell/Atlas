
// line_text_helpers.hpp

// header file for public implementation of a text drawing rendering helper

#ifndef ATLAS_TAG_HPP
#define ATLAS_TAG_HPP

#include <algorithm>
#include <pantheon.hpp>
#include <line_renderer.hpp>
#include <vector>

namespace atlas {

	std::vector<pantheon::LineRendererVertex> charToLines( char t_char, int t_offset );

	std::vector<pantheon::LineRendererVertex> charToLines( char t_char, int t_offset, const glm::vec3& t_pos );

	std::vector<pantheon::LineRendererVertex> charToLines( char t_char, int t_offset, const glm::vec3& t_pos, const glm::vec3& t_col );

	pantheon::LineRendererMessage stringToLines( std::string t_string );

	pantheon::LineRendererMessage stringToLines( std::string t_string, const glm::vec3& t_position );

	pantheon::LineRendererMessage stringToLines( std::string t_string, const glm::vec3& t_position, const glm::vec3& t_colour );

	template<typename T_POS_FUNC, typename T_COL_FUNC>
	pantheon::LineRendererMessage stringToLines( std::string t_string, T_POS_FUNC t_positionFunc, T_COL_FUNC t_colourFunc ) {

		pantheon::LineRendererMessage message{ };
		int offset{ 0 };
		for ( auto character : t_string ) {

			auto  charToLines( character, offset, t_positionFunc( character, offset ), t_colourFunc( character, offset ) );
			if ( vertices.size() > 0 ) {

				message.pushVertices( vertices.begin(), vertices.end() );
			}
			++offset;
		}
		return message;
	}
}

#endif

