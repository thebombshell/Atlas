
// line_renderer.cpp

// source file for the line renderer class

#include "line_renderer.hpp"

#include "gl_objects.hpp"
#include "helpers.hpp"
#include "push_buffer.hpp"
#include "shapes_2d.hpp"

#include <algorithm>
#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <SDL.h>
#include <vector>

using namespace pantheon;

const unsigned int MAX_VERTEX_COUNT{ 32000 };

///////////////////////////////////////////////////////////////////////////////
// helper functions for generating the meshes
///////////////////////////////////////////////////////////////////////////////

// creates and pushes vertex in data array

void pushVertex( glm::vec2 t_point, const LineRendererVertex& t_vertex, PushBuffer<float>& t_buffer ) {

	float* ptr = glm::value_ptr( t_point );
	t_buffer.pushData( ptr, ptr + 2 );
	t_buffer.pushData( t_vertex.data + 2, t_vertex.data + 6 );
};

// find perpendicular vector

glm::vec2 findPerp( glm::vec2 t_vector ) {

	return{ t_vector[1], -t_vector[0] };
};

// find inner offset corner

glm::vec2 findInCorner( glm::vec2 t_pointA, glm::vec2 t_normalA
	, glm::vec2 t_pointB, glm::vec2 t_normalB, glm::vec2 t_corner ) {

	glm::vec2 point{ 0.0f, 0.0f };
	if ( findIntersectionOfRays
		( { t_pointA + findPerp( t_normalA ) * 0.5f, t_normalA }
			, { t_pointB + findPerp( t_normalB ) * 0.5f, -t_normalB }
			, point ) == PANTHEON_INTERSECTION_NOT_FOUND ) {

		point = t_corner + findPerp( t_normalA ) * 0.5f;
	}
	return point;
};

// find outer offset corner

glm::vec2 findOutCorner( glm::vec2 t_pointA, glm::vec2 t_normalA
	, glm::vec2 t_pointB, glm::vec2 t_normalB, glm::vec2 t_corner ) {

	glm::vec2 point{ 0.0f, 0.0f };
	if ( findIntersectionOfRays
		( { t_pointA - findPerp( t_normalA ) * 0.5f, t_normalA }
			, { t_pointB - findPerp( t_normalB ) * 0.5f, -t_normalB }
			, point ) == PANTHEON_INTERSECTION_NOT_FOUND ) {

		point = t_corner - findPerp( t_normalA ) * 0.5f;
	}
	return point;
};

// find both corners and push into data

void findAndPushCorners
( glm::vec2 t_start, glm::vec2 t_corner, glm::vec2 t_end
	, const LineRendererVertex& t_vertex, PushBuffer<float>& t_buffer ) {

	glm::vec2 normA = glm::normalize( t_start - t_corner );
	glm::vec2 normB = glm::normalize( t_corner - t_end );
	pushVertex
		( findInCorner( t_start, normA, t_end, normB, t_corner ), t_vertex, t_buffer );
	pushVertex
		( findOutCorner( t_start, normA, t_end, normB, t_corner ), t_vertex, t_buffer );
};

// find start corners and push into data

void findAndPushStartCorners
	( glm::vec2 t_corner, glm::vec2 t_end
	, const LineRendererVertex& t_vertex, PushBuffer<float>& t_buffer ) {

	glm::vec2 normA = glm::normalize( t_corner - t_end );
	glm::vec2 normB = -normA;
	glm::vec2 start = t_corner - normA;
	pushVertex
		( findInCorner( start, normA, t_end, normB, t_corner ) + normA * 0.5f, t_vertex, t_buffer );
	pushVertex
		( findOutCorner( start, normA, t_end, normB, t_corner ) + normA * 0.5f, t_vertex, t_buffer );
};

// find end corners and push into data

void findAndPushEndCorners
	( glm::vec2 t_start, glm::vec2 t_corner
	, const LineRendererVertex& t_vertex, PushBuffer<float>& t_buffer ) {

	glm::vec2 normA = glm::normalize( t_start - t_corner );
	glm::vec2 normB = -normA;
	glm::vec2 end = t_corner + normA;
	pushVertex
		( findInCorner( t_start, normA, end, normB, t_corner ) + normB * 0.5f, t_vertex, t_buffer );
	pushVertex
		( findOutCorner( t_start, normA, end, normB, t_corner ) + normB * 0.5f, t_vertex, t_buffer );
};

