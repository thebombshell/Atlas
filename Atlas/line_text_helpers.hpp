
// line_text_helpers.hpp

// header file for public implementation of a text drawing rendering helper

#ifndef ATLAS_TAG_HPP
#define ATLAS_TAG_HPP

#include <algorithm>
#include <pantheon.hpp>
#include <line_renderer.hpp>
#include <vector>

namespace atlas {

	std::vector<pantheon::LineRendererVertex> charToLines( char t_char, int t_offset, const glm::vec3& t_pos, const glm::vec3& t_col ) {

		float offset = t_offset * 6.0f;
		std::vector<pantheon::LineRendererVertex> vertices;

		switch ( t_char ) {
			case 'a':
				vertices = {
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'b':
				vertices = {
					{ offset + 3.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'c':
				vertices = {
					{ offset + 4.0f, 1.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'd':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 1.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 1.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'e':
				vertices ={
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'f':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'g':
				vertices ={
					{ offset + 2.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'h':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'i':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 2.0f, 1.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'j':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'k':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 3.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'l':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'm':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 2.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'n':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'o':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'p':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'q':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 3.0f, 1.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'r':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case's':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case't':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 2.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'u':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'v':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'w':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 2.0f, 1.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'x':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'y':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 2.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'z':
				vertices ={
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 1.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'0':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'1':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 3.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'2':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'3':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'4':
				vertices ={
					{ offset + 3.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 1.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 1.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'5':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'6':
				vertices ={
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'7':
				vertices ={
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'8':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 1.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'9':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'.':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.01f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'&':
				vertices ={
					{ offset + 1.0f, 4.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 1.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 3.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 2.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 4.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.01f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 4.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.01f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
			case'!':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.01f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
				};
				break;
			case'?':
				vertices ={
					{ offset + 0.0f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.01f, 0.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() },
					{ offset + 0.0f, 2.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 3.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 4.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ offset + 0.0f, 5.0f, 0.0f, t_col[0], t_col[1], t_col[2] },
					{ pantheon::LineRendererVertex::separator() }
				};
				break;
		}
		for ( auto& vertex : vertices ) {

			vertex.position[0] += t_pos[0];
			vertex.position[1] += t_pos[1];
			vertex.position[2] += t_pos[2];
		}
		return vertices;
	}

	pantheon::LineRendererMessage stringToLines( std::string t_string ) {

		pantheon::LineRendererMessage message{ };
		int offset{ 0 };
		for ( auto character : t_string ) {

			auto vertices = charToLines( character, offset, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } );
			if ( vertices.size() > 0 ) {

				message.pushVertices( vertices.begin(), vertices.end() );
			}
			++offset;
		}
		return message;
	}

	pantheon::LineRendererMessage stringToLines( std::string t_string, const glm::vec3& t_colour ) {

		pantheon::LineRendererMessage message{ };
		int offset{ 0 };
		for ( auto character : t_string ) {

			auto vertices = charToLines( character, offset, { 0.0f, 0.0f, 0.0f }, t_colour );
			if ( vertices.size() > 0 ) {

				message.pushVertices( vertices.begin(), vertices.end() );
			}
			++offset;
		}
		return message;
	}

	pantheon::LineRendererMessage stringToLines( std::string t_string, const glm::vec3& t_position, const glm::vec3& t_colour ) {

		pantheon::LineRendererMessage message{ };
		int offset{ 0 };
		for ( auto character : t_string ) {

			auto vertices = charToLines( character, offset, t_position, t_colour );
			if ( vertices.size() > 0 ) {

				message.pushVertices( vertices.begin(), vertices.end() );
			}
			++offset;
		}
		return message;
	}

	template<typename T_POS_FUNC, typename T_COL_FUNC>
	pantheon::LineRendererMessage stringToLines( std::string t_string, T_POS_FUNC t_positionFunc, T_COL_FUNC t_colourFunc ) {

		pantheon::LineRendererMessage message{ };
		int offset{ 0 };
		for ( auto character : t_string ) {

			auto vertices = charToLines( character, offset, t_positionFunc( character, offset ), t_colourFunc( character, offset ) );
			if ( vertices.size() > 0 ) {

				message.pushVertices( vertices.begin(), vertices.end() );
			}
			++offset;
		}
		return message;
	}
}

#endif

