
// sprite_renderer.hpp

// Header file for the public implementation of the sprite renderer class

#ifndef PANTHEON_SPRITE_RENDERER_HPP
#define PANTHEON_SPRITE_RENDERER_HPP

#include "pantheon.hpp"
#include "game.hpp"
#include "gl_objects.hpp"

namespace pantheon {

	class PANTHEON_API SpriteRendererMessage {

	public:

		SpriteRendererMessage
			( const std::string& t_textureName, const glm::vec2& t_origin
			, const glm::vec2& t_sourceCoords, const glm::vec2& t_sourceSize
			, const glm::vec3& t_colour, float t_depth, const glm::mat3& t_transform );
		~SpriteRendererMessage();

		std::vector<float> findVertices();

		std::string sourceName;
		glm::vec2 sourceCoords;
		glm::vec2 sourceSize;
		glm::vec2 origin;
		glm::vec3 colour;
		glm::mat3 transform;
		float depth;
	};

	class PANTHEON_API SpriteRenderer : public IGameRenderer {

	public:
	
		static IGameRenderer* createInstance( ConstructRendererPermit& );

		void setViewPosition( const glm::vec2& t_position );
		void setViewSize( float t_minViewDistance );

		void loadTexture( const std::string& t_path );
		void loadTexture( const std::string& t_path, const std::string& t_name );
		bool hasTexture( const std::string& t_name );
		void unloadTexture( const std::string& t_name );

		void queueDraw( const SpriteRendererMessage& t_message );

		void render() override;

		void beforeActorsDraw() override;

		void afterActorsDraw() override;

		glm::vec2 getPosition() const;

		float getViewDistance() const;

	private:

		SpriteRenderer( ConstructRendererPermit& );
		~SpriteRenderer();

		class SpriteRendererImpl;
		SpriteRendererImpl* m_renderer{ nullptr };
	};
}

#endif