template<typename T>
void pushSequenceInBuffer( T t_begin, T t_end, PushBuffer<float>& t_buffer ) {

	int elementCount = t_end - t_begin;

	// offset to be filled in later

	t_buffer.pushData( t_buffer.pointer, t_buffer.pointer + 6 );

	// make up a previous point and find first corners

	{
		glm::vec2 start = glm::make_vec2( t_begin->position );
		glm::vec2 end = glm::make_vec2( (t_begin + 1)->position );
		findAndPushStartCorners( start, end, *t_begin, t_buffer );
	}

	// copy for degeneracy

	std::copy
		( t_buffer.pointer + (t_buffer.getIndex() - 12)
		, t_buffer.pointer + (t_buffer.getIndex() - 6)
		, t_buffer.pointer + (t_buffer.getIndex() - 18) );

	// for each vertex find and push corners

	T prev = t_begin;
	T next = t_begin + 2;

	if ( elementCount >= 3 ) {
		std::for_each( t_begin + 1, t_end - 1, [&]( const LineRendererVertex& t_vertex ) {

			findAndPushCorners
				( glm::make_vec2( prev->position )
					, glm::make_vec2( t_vertex.position )
					, glm::make_vec2( next->position )
					, t_vertex, t_buffer );

			clampIncrementIterator( prev, t_end );
			clampIncrementIterator( next, t_end );
		} );
	}

	// make up a next point and find first corners

	{
		glm::vec2 start = glm::make_vec2( (t_end - 2)->position );
		glm::vec2 end = glm::make_vec2( (t_end - 1)->position );
		findAndPushEndCorners( start, end, *(t_end - 1), t_buffer );
	}

	// copy for degeneracy

	t_buffer.pushData
		( t_buffer.pointer + (t_buffer.getIndex() - 6)
		, t_buffer.pointer + t_buffer.getIndex() );
}

template<typename T>
void pushCircuitInBuffer( T t_begin, T t_end, PushBuffer<float>& t_buffer ) {

	int startIndex = t_buffer.getIndex();

	// offset buffer by a vertex, to be filled late

	t_buffer.pushData( t_buffer.pointer, t_buffer.pointer + 6 );

	// for each vertex find and push corners

	T prev = t_end - 1;
	T next = t_begin + 1;

	std::for_each( t_begin, t_end, [&]( const LineRendererVertex& t_vertex ) {

		findAndPushCorners
			( glm::make_vec2( prev->position )
				, glm::make_vec2( t_vertex.position )
				, glm::make_vec2( next->position )
				, t_vertex, t_buffer );

		loopIncrementIterator( prev, t_begin, t_end );
		loopIncrementIterator( next, t_begin, t_end );
	} );

	// fill the offset from earlier with the first element for degeneracy

	std::copy
		( t_buffer.pointer + (startIndex + 6)
		, t_buffer.pointer + (startIndex + 12)
		, t_buffer.pointer + startIndex );

	// copy start of buffer to complete loop and pad for degeneracy

	t_buffer.pushData
		( t_buffer.pointer + (startIndex + 6)
		, t_buffer.pointer + (startIndex + 18) );
	t_buffer.pushData
		( t_buffer.pointer + (t_buffer.getIndex() - 6)
		, t_buffer.pointer + t_buffer.getIndex() );

}

void pushPointInBuffer
( const LineRendererVertex& t_vertex, PushBuffer<float>& t_buffer ) {

	glm::vec2 position = glm::make_vec2( t_vertex.position );
	glm::vec2 topLeft = position + glm::vec2( -0.5f, 0.5f );
	glm::vec2 topRight = position + glm::vec2( 0.5f, 0.5f );
	glm::vec2 botLeft = position + glm::vec2( -0.5f, -0.5f );
	glm::vec2 botRight = position + glm::vec2( 0.5f, -0.5f );

	// top left then copy for degeneracy

	pushVertex( topLeft, t_vertex, t_buffer );
	t_buffer.pushData( t_buffer.pointer, t_buffer.pointer + 6 );

	// top right then bottom right

	pushVertex( topRight, t_vertex, t_buffer );
	pushVertex( botLeft, t_vertex, t_buffer );

	// bottom right then copy for degeneracy

	pushVertex( botRight, t_vertex, t_buffer );
	t_buffer.pushData
		( t_buffer.pointer + (t_buffer.getIndex() - 6)
			, t_buffer.pointer + t_buffer.getIndex() );
}

