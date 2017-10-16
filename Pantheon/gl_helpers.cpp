
// gl_helpers.cpp

// soon to be deprecated in favour of gl_objects

#include "gl_helpers.hpp"

using namespace pantheon;

int pantheon::createVertexArrayObjects(
	const unsigned int t_count,
	unsigned int*& t_ids
	) {

	// assert count is more than zero and ids are nullptr

	assert( t_count > 0 );
	assert( t_ids == nullptr );

	t_ids = new unsigned int[t_count];
	glGenVertexArrays( t_count, &t_ids[0] );

	return PANTHEON_OK;
}

// static delete vertex array object function

int pantheon::deleteVertexArrayObjects(
	const unsigned int t_count,
	unsigned int*& t_ids
	) {

	// assert count is more than zero and ids are not nullptr

	assert( t_count > 0 );
	assert( t_ids != nullptr );

	glDeleteVertexArrays( t_count, t_ids );
	delete[] t_ids;
	t_ids = nullptr;

	return PANTHEON_OK;
}

// static create graphical buffers function

int pantheon::createBuffers(
	const unsigned int t_count,
	unsigned int*& t_ids
	) {

	// assert count is more than zero and ids are nullptr

	assert( t_count > 0 );
	assert( t_ids == nullptr );

	t_ids = new unsigned int[t_count];
	glGenBuffers( t_count, t_ids );

	return PANTHEON_OK;
}

// static delete graphical buffers function

int pantheon::deleteBuffers(
	const unsigned int t_count,
	unsigned int*& t_ids
	) {

	// assert count is more than zero and ids are not nullptr

	assert( t_count > 0 );
	assert( t_ids != nullptr );

	glDeleteBuffers( t_count, t_ids );
	delete[] t_ids;
	t_ids = nullptr;

	return PANTHEON_OK;
}

// static fill graphical buffer function

int pantheon::fillBuffer(
	const unsigned int t_id,
	const void* t_dataPointer,
	const unsigned int t_dataLength,
	const unsigned int t_usage
	) {

	// assert pointer is not nullptr and data length is more than 0

	assert( t_dataPointer != nullptr );
	assert( t_dataLength > 0 );

	glBindBuffer( GL_ARRAY_BUFFER, t_id );
	glBufferData( GL_ARRAY_BUFFER, t_dataLength, t_dataPointer, t_usage );
	glBindBuffer( GL_ARRAY_BUFFER, NULL );

	return PANTHEON_OK;

}

// static create graphical shader function

int pantheon::createShader(
	const std::string t_source,
	const unsigned int t_type,
	unsigned int& t_id,
	std::string& t_errorString
	) {

	// assert source length is more than 0

	assert( t_source.length() > 0 );

	// create shader id

	t_id = glCreateShader( t_type );

	// convert string to char array and length

	const char* shaderSource{ t_source.c_str() };
	int shaderLength = strlen( shaderSource );

	// compile shader

	glShaderSource( t_id, 1, &shaderSource, &shaderLength );
	glCompileShader( t_id );

	// if compile failed delete shader, show an error message and return error

	int success{ 0 };
	glGetShaderiv( t_id, GL_COMPILE_STATUS, &success );
	if ( !success ) {

		int length{ 0 };
		glGetShaderiv( t_id, GL_INFO_LOG_LENGTH, &length );
		t_errorString.resize( length );
		glGetShaderInfoLog( t_id, length, NULL, &t_errorString[0] );
		glDeleteShader( t_id );
		return PANTHEON_ERROR;
	}

	return PANTHEON_OK;
}


// static delete graphical shader function

int pantheon::deleteShader(
	unsigned int t_id
	) {

	glDeleteShader( t_id );
	t_id = 0;

	return PANTHEON_OK;
}

// static create graphical program from shaders function

int pantheon::createProgram(
	const unsigned int t_vertexShaderId,
	const unsigned int t_fragmentShaderId,
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

int pantheon::createProgram(
	const std::string t_vertexShaderSource,
	const std::string t_fragmentShaderSource,
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
		vertexShader, fragmentShader, t_programId, errorString );

	// if is error delete shaders, output errorstring and return error

	if ( isError ) {

		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		t_errorString = errorString;
		return PANTHEON_ERROR;
	}

	return PANTHEON_OK;
}
