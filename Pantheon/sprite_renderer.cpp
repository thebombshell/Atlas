
// sprite_renderer.cpp

// source file for sprite renderer class

#include "sprite_renderer.hpp"

#include "gl_objects.hpp"
#include "helpers.hpp"

#include <GL\glew.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <memory>
#include <SDL.h>

using namespace pantheon;

const unsigned short MAX_SPRITE_DRAW = 1024;
const unsigned short MAX_VERTEX_DRAW = MAX_SPRITE_DRAW * 6;

///////////////////////////////////////////////////////////////////////////////
// SpriteRendererMessage
///////////////////////////////////////////////////////////////////////////////

SpriteRendererMessage::SpriteRendererMessage
( const std::string& t_textureName, const glm::vec2& t_origin
	, const glm::vec2& t_sourceCoords, const glm::vec2& t_sourceSize
	, const glm::vec3& t_colour, float t_depth, const glm::mat3& t_transform )
	: sourceName{ t_textureName }, origin{ t_origin }
	, sourceCoords{ t_sourceCoords }, sourceSize{ t_sourceSize }
	, colour{ t_colour }, depth{ t_depth }, transform{ t_transform } {

}

SpriteRendererMessage::~SpriteRendererMessage() {

}

std::vector<float> SpriteRendererMessage::findVertices() {

	glm::vec2 topLeft = transform * glm::vec3{ glm::vec2{ 0.0f, 1.0f } - origin, 1.0f };
	glm::vec2 botLeft = transform * glm::vec3{ glm::vec2{ 0.0f, 0.0f } - origin, 1.0f };
	glm::vec2 botRight = transform * glm::vec3{ glm::vec2{ 1.0f, 0.0f } - origin, 1.0f };
	glm::vec2 topRight = transform * glm::vec3{ glm::vec2{ 1.0f, 1.0f } - origin, 1.0f };
	return std::vector<float> 
		{ topLeft[0], topLeft[1], depth, colour[0], colour[1], colour[2], sourceCoords[0], sourceCoords[1]
		, botLeft[0], botLeft[1], depth, colour[0], colour[1], colour[2], sourceCoords[0], sourceCoords[1] + sourceSize[1]
		, topRight[0], topRight[1], depth, colour[0], colour[1], colour[2], sourceCoords[0] + sourceSize[0], sourceCoords[1]
		, botRight[0], botRight[1], depth, colour[0], colour[1], colour[2], sourceCoords[0] + sourceSize[0], sourceCoords[1] + sourceSize[1] };
}

///////////////////////////////////////////////////////////////////////////////
// SpriteRenderer
///////////////////////////////////////////////////////////////////////////////

class SpriteRenderer::SpriteRendererImpl {

	friend class SpriteRenderer;

	typedef std::vector<SpriteRendererMessage> drawQueue;

	SpriteRendererImpl() {

		// load program

		try {

			GlShader vertexShader{ GlShader::vertexShader() };
			vertexShader.compileFromFile( "shaders/sprite_vs_120.glsl" );
			GlShader fragmentShader{ GlShader::fragmentShader() };
			fragmentShader.compileFromFile( "shaders/sprite_fs_120.glsl" );
			m_program.attachShader( vertexShader );
			m_program.attachShader( fragmentShader );
			m_program.bindAttributeLocation( "attributePosition", 0 );
			m_program.bindAttributeLocation( "attributeColour", 1 );
			m_program.bindAttributeLocation( "attributeTexture", 2 );
			m_program.link();

			GlVertexAttribute attributes[3] = {
				GlVertexAttribute::floatAttribute( 3 ),
				GlVertexAttribute::floatAttribute( 3 ),
				GlVertexAttribute::floatAttribute( 2 )
			};
			m_mesh = new GlMesh( { attributes, attributes + 3 } );
			std::vector<unsigned short> indices;
			indices.resize( MAX_VERTEX_DRAW );
			for ( unsigned short i = 0; i < MAX_SPRITE_DRAW; ++i ) {

				unsigned short vertexOffset = i * 4;
				unsigned short indexOffset = i * 6;
				indices[indexOffset + 0] = vertexOffset + 0;
				indices[indexOffset + 1] = vertexOffset + 0;
				indices[indexOffset + 2] = vertexOffset + 1;
				indices[indexOffset + 3] = vertexOffset + 2;
				indices[indexOffset + 4] = vertexOffset + 3;
				indices[indexOffset + 5] = vertexOffset + 3;
			}
			m_mesh->fillIndicesStatic<unsigned short>( indices );
		}
		catch ( const IGlException& e ) {

			printf( e.what() );
			std::rethrow_exception( std::current_exception() );
		}
		m_window = SDL_GL_GetCurrentWindow();
	}

