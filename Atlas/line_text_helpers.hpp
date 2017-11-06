
// line_text_helpers.hpp

// header file for public implementation of a text drawing rendering helper

#ifndef ATLAS_TAG_HPP
#define ATLAS_TAG_HPP

#include <algorithm>
#include <pantheon.hpp>
#include <line_renderer.hpp>
#include <vector>

namespace atlas {

	std::vector<pantheon::LineRendererVertex> charToLines( char t_char, int t_offset ) {

		float offset = t_offset * 6.0f;
		switch ( t_char ) {
			case 'a':
				return {
					{ offset + 3.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'b':
				return {
					{ offset + 0.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'c':
				return {
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'd':
				return{
					{ offset + 4.0f, 5.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 3.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'e':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'f':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'g':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'h':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'i':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'j':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'k':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'l':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'm':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'n':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'o':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'p':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'q':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'r':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case's':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case't':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'u':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'v':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'w':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'x':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'y':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'z':
				return{
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'0':
				return{
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'1':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'2':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'3':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'4':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'5':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'6':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'7':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'8':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() }
				};
			case'9':
				return{
					{ offset + 1.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 4.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ offset + 4.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
					{ pantheon::LineRendererVertex::separator() },
					{ pantheon::LineRendererVertex::separator() }
				};
		}
		return { };
	}

	pantheon::LineRendererMessage stringToLines( std::string t_string ) {

		pantheon::LineRendererMessage message{ };
		int offset{ 0 };
		for( auto character : t_string ) {

			auto vertices = charToLines( character, offset );
			if ( vertices.size() > 0 ) {

				message.pushVertices( vertices.begin(), vertices.end() );
			}
			++offset;
		}
		return message;
	}
}

#endif

