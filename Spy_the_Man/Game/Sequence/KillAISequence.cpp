/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Kill_AI.cpp
Project: CS200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#include "KillAISequence.h"
#include "../Player.h"
#include "../Engine/Engine.h"
#include "../Master_AI.h"
#include "../Basic_NPC.h"
#include "../../Engine/SoundManager.h"

namespace PM
{
	class UIManager;
}

Kill_AI::Kill_AI(PM::SequenceType type, PM::GameObject* player , PM::GameObject* enemy) : Sequence_base(type, player, enemy)
{
	currState = &state_walking_;
	currState->Enter(this);
}

void Kill_AI::Update(double dt)
{
	currState->Update(this,dt);
	currState->TestForExit(this);
}


void Kill_AI::State_walking::Enter(Kill_AI* kill)
{
	if (kill->enemyDeath == false) {
		static_cast<Player*>(kill->obj1)->GoTo(kill->obj2->GetPosition());
	}
}
void Kill_AI::State_walking::Update(Kill_AI* /*kill*/, double /*dt*/)
{
	
}
void Kill_AI::State_walking::TestForExit(Kill_AI* kill)
{
	Player* player = static_cast<Player*>(kill->obj1);
	Basic_NPC* enemySoldier = static_cast<Basic_NPC*>(kill->obj2);

	const glm::vec2 pPos = player->GetPosition();
	const glm::vec2 ePos = enemySoldier->GetPosition();
	const glm::vec2 pSize = player->GetSize();
	const glm::vec2 eSize = enemySoldier->GetSize();

	if(pPos.x+ pSize.x >= ePos.x + eSize.x/2 && pPos.x < ePos.x +eSize.x/2 )
	{
		player->SetGoTo(false);
		kill->ChangeState(&kill->state_kill_);
	}
}


void Kill_AI::State_kill::Enter(Kill_AI* kill)
{
	Engine::GetSoundManager().PlaySound("kill", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	kill->enemyDeath = true;
}
void Kill_AI::State_kill::Update(Kill_AI* kill, double dt)
{
	if(kill->enemyDeath == true)
	{
		kill->timer += dt;
	}
}
void Kill_AI::State_kill::TestForExit(Kill_AI* kill)
{
	//Player* player = static_cast<Player*>(kill->obj1);
	Basic_NPC* enemySoldier = static_cast<Basic_NPC*>(kill->obj2);
	if (kill->timer >= 1)
	{
		kill->enemyDeath = false;
	}
	Engine::GetGSComponent<Master_AI>()->removeNPC(enemySoldier);
	/*PM::UI* ui = enemySoldier->GetGOComponent<PM::UI>();
	std::list<PM::UI*>& uis = Engine::GetGSComponent<PM::UIManager>()->uis();
	uis.remove(ui);
	*/
	kill->setIsValid(false);
}

