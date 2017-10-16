
// sprite_renderer.hpp

// Header file for the public implementation of the sprite renderer class

#ifndef PANTHEON_SPRITE_RENDERER_HPP
#define PANTHEON_SPRITE_RENDERER_HPP

#include "pantheon.hpp"

#include "game.hpp"

namespace pantheon {

	class PANTHEON_API SpriteRenderer : IGameRenderer {

	public:
	
		static IGameRenderer* createInstance( ConstructRendererPermit& );

		void render() override;

		void beforeActorsDraw() override;

		void afterActorsDraw() override;

	private:

		SpriteRenderer( ConstructRendererPermit& );
		~SpriteRenderer();

		class SpriteRendererImpl;
		SpriteRendererImpl* m_renderer{ nullptr };
	};
}

#endif