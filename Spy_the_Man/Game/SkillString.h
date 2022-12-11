#pragma once
#include "SkillType.h"
#include <string>

namespace PM {
	inline std::string Null = "";

	inline std::string Drone1 { "Can activate drones and move them instead" };
	inline std::string Drone2{ "Press skill button again to deactivate" };
	inline std::string Drone3{ "It will leave a camera and make noises" };

	inline std::string DroneSound1 {"Drone can make noise when deactivated"};
	inline std::string DroneSound2 {"The sound will get attract enemies"};

	inline std::string Chameleon1 {"Player will be invisible while standing still"};
	inline std::string Chameleon2 {"Return back to normal if moved"};

	inline std::string Invisible1 {"Player will be invisible for a short time"};
	inline std::string Invisible2 {"This will last for 4 seconds."};

	inline std::string Visualize1 {"The sound enemies make will be visualized"};
	inline std::string Visualize2 {"Position of enemies will be shown by waves"};

	inline std::string SilentRun1 {"Running does not make sound anymore"};
	inline std::string SilentRun2 {"Enemies will not be attracted by running"};

	inline std::string Assassinate1{ "Can kill enemy workers and guards" };
	inline std::string Assassinate2{ "The button will be shown on enemies" };

	inline std::string SlowTrap1{ "Spawns a trap that slows down enemies" };
	inline std::string SlowTrap2{ "The trap will always be on front of player" };

	inline std::string Detect1{ "Can detect position of enemies with sound bomb" };
	inline std::string Detect2{ "The positions will be shown if bomb is touched " };

	inline std::string Push1{ "Push enemies with strong wind" };
	inline std::string Push2{ "Enemies will be pushed to the end" };

	inline std::string Dash1{ "Do a short dash to the front" };
	inline std::string Dash2{ "Will be invincible while dashing" };
	inline std::string Dash3{ "Press space bar to dash, 5 sec cooldown" };

	inline std::string Flashbomb1{ "Throw a flash bomb on player's position" };
	inline std::string Flashbomb2{ "Will stun enemies inside the bomb range" };

	struct SkillString {
		SkillString(SkillType skilltype) {
			switch (skilltype) {
			case SkillType::Null:
				SS1 = Null; SS2 = Null; SS3 = Null;
				break;
			case SkillType::Drone:
				SS1 = Drone1; SS2 = Drone2; SS3 = Drone3;
				break;
			case SkillType::DroneSound:
				SS1 = DroneSound1; SS2 = DroneSound2; SS3 = Null;
				break;
			case SkillType::Chameleon:
				SS1 = Chameleon1; SS2 = Chameleon2; SS3 = Null;
				break;
			case SkillType::Invisible:
				SS1 = Invisible1; SS2 = Invisible2; SS3 = Null;
				break;
			case SkillType::Visualize:
				SS1 = Visualize1; SS2 = Visualize2; SS3 = Null;
				break;
			case SkillType::SilentRun:
				SS1 = SilentRun1; SS2 = SilentRun2; SS3 = Null;
				break;
			case SkillType::Assassinate:
				SS1 = Assassinate1; SS2 = Assassinate2; SS3 = Null;
				break;
			case SkillType::SlowTrap:
				SS1 = SlowTrap1; SS2 = SlowTrap2; SS3 = Null;
				break;
			case SkillType::Detect:
				SS1 = Detect1; SS2 = Detect2; SS3 = Null;
				break;
			case SkillType::Push:
				SS1 = Push1; SS2 = Push2; SS3 = Null;
				break;
			case SkillType::Dash:
				SS1 = Dash1; SS2 = Dash2; SS3 = Dash3;
				break; 
			case SkillType::Flashbomb:
				SS1 = Flashbomb1; SS2 = Flashbomb2; SS3 = Null;
				break;
			}
		}
		std::string SS1;
		std::string SS2;
		std::string SS3;
	};
}