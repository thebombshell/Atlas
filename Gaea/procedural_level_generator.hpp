
// procedural_level_generator.hpp

#ifndef GAEA_PROCEDRUAL_LEVEL_GENERATOR_HPP
#define GAEA_PROCEDURAL_LEVEL_GENERATOR_HPP

#include <pantheon.hpp>

#include <collision_2d.hpp>

namespace gaea {
	
	class ProceduralChunk {

	public:

		ProceduralChunk(int t_offsetIndex);
		~ProceduralChunk();
		
		const int offsetIndex;
		const float offsetPosition;
		std::vector<pthn::Actor*> actors;
		ProceduralChunk* previousChunk;
		ProceduralChunk* nextChunk;
	};

	class ProceduralLevelGenerator : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		ProceduralLevelGenerator( pthn::ConstructComponentPermit&, pthn::Actor& );
		~ProceduralLevelGenerator();

		void update( float t_delta ) override;
		void render( ) override;

		void handleTracking();

		void handleCulling();

		void handleProcedure();

		float getPredictedY( float t_x );

		float getPredictedX( float t_y );

	private:

		pthn::Actor* m_player;
		pthn::Actor* m_ground;
		pthn::Actor* m_wall;
		pthn::ConvexHull* m_cullBarrier;
		std::vector<pthn::Actor*> m_levelActors;
		float m_backTrackLimit;
		float m_chunkTimer;
		
		glm::vec2 m_minLocation;
		glm::vec2 m_maxLocation;

		float m_predictedJumpTime;
		float m_predictedJumpDistance;
		float m_predictedJumpHeight;
	};
}

#endif