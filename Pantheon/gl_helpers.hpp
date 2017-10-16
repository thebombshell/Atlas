
// gl_helpers.hpp

// Header file for a variety of gl helper functions to be used internally

#ifndef PANTHEON_GL_HELPERS_HPP
#define PANTHEON_GL_HELPERS_HPP

#include "pantheon.hpp"

#include <GL\glew.h>
#include <string>

namespace pantheon {

	// static create vertex array object function

	PANTHEON_API int createVertexArrayObjects(
		const unsigned int t_count,
		unsigned int*& t_ids
		);

	// static delete vertex array object function

	PANTHEON_API int deleteVertexArrayObjects(
		const unsigned int t_count,
		unsigned int*& t_ids
		);

	// static create graphical buffers function

	PANTHEON_API int createBuffers(
		const unsigned int t_count,
		unsigned int*& t_ids
		);

	// static delete graphical buffers function

	PANTHEON_API int deleteBuffers(
		const unsigned int t_count,
		unsigned int*& t_ids
		);

	// static fill graphical buffer function

	PANTHEON_API int fillBuffer(
		const unsigned int t_id,
		const void* t_dataPointer,
		const unsigned int t_dataLength,
		const unsigned int t_usage
		);

	// static create graphical shader function

	PANTHEON_API int createShader(
		const std::string t_source,
		const unsigned int t_type,
		unsigned int& t_id,
		std::string& t_errorString
		);


	// static delete graphical shader function

	PANTHEON_API int deleteShader(
		unsigned int t_id
		);

	// static create graphical program from shaders function

	PANTHEON_API int createProgram(
		const unsigned int t_vertexShaderId,
		const unsigned int t_fragmentShaderId,
		unsigned int& t_id,
		std::string& t_errorString
		);

	// static create graphical program from shader with bound attribute locations function

	template<typename T>
	int createProgram(
		const unsigned int t_vertexShaderId,
		const unsigned int t_fragmentShaderId,
		T t_attributeNames,
		unsigned int& t_id,
		std::string& t_errorString
		) {

		// set up error variable

		std::string errorString{ "" };

		// create program

		t_id = glCreateProgram();

		// attach shaders

		glAttachShader( t_id, t_vertexShaderId );
		glAttachShader( t_id, t_fragmentShaderId );

		// bind attribute names from pairs

		for ( auto iter = t_attributeNames.begin(); iter != t_attributeNames.end(); ++iter ) {

			glBindAttribLocation( t_id, iter->second, iter->first );
		}

		// link program

		glLinkProgram( t_id );


		// if link was not successful delete program, output errorstring and return error

		int success{ 0 };
		glGetProgramiv( t_id, GL_LINK_STATUS, &success );
		if ( success == GL_FALSE ) {

			int length{ 0 };
			glGetProgramiv( t_id, GL_INFO_LOG_LENGTH, &length );
			t_errorString.resize( length );
			glGetProgramInfoLog( t_id, length, NULL, &t_errorString[0] );
			glDeleteProgram( t_id );
			return PANTHEON_ERROR;
		}

		return PANTHEON_OK;
	}

	// static create graphical program from shader contents function

	PANTHEON_API int createProgram(
		const std::string t_vertexShaderSource,
		const std::string t_fragmentShaderSource,
		unsigned int& t_programId,
		std::string& t_errorString
		);

	// static create graphical program from shader contents with bound attribute locations function

	template<typename T>
	int createProgram(
		const std::string t_vertexShaderSource,
		const std::string t_fragmentShaderSource,
		T t_attributeNames,
		unsigned int& t_programId,
		std::string& t_errorString
		) {

		// set up error variable

		int isError{ 0 };
		std::string errorString{ "" };

		// create vertex shader

		unsigned int vertexShader;
		isError = createShader(
			t_vertexShaderSource, GL_VERTEX_SHADER, vertexShader, errorString );

		// if is error output errorstring and return error

		if ( isError ) {

			t_errorString = errorString;
			return PANTHEON_ERROR;
		}

		// create fragment shader

		unsigned int fragmentShader;
		isError = createShader(
			t_fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader, errorString );

		// if is error delete vertes shader, output errorstring and return error

		if ( isError ) {

			glDeleteShader( vertexShader );
			t_errorString = errorString;
			return PANTHEON_ERROR;
		}

		isError = createProgram(
			vertexShader, fragmentShader, t_attributeNames, t_programId, errorString );

		// if is error delete shaders, output errorstring and return error

		if ( isError ) {

			glDeleteShader( vertexShader );
			glDeleteShader( fragmentShader );
			t_errorString = errorString;
			return PANTHEON_ERROR;
		}

		return PANTHEON_OK;
	}

}

#endif // PANTHEON_GL_HELPERS_HPP