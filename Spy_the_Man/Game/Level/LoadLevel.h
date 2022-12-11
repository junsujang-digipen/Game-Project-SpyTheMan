/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: newLevel.h
Project: Spy the Man
Author: 
Creation date: 5/2/2022
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include "..\Engine\GameState.h"

namespace PM
{
	class Texture;
	class LevelData;
	class World;
}

class Player;

class LoadLevel : public PM::GameState {
public:
	LoadLevel();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;
	std::string GetName() override { return "LoadLevel"; }

	void LoadObj(PM::LevelData* data);
	void LoadRoom(PM::LevelData* data);
	
private:
	Player* playerptr{nullptr};
	PM::World* world{ nullptr };
	PM::Texture* BackgroundBackPtr{ nullptr };
	PM::Texture* BackgroundMiddlePtr{ nullptr };
	PM::Texture* BackgroundFrontPtr{ nullptr };
	bool openingAni = false;

	glm::dvec2 OpeningRectPos{ 0,0 };
};