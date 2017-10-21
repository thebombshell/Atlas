
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

	class TestPrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		TestPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner )
			: IActorComponent{ t_permit, t_owner }, m_circle{ t_owner.getTransform(), 0.5f } {

			pthn::SpriteRenderer* const renderer = pthn::Game::GetRendererAs<pthn::SpriteRenderer>();
			assert( renderer != nullptr );
			if ( !renderer->hasTexture( "test" ) ) {

				renderer->loadTexture( "images/test_pic.png", "test" );
			}
			t_owner.createComponent<pthn::Collision2DComponent>();
			t_owner.createComponent<pthn::PhysicsComponent2D>();
			auto& collisions = t_owner.getComponent<pthn::Collision2DComponent>();
			collisions.addCollider( &m_circle );
			auto& physics = t_owner.getComponent<pthn::PhysicsComponent2D>();
			physics.setDynamic();
			physics.setSolid();
			physics.setGravityEnabled();
			physics.bounce = 0.1f;
			physics.friction = 0.1f;
		}
		~TestPrefab() {

		}

		void update( float t_delta ) override {

			auto& input = pthn::Game::GetInput();
			auto& physics = getOwner().getComponent<pthn::PhysicsComponent2D>();

			if ( input.isKeyDown( "Left" ) ) {

				physics.velocity[0] -= 8.0f * t_delta;
			}
			if ( input.isKeyDown( "Right" ) ) {

				physics.velocity[0] += 8.0f * t_delta;
			}

			if ( input.isKeyDown( "Down" ) ) {

				physics.velocity[1] -= 8.0f * t_delta;
			}
			if ( input.isKeyDown( "Up" ) ) {

				physics.velocity[1] += 8.0f * t_delta;
			}
		}

		void render() override {
			
			pthn::SpriteRenderer* const renderer = pthn::Game::GetRendererAs<pthn::SpriteRenderer>();
			pthn::Transform2D transform = getOwner().getTransform2D();
			pthn::SpriteRendererMessage messageA{ "test", { 0.5f, 0.5f }, {	0, 0 }, { 64, 64 }, { 1.0f, 1.0f, 1.0f }, 0.5f, transform.findMatrix() };
			renderer->queueDraw( messageA );
		}

		pthn::Circle m_circle;
	};

	class FloorPrefab : public pthn::IRenderable {

	public:

		FloorPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner )
			: IActorComponent{ t_permit, t_owner }, m_hull{ t_owner.getTransform() } {

			pthn::SpriteRenderer* const renderer = pthn::Game::GetRendererAs<pthn::SpriteRenderer>();
			assert( renderer != nullptr );
			if ( !renderer->hasTexture( "test" ) ) {

				renderer->loadTexture( "images/test_pic.png", "test" );
			}
			t_owner.createComponent<pthn::Collision2DComponent>();
			t_owner.createComponent<pthn::PhysicsComponent2D>();
			auto& collisions = t_owner.getComponent<pthn::Collision2DComponent>();
			glm::vec2 points[]{
				{ -0.5f, -0.5f },
				{ 0.5f, -0.5f },
				{ 0.5f, 0.5f },
				{ -0.5f, 0.5f }
			};
			m_hull.points.insert( m_hull.points.end(), points, points + 4 );
			collisions.addCollider(&m_hull);
			auto& physics = t_owner.getComponent<pthn::PhysicsComponent2D>();
			physics.setKinematic();
			physics.setSolid();
		}
		~FloorPrefab() {

		}

		void render() override {

			pthn::SpriteRenderer* const renderer = pthn::Game::GetRendererAs<pthn::SpriteRenderer>();
			pthn::Transform2D transform = getOwner().getTransform2D();
			pthn::SpriteRendererMessage messageA{ "test", { 0.5f, 0.5f }, { 96, 32 }, { 64, 64 }, { 1.0f, 1.0f, 1.0f }, 0.5f, transform.findMatrix() };
			renderer->queueDraw( messageA );
		}

		pthn::ConvexHull m_hull;
	};
}

#endif