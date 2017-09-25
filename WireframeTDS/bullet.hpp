
// bullet.hpp

// Header file for the player ship class

#ifndef WIREFRAMETDS_BULLET_HPP
#define WIREFRAMETDS_BULLET_HPP

#include "wireframe_tds.hpp"
#include "game_actor.hpp"
#include "transform.hpp"

namespace wireframe_tds {

	namespace game {

		struct BulletInfo {
		public:
			BulletInfo(
				glm::vec2 t_position,
				glm::vec2 t_direction,
				IGameActor* t_owner
				) : position(t_position)
				, direction(t_direction)
				, owner( t_owner ) {

			}

			glm::vec2 position{ 0.0f, 0.0f };
			glm::vec2 direction{ 1.0f, 0.0f };
			IGameActor* const owner{ nullptr };
		};

		class Bullet : public IGameUpdatable, IGameCollidable, IGameRenderable {
		private:

			glm::vec2 m_position{ 0.0f, 0.0f };
			glm::vec2 m_direction{ 1.0f, 0.0f };
			float m_age{ 0.0f };
			collisions::Circle m_collider{ { 0.0f, 0.0f }, 0.2f };
			IGameActor* const m_owner{ nullptr };
		public:
			Bullet(
				const BulletInfo& t_info
				) : m_position(t_info.position)
				, m_direction(t_info.direction)
				, m_collider(t_info.position, 0.2f)
				, m_owner( t_info.owner ) {

				assert( glm::length( t_info.direction ) - 1.0f <= std::numeric_limits<float>::epsilon() );
			}

			~Bullet(
				) override;

			const collisions::ICollider* getCollider(
				) const override;

			void onCollision(
				const glm::vec3& t_axis,
				IGameActor& t_other
				) override;

			void onRender(
				float delta
				) const override;

			void onUpdate(
				float delta
				) override;
		};
	}

}

#endif