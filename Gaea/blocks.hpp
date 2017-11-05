
// test_prefab.hpp

#ifndef GAEA_TEST_PREFAB_HPP
#define GAEA_TEST_PREFAB_HPP

#include <pantheon.hpp>

#include <collision_2d.hpp>
#include <iactor_component.hpp>
#include <physics_component.hpp>
#include <shapes_2d.hpp>
#include <sprite_renderer.hpp>

namespace gaea {

	class BlockMessage {

	public:

		BlockMessage( const glm::vec2& t_position, const glm::vec2& t_scale );

		glm::vec2 position;
		glm::vec2 scale;
	};

	class BlockPrefab : public pthn::IRenderable {

	public:

		BlockPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner, const BlockMessage& t_message );
		~BlockPrefab();

		void render() override;

		pthn::ConvexHull m_hull;
	};

	class MovingBlockMessage {

	public:

		MovingBlockMessage( const glm::vec2& t_start, const glm::vec2& t_end, const glm::vec2& t_scale );

		glm::vec2 start;
		glm::vec2 end;
		glm::vec2 scale;
	};
	
	class MovingBlockPrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		MovingBlockPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner, const MovingBlockMessage& t_message );
		~MovingBlockPrefab();

		void update( float t_delta ) override;

		void render() override;

		pthn::ConvexHull m_hull;
		glm::vec2 start;
		glm::vec2 end;
		bool isMovingToEnd{ true };
	};

	class HazardPrefab : public pthn::IRenderable {

	public:

		HazardPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner, const BlockMessage& t_message );
		~HazardPrefab();

		void render() override;

		pthn::ConvexHull m_hull;
	};
}

#endif