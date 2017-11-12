
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
			, fire{ "NA" }, boost{ "NA" } {

		}
		PlayerInputProfile
			( std::string t_forward, std::string t_backward
			, std::string t_rotateCW, std::string t_rotateCCW
			, std::string t_fire, std::string t_boost )
			: forward{ t_forward }, backward{ t_backward }
			, rotateCW{ t_rotateCW }, rotateCCW{ t_rotateCCW }
			, fire{ t_fire }, boost{ t_boost } {

		}

		// generates a standard input profile from a player index

		static PlayerInputProfile fromIndex(int index) {

			std::string joystick = "";
			switch ( index ) {
				case 0:
				case 1:
					joystick = "joystick" + std::to_string( index );
					return{
						joystick + "_axis1", "NA",
						joystick + "_axis0", "NA",
						joystick + "_button4",
						joystick + "_axis2"
					};
					break;
				case 2:
				case 3:
					joystick = "joystick" + std::to_string( index - 2 );
					return{
						joystick + "_axis4", "NA",
						joystick + "_axis3", "NA",
						joystick + "_button5",
						joystick + "_axis5"
					};
					break;

			}
			return{ };
		}

		std::string forward;
		std::string backward;
		std::string rotateCW;
		std::string rotateCCW;
		std::string fire;
		std::string boost;
	};

	// info object to be passed on player creation

	struct PlayerInfo {

	public:

		PlayerInfo( int t_index ) : index( t_index ) {

		}

		const int index;
	};

	class Player : public pantheon::IUpdatable, public pantheon::IRenderable {

	public:

		Player( pantheon::ConstructComponentPermit&, pantheon::Actor&, const PlayerInfo& t_info );

		~Player();

		void update( float ) override;

		void render() override;

		void onEventMessage( pantheon::IActorEventMessage* const ) override;

		bool kill();

		bool isKillable();

		void makeJuggernaught();

		void takeJuggernaught();

		bool isJuggernaught();

		void findSize();

		static void reset();

		static void beginJuggernaught();

	private:

		void setupColliders();
		void setupPhysics();
		void setupSounds();

		int m_index{ 0 };
		bool m_isJuggernaught{ false };
		PlayerInputProfile m_input{ };

		glm::vec3 m_vertices[4];
		pthn::ConvexHull m_colliders[2];
		pthn::Source* m_bonkSource;
		pthn::Source* m_wooshSource; 
		pthn::Source* m_humSource;

		float m_respawnTimer{ 0.0f };
		float m_shieldTimer{ 1.0f };
		float m_boostTimer{ 1.0f };
	};
}
#endif