
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
			physics.bounce = 0.0f;
			physics.friction = 0.02f;
			physics.airResistance = 1.0f;
		}
		~TestPrefab() {

		}

		void update( float t_delta ) override {

			auto& input = pthn::Game::GetInput();
			auto& physics = getOwner().getComponent<pthn::PhysicsComponent2D>();
			auto& transform = getOwner().getTransform();

			if ( input.isKeyDown( "Left" ) ) {

				physics.velocity[0] -= 8.0f * t_delta;
			}
			if ( input.isKeyDown( "Right" ) ) {

				physics.velocity[0] += 8.0f * t_delta;
			}

			if ( input.isKeyDown( "Space" ) && m_isOnGround ) {

				physics.velocity[1] = m_jumpStrength;
			}
			float alpha = t_delta * 10.0f;
			m_viewPosition = m_viewPosition * alpha + glm::vec2( transform.position ) * ( 1.0f - alpha );
			m_isOnGround = false;
		}

		void render() override {
			
			pthn::SpriteRenderer* const renderer = pthn::Game::GetRendererAs<pthn::SpriteRenderer>();
			pthn::Transform2D transform = getOwner().getTransform2D();

			renderer->setViewPosition( transform.position );
			pthn::SpriteRendererMessage messageA{ "test", { 0.5f, 0.5f }, {	0, 0 }, { 64, 64 }, { 1.0f, 1.0f, 1.0f }, 0.5f, transform.findMatrix() };
			renderer->queueDraw( messageA );
		}


		void onEventMessage( pthn::IActorEventMessage* const t_message ) override {

			pthn::PhysicsCollisionMessage2D* const physicsMessage = t_message->as<pthn::PhysicsCollisionMessage2D>();
			if ( physicsMessage != nullptr ) {

				if ( glm::dot( glm::normalize( physicsMessage->resolve ), { 0.0f, 1.0f } ) > 0.5f ) {

					m_isOnGround = true;
				}
			}
		}

		pthn::Circle m_circle;
		float m_jumpStrength{ 6.0f };
		bool m_isOnGround{ false };
		glm::vec2 m_viewPosition{ 0.0f, 0.0f };
	};

	class BlockMessage {

	public:

		BlockMessage( const glm::vec2& t_position, const glm::vec2& t_scale, bool t_isDynamic ) 
			: position{ t_position }, scale{ t_scale }, isDynamic{ t_isDynamic } {

		}

		glm::vec2 position;
		glm::vec2 scale;
		bool isDynamic;
	};

	class BlockPrefab : public pthn::IRenderable {

	public:

		BlockPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner )
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
			collisions.addCollider( &m_hull );
			auto& physics = t_owner.getComponent<pthn::PhysicsComponent2D>();
			physics.setKinematic();
			physics.setSolid();
		}
		BlockPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner, const BlockMessage& t_message )
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
			collisions.addCollider( &m_hull );
			auto& physics = t_owner.getComponent<pthn::PhysicsComponent2D>();
			if ( t_message.isDynamic ) {

				physics.setDynamic();
				physics.setGravityEnabled();
			}
			else {

				physics.setKinematic();
			}
			physics.setSolid();
			physics.bounce = 0.5f;
			physics.friction = 0.5f;

			pthn::Transform& transform = t_owner.getTransform();
			transform.position = { t_message.position, 0.0f };
			transform.scale = { t_message.scale, 1.0f };
		}
		~BlockPrefab() {

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