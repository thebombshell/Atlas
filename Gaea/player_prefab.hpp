
// player_prefab.hpp

#ifndef GAEA_PLAYER_PREFAB_HPP
#define GAEA_PLAYER_PREFAB_HPP

#include <pantheon.hpp>
#include <collision_2d.hpp>
#include <sprite_renderer.hpp>
#include <physics_component.hpp>

namespace gaea {

	const int PLAYER_STATE_NONE{ 0 };
	const int PLAYER_STATE_IS_RUNNING{ 1 };
	const int PLAYER_STATE_IS_JUMPING{ 2 };
	const int PLAYER_STATE_IS_LOOKING_RIGHT{ 4 };

	const float PLAYER_TOP_SPEED{ 5.0f };
	const float PLAYER_JUMP_STRENGTH{ 8.0f };
	const float WORLD_GRAVITY_STRENGTH{ 14.0f };

	class PlayerPrefab : public pthn::IUpdatable, public pthn::IRenderable {

	public:

		PlayerPrefab( pthn::ConstructComponentPermit& t_permit, pthn::Actor& t_owner );
		~PlayerPrefab();

		void onEventMessage( pthn::IActorEventMessage* const t_message ) override;

		void update( float t_delta ) override;
		void render() override;

		void kill();

	private:

		void updateGround();
		void updateInput( float t_delta );
		bool checkGroundViability( const std::vector<pthn::CollisionGroup2D>& t_possibleGround );
		bool checkStickViability( const std::vector<pthn::CollisionGroup2D>& t_possibleGround );

		pthn::Circle m_circle;
		float m_jumpingForce{ PLAYER_JUMP_STRENGTH };
		float m_acceleration{ 10.0f };
		float m_decceleration{ 10.0f };
		float m_topSpeed{ PLAYER_TOP_SPEED };
		bool m_isOnGround{ false };
		glm::vec2 m_stickPosition{ 0.0f, 0.0f };
		pthn::Transform* m_stuckTo{ nullptr };
		int m_state{ PLAYER_STATE_IS_LOOKING_RIGHT };
		float m_animTimer{ 0.0f };
	};

}

#endif