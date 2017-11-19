
// procedural_level_generator.hpp

#ifndef GAEA_PROCEDRUAL_LEVEL_GENERATOR_HPP
#define GAEA_PROCEDURAL_LEVEL_GENERATOR_HPP

#include <pantheon.hpp>

#include <collision_2d.hpp>

namespace gaea {
	
	class IPCGOption {

	public:

		IPCGOption() {

		}

		virtual ~IPCGOption() = 0 {

		}

		virtual float getValue() = 0;

		virtual void update() = 0;

		virtual void render() = 0;
	};

	class ToggleTagOption : public IPCGOption {

	public:

		ToggleTagOption(int t_index, std::string t_name);
		~ToggleTagOption();

		float getValue() override;
		void update() override;
		void render() override;

	private:

		pthn::Transform2D m_transform;
		std::string m_name;
		glm::vec2 m_min;
		glm::vec2 m_max;
		int m_index;
		bool m_wasMouseDown;
		bool m_isMouseOver;
		bool m_isEnabled;
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

		ToggleTagOption* m_qualityControl;
		ToggleTagOption* m_snapToGrid;
		ToggleTagOption* m_heightLimit;
		ToggleTagOption* m_minimumJump;
		ToggleTagOption* m_variablePlatforms;
		ToggleTagOption* m_weightedVerticality;
		std::vector<IPCGOption*> m_options;
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