	~SpriteRendererImpl() {

		delete m_mesh;
	}

	void setupViewMatrix() {

		// set up essential variables

		glm::vec3 eye{ m_viewPostion, 0.0f };
		glm::vec3 look{ 0.0f, 0.0f, 1.0f };
		glm::vec3 up{ 0.0f, 1.0f, 0.0f };

		// create view matrix

		glm::mat4 viewMatrix{ glm::lookAt( eye, look, up ) };
		float* view{ glm::value_ptr( viewMatrix ) };

		// upload to shader

		m_program.getUniform( "uniformView" ).setValue( std::vector<float>{ view, view + 16 } );
	}
	void setupProjectionMatrix() {

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

		float halfViewWidth{ m_minViewDistance * hRatio * 0.5f };
		float halfViewHeight{ m_minViewDistance * vRatio * 0.5f };

		glm::mat4 projectionMatrix{ glm::ortho(
			halfViewWidth, -halfViewWidth, -halfViewHeight, halfViewHeight, -1.0f, 1.0f ) };
		float* projection{ glm::value_ptr( projectionMatrix ) };
		m_program.getUniform( "uniformProjection" ).setValue( std::vector<float>{ projection, projection + 16 } );
	}

	void render() {

		int windowWidth{ 1280 };
		int windowHeight{ 720 };
		SDL_GetWindowSize( m_window, &windowWidth, &windowHeight );

		glDisable( GL_DEPTH_TEST );
		glDisable( GL_CULL_FACE );
		glViewport( 0, 0, windowWidth, windowHeight );

		GlGuard guards[] = {
			{ m_program },
			{ *m_mesh }
		};
		m_program.bind();
		
		setupViewMatrix();
		setupProjectionMatrix();
		m_program.getUniform( "uniformSampler" ).setValue( 0 );

		std::vector<float> meshData;
		std::vector<float> verts;

		for ( auto& pair : m_loadQueue ) {

			loadTexture( pair.second, pair.first );
		}
		m_loadQueue.clear();
		for ( auto& name : m_removeQueue ) {

			unloadTexture( name );
		}
		m_removeQueue.clear();

		for ( auto& pair : m_drawQueues ) {

			if ( pair.second.size() <= 0 ) {

				continue;
			}

			auto other_pair = m_textures.find( pair.first );
			other_pair->second->bind( 0 );
			//GlGuard textureGuard{ *other_pair->second };
			for ( auto& message : pair.second ) {

				verts = message.findVertices();
				meshData.insert( meshData.end(), verts.begin(), verts.end() );
			}
			m_mesh->fillVerticesDynamic( &meshData[0], meshData.size() * sizeof( float ) );
			meshData.clear();
			m_mesh->bind();
			int size = pair.second.size() * 6; 
			GlDebug debug;
			debug.update();
			glDrawElements( GL_TRIANGLE_STRIP, size, GL_UNSIGNED_SHORT, static_cast<void*>(0) );
		}

	}

	void setViewPosition(const glm::vec2& t_viewPosition) {

		m_viewPostion = t_viewPosition;
	}

