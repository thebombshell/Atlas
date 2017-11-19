
// text_helper.hpp

#ifndef GAEA_TEXT_HELPER_HPP
#define GAEA_TEXT_HELPER_HPP

#include <pantheon.hpp>
#include <sprite_renderer.hpp>

namespace gaea {

	pthn::SpriteRendererMessage drawCharacter( char t_char, int t_offset, const glm::vec3& t_colour, const glm::mat3& t_matrix ) {

		switch ( t_char ) {

			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
				return pthn::SpriteRendererMessage{ "sprites", { -t_offset, 0.0f }, { (t_char - 'a') * 8.0f, 240.0f }, { 8.0f, 8.0f }, t_colour, 0.0f, t_matrix };
				break;
		}
		return pthn::SpriteRendererMessage{ "sprites", { -t_offset, 0.0f }, { 0.0f, 0.0f }, { 0.001f, 0.001f }, t_colour, 0.0f, t_matrix };
	}
	void drawText( const std::string& t_text, const glm::vec3& t_colour, const glm::mat3& t_matrix ) {

		pthn::SpriteRenderer& renderer = *pthn::Game::GetRendererAs<pthn::SpriteRenderer>();
		for ( unsigned int i = 0; i < t_text.size(); ++i ) {

			if ( t_text[i] >= 'a' && t_text[i] <= 'z' ) {
			
				renderer.queueDraw( drawCharacter( t_text[i], static_cast<int>(i), t_colour, t_matrix ) );
			}
		}
	}
}

#endif