
// sprite_renderer.cpp

// source file for sprite renderer class

#include "sprite_renderer.hpp"

#include "gl_helpers.hpp"
#include "helpers.hpp"

using namespace pantheon;

class SpriteRenderer::SpriteRendererImpl {

	friend class SpriteRenderer;

	SpriteRendererImpl() {

		// load program

		std::string vertexShader;
		std::string fragmentShader;
		int isError = getFileAsString( "shaders/sprite_vs_120.glsl", vertexShader );
		assert( !isError && "Could not open vertex shader" );
		isError = getFileAsString( "shaders/sprite_fs_120.glsl", fragmentShader );
		assert( !isError && "Could not open fragment shader" );
		std::string errorString;
		isError = createProgram( vertexShader, fragmentShader, m_programId, errorString );
		if ( isError ) {

			printf( (errorString + "\n").c_str() );
			assert( false && "Could not create program" );
		}

		// create buffers

		glGenBuffers( 1, &m_bufferId );
		glGenVertexArrays( 1, &m_vertexArrayId );

		// define array

		glBindVertexArray( m_vertexArrayId );
		glBindBuffer( GL_ARRAY_BUFFER, m_bufferId );
		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 32, (void*)(0) );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 32, (void*)(12) );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 32, (void*)(12) );
		glBindVertexArray( NULL );
	}

	~SpriteRendererImpl() {

	}

	void render() {

	}

	unsigned int m_bufferId;
	unsigned int m_vertexArrayId;
	unsigned int m_programId;
	std::map<std::string, unsigned int> m_textures;
};

SpriteRenderer::SpriteRenderer( ConstructRendererPermit& t_permit ) 
	: IGameRenderer{ t_permit } {

	m_renderer = new SpriteRendererImpl();
}

SpriteRenderer::~SpriteRenderer() {

	delete m_renderer;
}

void pantheon::SpriteRenderer::beforeActorsDraw() {

}

void pantheon::SpriteRenderer::afterActorsDraw() {

}

void SpriteRenderer::render() {

	m_renderer->render();
}

IGameRenderer* SpriteRenderer::createInstance( ConstructRendererPermit& t_permit ) {

	return new SpriteRenderer( t_permit );
}