///////////////////////////////////////////////////////////////////////////////
// The implementation itself
///////////////////////////////////////////////////////////////////////////////

class LineRenderer::LineRendererImpl {

	friend class LineRenderer;

	LineRendererImpl() {

		m_vertexData = new float[MAX_VERTEX_COUNT * 6];
		initialize();
	}

	~LineRendererImpl() {

		finalize();
		delete[] m_vertexData;
	}

	void initialize() {

		GlShader vertexShader{ GlShader::vertexShader() };
		vertexShader.compileFromFile( "shaders/color_line_vs_120.glsl" );
		GlShader fragmentShader{ GlShader::fragmentShader() };
		fragmentShader.compileFromFile( "shaders/color_line_fs_120.glsl" );
		m_program.attachShader( vertexShader );
		m_program.attachShader( fragmentShader );
		m_program.bindAttributeLocation( "attributePosition", 0 );
		m_program.bindAttributeLocation( "attributeColor", 1 );
		m_program.link();

		try {

			GlShader clearVertexShader{ GlShader::vertexShader() };
			clearVertexShader.compileFromFile( "shaders/fsq_vs_330.glsl" );
			GlShader clearGeometryShader{ GlShader::geometryShader() };
			clearGeometryShader.compileFromFile( "shaders/fsq_gs_330.glsl" );
			GlShader clearFragmentShader{ GlShader::fragmentShader() };
			clearFragmentShader.compileFromFile( "shaders/clear_fs_330.glsl" );
			m_clearProgram.attachShader( clearVertexShader );
			m_clearProgram.attachShader( clearGeometryShader );
			m_clearProgram.attachShader( clearFragmentShader );
			m_clearProgram.link();
		}
		catch ( const std::exception& e ) {

			printf( e.what() );
			std::rethrow_exception( std::current_exception() );
		}
		GlVertexAttribute attributes[2] = {
			GlVertexAttribute::floatAttribute( 3 ),
			GlVertexAttribute::floatAttribute( 3 )
		};

		m_definition = new GlVertexDefinition{ attributes, attributes + 2 };

		GlGuard guards[] = {
			{ m_buffer },
			{ *m_definition },
			{ m_vertexArrayObject }
		};

		m_vertexArrayObject.bind();
		m_buffer.bindToArray();
		m_definition->bind();

		m_window = SDL_GL_GetCurrentWindow();
	}

	void finalize() {

		delete m_definition;
	}

	void updateVertexBuffer() {

		GlGuard guards[] = {
			{ *m_definition },
			{ m_buffer },
			{ m_vertexArrayObject }
		};
		m_vertexArrayObject.bind();
		m_buffer.bindToArray();
		m_buffer.fillDynamicDraw( m_vertexData, m_dataLength * sizeof( float ) );
		m_definition->bind();
	}

	// given the current state of Game, sets up the view matrix and sends it to the shader

	void setupViewMatrix(
		) {

		// set up essential variables

		glm::vec3 eye{ 0.0f, 0.0f, 0.0f };
		glm::vec3 look{ 0.0f, 0.0f, 1.0f };
		glm::vec3 up{ 0.0f, 1.0f, 0.0f };

		// create view matrix

		glm::mat4 viewMatrix{ glm::lookAt( eye, look, up ) };
		float* view{ glm::value_ptr( viewMatrix ) };

		// upload to shader

		m_program.getUniform( "uniformView" ).setValue( std::vector<float>{ view, view + 16 } );
	}

	// given the current state of Game, sets up the projection matrix and sends it to the shader

