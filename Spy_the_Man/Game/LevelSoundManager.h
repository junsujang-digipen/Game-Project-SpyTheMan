#pragma once
#include <vector>

#include "../Engine/Component.h"
#include "../Engine/GameObject.h"

class Player;

namespace PM {
	class World;
}

class LevelSoundManager : public PM::Component
{
public:

	LevelSoundManager();
	void Update(double) override;

	PM::World* world;
	Player* player;
	std::vector<PM::GameObject*> NPC;
};
