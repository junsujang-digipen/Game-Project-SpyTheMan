/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include "..\Engine\GameState.h"

namespace PM
{
	class World;
}

class Player;

class Tuto : public PM::GameState {
public:
	Tuto();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;
	std::string GetName() override { return "Level1"; }

private:
	void MapLoad();
	void FloorF1RoomLoad();
	void FloorF1ObjLoad();

	void FloorF2RoomLoad();
	void FloorF2ObjLoad();

	Player* playerptr{ nullptr };
	PM::World* world{ nullptr };
};