	void setupProjectionMatrix(
		) {


		// create and use projection matrix

		int windowWidthInt{ 1280 };
		int windowHeightInt{ 720 };
		SDL_GetWindowSize( m_window, &windowWidthInt, &windowHeightInt );
		float windowWidth = static_cast<float>(windowWidthInt);
		float windowHeight = static_cast<float>(windowHeightInt);
		float hRatio{ 1.0f };
		float vRatio{ 1.0f };


		if ( windowWidth > windowHeight ) {

			hRatio = windowWidth / windowHeight;
		}
		else {

			vRatio = windowHeight / windowWidth;
		}

		float halfViewWidth{ 300.0f * hRatio * 0.5f };
		float halfViewHeight{ 300.0f * vRatio * 0.5f };

		glm::mat4 projectionMatrix{ glm::ortho(
			halfViewWidth, -halfViewWidth, -halfViewHeight, halfViewHeight, -1.0f, 1.0f ) };
		float* projection{ glm::value_ptr( projectionMatrix ) };
		m_program.getUniform( "uniformProjection" ).setValue( std::vector<float>{ projection, projection + 16 } );
	}

	void render() {

		int windowWidth{ 1280 };
		int windowHeight{ 720 };
		SDL_GetWindowSize( m_window, &windowWidth, &windowHeight );

		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		m_clearProgram.bind();
		m_clearProgram.getUniform( "uniformColour" ).setValue( std::vector<float>{ 0.0f, 0.0f, 0.0f, 0.5f } );
		glDrawArrays( GL_POINTS, 0, 1 );

		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
		glBlendFunc( GL_ONE, GL_ONE );
		glViewport( 0, 0, windowWidth, windowHeight );
		m_definition->enable();

		updateVertexBuffer();

		GlGuard guards[] = {
			{ *m_definition },
			{ m_program },
			{ m_vertexArrayObject }
		};
		m_program.bind();

		setupViewMatrix();
		setupProjectionMatrix();

		m_vertexArrayObject.bind();

		try {

			glDrawArrays( GL_TRIANGLE_STRIP, 0, m_dataLength / 6 );
		}
		catch ( const std::exception& e ) {

			printf( e.what() );
		}
	}

	void queueDraw( std::vector<LineRendererVertex>& t_vertices, glm::mat3& t_matrix ) {

		PushBuffer<float> buffer( static_cast<int>(t_vertices.size() * 12) );
		auto begin = t_vertices.begin();
		auto end = t_vertices.begin();
		for ( ; end != t_vertices.end(); ++end ) {

			if ( end->isSeperator() && begin != t_vertices.end()) {

				if ( end - begin == 1 ) {

					pushPointInBuffer( *begin, buffer );
				}
				else if ( (end + 1) != t_vertices.end()
					&& (end + 1)->isSeperator() ) {

					pushCircuitInBuffer( begin, end, buffer );
				}
				else {

					pushSequenceInBuffer( begin, end, buffer );
				}
				begin = t_vertices.end();
			}
			else if (!end->isSeperator()) {

				if ( begin == t_vertices.end() ) {

					begin = end;
				}
				glm::vec3 position = t_matrix * glm::vec3(glm::make_vec2( end->position ), 1.0f);
				end->position[0] = position[0];
				end->position[1] = position[1];
			}
		};
		std::copy( buffer.pointer, buffer.pointer + buffer.size
				, m_vertexData + m_dataLength );
		m_dataLength += buffer.size;
	}

	void clearQueue() {

		m_dataLength = 0;
	}

	GlBuffer m_buffer;
	GlVAObject m_vertexArrayObject;
	GlProgram m_program;
	GlProgram m_clearProgram;
	GlVertexDefinition* m_definition;

	float* m_vertexData;
	unsigned int m_dataLength{ 0 };
	SDL_Window* m_window;
};

LineRenderer::LineRenderer( ConstructRendererPermit& t_permit ) : IGameRenderer{ t_permit } {

	m_lineRenderer = new LineRendererImpl();
}

LineRenderer::~LineRenderer() {

	delete m_lineRenderer;
}

void LineRenderer::render() {

	m_lineRenderer->render();
}

void LineRenderer::queueDraw( LineRendererMessage& t_message ) {

	m_lineRenderer->queueDraw( t_message.vertices, t_message.transform );
}

void LineRenderer::beforeActorsDraw() {

	m_lineRenderer->clearQueue();
}

void LineRenderer::afterActorsDraw() {

}

IGameRenderer* LineRenderer::createInstance( ConstructRendererPermit& t_permit ) {

	return new LineRenderer( t_permit );
}

