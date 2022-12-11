#include "LevelSoundManager.h"
#include "../Engine/Engine.h"
#include "Player.h"
LevelSoundManager::LevelSoundManager()
{
	world = Engine::GetGSComponent<PM::World>();
	player = dynamic_cast<Player*>(world->GetPlayerPtr());
	NPC = world->GetAllNPC();
}


void LevelSoundManager::Update(double)
{
	
}
