
// player_ship.hpp

// Header file for the player ship class

#ifndef WIREFRAMETDS_PLAYER_SHIP_HPP
#define WIREFRAMETDS_PLAYER_SHIP_HPP

#include "wireframe_tds.hpp"
#include "collisions.hpp"
#include "game_actor.hpp"
#include "transform.hpp"
#include <string>

namespace atlas {

	namespace game {

		const int WTDS_PLAYER_ALIVE{ 0 };
		const int WTDS_PLAYER_DEAD{ 1 };
		const int WTDS_PLAYER_RESPAWNING{ 2 };

		class PlayerShip : public IGameUpdatable, IGameCollidable, IGameRenderable {
		private:

			friend class Bullet;

			void attributeKill(
				);

			glm::vec2 m_thrust{ 0.0f, 0.0f };
			float m_rotationSpeed{ 0.0f };
			Transform m_transform{ };
			std::string m_forwardInput{ "w" };
			std::string m_backwardInput{ "s" };
			std::string m_clockwiseInput{ "d" };
			std::string m_counterClockwiseInput{ "a" };
			std::string m_fireInput{ " " };
			glm::vec2 m_points[3];
			collisions::Triangle m_collider{ { { 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 0.0f } } };
			int m_playerState{ WTDS_PLAYER_ALIVE };
			float m_cooldown{ 0.0f };
			float m_deathTimer{ 0.0f };
			float m_respawnTimer{ 0.0f };
		public:
			PlayerShip(
				const int& t_index
				);

			~PlayerShip(
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

			int getPlayerState(
				);

			void killPlayer(
				);
		};
	}

}

#endif