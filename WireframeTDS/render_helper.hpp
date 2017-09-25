
// render_helper.hpp

// Header file for the rendering helper class.

#ifndef WIREFRAMETDS_RENDER_HELPER_HPP
#define WIREFRAMETDS_RENDER_HELPER_HPP

#include "wireframe_tds.hpp"
#include <vector>

namespace wireframe_tds {

	class RenderHelper {
	private:
		RenderHelper(
			) {

		}

		void* m_renderingContext{ nullptr };
	public:
		
		// lazy singleton

		RenderHelper( RenderHelper const& ) = delete;
		void operator=( RenderHelper const& ) = delete;
		static RenderHelper& getInstance(
			) {
			static RenderHelper instance;
			return instance;
		}

		// static initialize graphical api function

		static int initializeApi(
			);

		// static finalize graphical api function

		static int finalizeApi(
			);

		// static create vertex array object function

		static int createVertexArrayObjects(
			const unsigned int t_count,
			unsigned int*& t_ids
			);

		// static delete vertex array object function

		static int deleteVertexArrayObjects(
			const unsigned int t_count,
			unsigned int*& t_ids
			);

		// static create graphical buffers function

		static int createBuffers(
			const unsigned int t_count,
			unsigned int*& t_ids
			);

		// static delete graphical buffers function

		static int deleteBuffers(
			const unsigned int t_count,
			unsigned int*& t_ids
			);

		// static fill graphical buffer function

		static int fillBuffer(
			const unsigned int t_bufferId,
			const void* t_dataPointer,
			const unsigned int t_dataLength,
			const unsigned int t_usage
			);

		// static create graphical shader function

		static int createShader(
			const std::string t_source,
			unsigned int t_type,
			unsigned int& t_id,
			std::string& t_errorString
			);


		// static delete graphical shader function

		static int deleteShader(
			unsigned int t_id
			);

		// static create graphical program from shaders function

		static int createProgram(
			unsigned int t_vertexShaderId,
			unsigned int t_fragmentShaderId,
			unsigned int& t_id,
			std::string& t_errorString
			);

		// static create graphical program from shader with bound attribute locations function

		static int createProgram(
			unsigned int t_vertexShaderId,
			unsigned int t_fragmentShaderId,
			const std::vector<std::pair<char*, unsigned int>> t_attributeNames,
			unsigned int& t_id,
			std::string& t_errorString
			);

		// static create graphical program from shader contents function

		static int createProgram(
			const std::string t_vertexShaderSource,
			const std::string t_fragmentShaderSource,
			unsigned int& t_programId,
			std::string& t_errorString
			);

		// static create graphical program from shader contents with bound attribute locations function
		
		static int createProgram(
			const std::string t_vertexShaderSource,
			const std::string t_fragmentShaderSource,
			const std::vector<std::pair<char*, unsigned int>> t_attributeNames,
			unsigned int& t_programId,
			std::string& t_errorString
			);
	};
}

#endif