	void setViewSize( float t_minViewDistance ) {

		t_minViewDistance = m_minViewDistance;
	}

	void queueLoadTexture( const std::string& t_path ) {

		queueLoadTexture( t_path, t_path );
	}

	void queueLoadTexture( const std::string& t_path, const std::string& t_name ) {

		assert( !hasTexture( t_name ) && "texture is already loaded / queued for loading" );
		m_loadQueue.insert( { t_name, t_path } );
		m_drawQueues.insert( { t_name, { } } );
	}

	void loadTexture( const std::string& t_path, const std::string& t_name ) {

		GlTexture2D* texture = new GlTexture2D();
		try {
			GlGuard guard{ *texture };
			texture->bind( 0 );
			texture->fillFromFile( t_path );
		}
		catch ( const std::exception& e ) {

			delete texture;
			throw e;
		}

		m_textures.insert( { t_name, texture } );
	}

	bool hasTexture( const std::string& t_name ) {

		auto waiting = m_loadQueue.find( t_name );
		auto pair = m_textures.find( t_name );
		return pair != m_textures.end() || waiting != m_loadQueue.end();
	}

	void queueUnloadTexture( const std::string& t_name ) {

		assert( hasTexture( t_name ) && "texture does not exist" );
		m_removeQueue.push_back( t_name );
	}
	void unloadTexture( const std::string& t_name ) {

		m_textures.erase( t_name );
		m_drawQueues.erase( t_name );
	}

	void beforeActorsDraw() {

		for ( auto& pair : m_drawQueues ) {

			pair.second.clear();
		}
	}

	void queueDraw( const SpriteRendererMessage& t_message ) {

		auto pair = m_drawQueues.find( t_message.sourceName );
		assert( pair != m_drawQueues.end() && "textures draw queue does not exist" );
		pair->second.push_back( t_message );
	}

	glm::vec2 m_viewPostion{ 0.0f, 0.0f };
	float m_minViewDistance{ 8.0f };
	GlMesh* m_mesh;
	GlProgram m_program;
	SDL_Window* m_window;
	std::map<std::string, GlTexture2D*> m_textures;
	std::map<std::string, drawQueue> m_drawQueues;
	std::map<std::string, std::string> m_loadQueue;
	std::vector<std::string> m_removeQueue;

};

SpriteRenderer::SpriteRenderer( ConstructRendererPermit& t_permit ) 
	: IGameRenderer{ t_permit } {

	m_renderer = new SpriteRendererImpl();
}

SpriteRenderer::~SpriteRenderer() {

	delete m_renderer;
}

void SpriteRenderer::beforeActorsDraw() {

	m_renderer->beforeActorsDraw();
}

void SpriteRenderer::afterActorsDraw() {

}

void SpriteRenderer::render() {

	m_renderer->render();
}

void SpriteRenderer::setViewPosition( const glm::vec2& t_viewPosition ) {

	m_renderer->setViewPosition( t_viewPosition );
}

void SpriteRenderer::setViewSize( float t_minViewDistance ) {

	m_renderer->setViewSize( t_minViewDistance );
}

void SpriteRenderer::loadTexture( const std::string& t_path ) {

	m_renderer->queueLoadTexture( t_path );
}

void SpriteRenderer::loadTexture( const std::string& t_path, const std::string& t_name ) {

	m_renderer->queueLoadTexture( t_path, t_name );
}

bool SpriteRenderer::hasTexture( const std::string& t_name ) {

	return m_renderer->hasTexture( t_name );
}

void SpriteRenderer::unloadTexture( const std::string& t_name ) {

	m_renderer->queueUnloadTexture( t_name );
}

void SpriteRenderer::queueDraw( const SpriteRendererMessage& t_message ) {

	m_renderer->queueDraw( t_message );
}

IGameRenderer* SpriteRenderer::createInstance( ConstructRendererPermit& t_permit ) {

	return new SpriteRenderer( t_permit );
}