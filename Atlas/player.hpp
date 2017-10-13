
// player.hpp

// header file for public implementation of player actor

#ifndef ATLAS_PLAYER_HPP
#define ATLAS_PLAYER_HPP

#include <pantheon.hpp>
#include <shapes_2d.hpp>

namespace atlas {

	// stores and communicates input profiles

	struct PlayerInputProfile {

	public:

		PlayerInputProfile()
			: forward{ "NA" }, backward{ "NA" }
			, rotateCW{ "NA" }, rotateCCW{ "NA" }
			, fire{ "NA" } {

		}
		PlayerInputProfile
			( std::string t_forward, std::string t_backward
			, std::string t_rotateCW, std::string t_rotateCCW
			, std::string t_fire )
			: forward{ t_forward }, backward{ t_backward }
			, rotateCW{ t_rotateCW }, rotateCCW{ t_rotateCCW }
			, fire{ t_fire } {

		}

		// generates a standard input profile from a player index

		static PlayerInputProfile fromIndex(int index) {

			std::string joystick = "joystick" + std::to_string( index );
			return{
				joystick + "_axis1", "NA",
				joystick + "_axis0", "NA",
				joystick + "_button0"
			};
		}

		std::string forward;
		std::string backward;
		std::string rotateCW;
		std::string rotateCCW;
		std::string fire;
	};

	// info object to be passed on player creation

	struct PlayerInfo {

	public:

		PlayerInfo( int t_index, const glm::vec3& t_position )
			: index( t_index ), position{ t_position } {

		}

		const int index;
		const glm::vec3 position;
	};

	class Player : public pantheon::IUpdatable, public pantheon::IRenderable {

	private:

		void setupColliders();
		void setupVertices();

		int m_index{ 0 };
		PlayerInputProfile m_input{ };

		glm::vec3 m_vertices[4];
		pantheon::ConvexHull m_colliders[2];

		float m_respawnTimer{ 0.0f };
		float m_shieldTimer{ 0.0f };

	public:

		Player( pantheon::Actor::Key, pantheon::Actor&, const PlayerInfo& t_info );

		~Player();

		void update( float ) override;

		void render() override;

		void onEventMessage( pantheon::IActorEventMessage* const ) override;

		bool kill();

		void grow();
	};
}
#endif