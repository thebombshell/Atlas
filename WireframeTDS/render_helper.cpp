
// render_helper.cpp

// Source file for the rendering class

#include "render_helper.hpp"
#include "error.hpp"
#include <assert.h>

// gl includes.

#define GLEW_DYNAMIC
#include <glew.h>
#include <wglew.h>
//#include <freeglut.h>

using namespace wireframe_tds;

#ifdef NDEBUG

bool g_isInitialized = false;

#endif

// static initialize graphical api function

int RenderHelper::initializeApi(
	) {

	// initialize glew

	GLenum glError = glewInit();
	if ( glError != GLEW_OK ) {

		return WTDS_ERROR_COULT_NOT_INIT_GL;
	}

	// if gl version 3.2.0 is not supported return error

	if ( !glewIsSupported( "GL_VERSION_3_2" ) ) {

		return WTDS_ERROR_GL_VERSION_NOT_SUPPORTED;
	}

	// set up specifically versioned rendering context

	const int attribList[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	HDC displayContext = wglGetCurrentDC();
	RenderHelper& singleton = RenderHelper::getInstance();
	singleton.m_renderingContext = static_cast<void*>( wglCreateContextAttribsARB(
		displayContext, NULL, attribList ) );
	wglMakeCurrent( 
		displayContext, static_cast<HGLRC>( singleton.m_renderingContext ) );

	// glew is to be initialized after every context change

	glError = glewInit();
	if ( glError != GLEW_OK ) {

		return WTDS_ERROR_COULT_NOT_INIT_GL;
	}

	// set up gl state

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LESS );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	//

	return WTDS_ERROR_OK;
}

// static finalize graphical api function

int RenderHelper::finalizeApi(
	) {

	// clean up rendering context

	wglMakeCurrent( NULL, NULL );
	RenderHelper& singleton = RenderHelper::getInstance();
	wglDeleteContext( static_cast<HGLRC>(singleton.m_renderingContext) );
	singleton.m_renderingContext = nullptr;


	return WTDS_ERROR_OK;
}

// static create vertex array object function

int RenderHelper::createVertexArrayObjects(
	const unsigned int t_count,
	unsigned int*& t_ids
	) {
	
	// assert count is more than zero and ids are nullptr

	assert( t_count > 0 );
	assert( t_ids == nullptr );

	t_ids = new unsigned int[t_count];
	glGenVertexArrays( t_count, &t_ids[0] );

	return WTDS_ERROR_OK;
}

// static delete vertex array object function

int RenderHelper::deleteVertexArrayObjects(
	const unsigned int t_count,
	unsigned int*& t_ids
	) {

	// assert count is more than zero and ids are not nullptr

	assert( t_count > 0 );
	assert( t_ids != nullptr );

	glDeleteVertexArrays( t_count, t_ids );
	delete[] t_ids;
	t_ids = nullptr;

	return WTDS_ERROR_OK;
}

// static create graphical buffers function

int RenderHelper::createBuffers(
	const unsigned int t_count,
	unsigned int*& t_ids
	) {

	// assert count is more than zero and ids are nullptr

	assert( t_count > 0 );
	assert( t_ids == nullptr );

	t_ids = new unsigned int[t_count];
	glGenBuffers( t_count, t_ids );

	return WTDS_ERROR_OK;
}

// static delete graphical buffers function

int RenderHelper::deleteBuffers(
	const unsigned int t_count,
	unsigned int*& t_ids
	) {

	// assert count is more than zero and ids are not nullptr

	assert( t_count > 0 );
	assert( t_ids != nullptr );

	glDeleteBuffers( t_count, t_ids );
	delete[] t_ids;
	t_ids = nullptr;

	return WTDS_ERROR_OK;
}

// static fill graphical buffer function

int RenderHelper::fillBuffer(
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

	return WTDS_ERROR_OK;

}

// static create graphical shader function

int RenderHelper::createShader(
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
		return WTDS_ERROR_SHADER_COMPILE;
	}

	return WTDS_ERROR_OK;
}


// static delete graphical shader function

int RenderHelper::deleteShader(
	unsigned int t_id
	) {

	glDeleteShader( t_id );
	t_id = 0;

	return WTDS_ERROR_OK;
}

// static create graphical program from shaders function

int RenderHelper::createProgram(
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
		return WTDS_ERROR_PROGRAM_LINK;
	}

	return WTDS_ERROR_OK;
}

// static create graphical program from shader with bound attribute locations function

int RenderHelper::createProgram(
	const unsigned int t_vertexShaderId,
	const unsigned int t_fragmentShaderId,
	const std::vector<std::pair<char*, unsigned int>> t_attributeNames,
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
		return WTDS_ERROR_PROGRAM_LINK;
	}

	return WTDS_ERROR_OK;
}

// static create graphical program from shader contents function

int RenderHelper::createProgram(
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
	isError = RenderHelper::createShader( 
		t_vertexShaderSource, GL_VERTEX_SHADER, vertexShader, errorString );

	// if is error output errorstring and return error

	if ( isError ) {

		t_errorString = errorString;
		return WTDS_ERROR_VERTEX_SHADER_COMPILE;
	}

	// create fragment shader

	unsigned int fragmentShader;
	isError = RenderHelper::createShader( 
		t_fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader, errorString );

	// if is error delete vertes shader, output errorstring and return error

	if ( isError ) {

		glDeleteShader( vertexShader );
		t_errorString = errorString;
		return WTDS_ERROR_FRAGMENT_SHADER_COMPILE;
	}

	isError = RenderHelper::createProgram( 
		vertexShader, fragmentShader, t_programId, errorString );

	// if is error delete shaders, output errorstring and return error

	if ( isError ) {

		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		t_errorString = errorString;
		return WTDS_ERROR_PROGRAM_LINK;
	}

	return WTDS_ERROR_OK;
}

// static create graphical program from shader contents with bound attribute locations function

int RenderHelper::createProgram(
	const std::string t_vertexShaderSource,
	const std::string t_fragmentShaderSource,
	const std::vector<std::pair<char*, unsigned int>> t_attributeNames,
	unsigned int& t_programId,
	std::string& t_errorString
	) {

	// set up error variable

	int isError{ 0 };
	std::string errorString{ "" };

	// create vertex shader

	unsigned int vertexShader;
	isError = RenderHelper::createShader(
		t_vertexShaderSource, GL_VERTEX_SHADER, vertexShader, errorString );

	// if is error output errorstring and return error

	if ( isError ) {

		t_errorString = errorString;
		return WTDS_ERROR_VERTEX_SHADER_COMPILE;
	}

	// create fragment shader

	unsigned int fragmentShader;
	isError = RenderHelper::createShader(
		t_fragmentShaderSource, GL_FRAGMENT_SHADER, fragmentShader, errorString );

	// if is error delete vertes shader, output errorstring and return error

	if ( isError ) {

		glDeleteShader( vertexShader );
		t_errorString = errorString;
		return WTDS_ERROR_FRAGMENT_SHADER_COMPILE;
	}

	isError = RenderHelper::createProgram(
		vertexShader, fragmentShader, t_attributeNames, t_programId, errorString );

	// if is error delete shaders, output errorstring and return error

	if ( isError ) {

		glDeleteShader( vertexShader );
		glDeleteShader( fragmentShader );
		t_errorString = errorString;
		return WTDS_ERROR_PROGRAM_LINK;
	}

	return WTDS_ERROR_OK;
}

#undef SINGLETON