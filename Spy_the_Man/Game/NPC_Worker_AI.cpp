/*--------------------------------------------------------------
File Name: NPC_Worker_AI.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 12/7/2021
-----------------------------------------------------------------*/
#include "NPC_Worker_AI.h"
#include "Basic_NPC.h"
#include "Player.h"
#include "../Engine/Helper_func_map.h"
#include "../Engine/Helper_func_vec.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"

#include "WorldGuardLevel.h"
#include "Master_AI.h"
#include "../Engine/Wave.h"

void NPC_Worker_AI::start()
{
	ChangeState(&stateWandering);
}

void NPC_Worker_AI::taskSelectAlgo(double dt)
{
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(AIOwner);
	if (npc->getHasImportantTask() == true) {
		taskAI = npc->getSelectedTask();
	}

	if (taskAI != nullptr) {
		taskAI->Update(dt);
		if (taskAI->getIsValid() == false) {
			taskAI = nullptr;
		}
	}
	else {
		taskAI = npc->getSelectedTask();
	}
}

NPC_Worker_AI::NPC_Worker_AI(PM::GameObject* AIOwner) :AI(AIOwner) {
	ChangeState(&stateStart);
}

//void Enemy_soldier_AI::Update(double){}


void NPC_Worker_AI::State_Wandering::Enter(NPC_Worker_AI* ai) {

	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	npc->setMarkState(0);
	npc->SetVelocity(glm::vec2{});
	//Engine::GetLogger().LogDebug("Enter " + GetName());
}
void NPC_Worker_AI::State_Wandering::Update(NPC_Worker_AI* ai, double dt) {
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	ai->taskSelectAlgo(dt);
	if (npc->canSeePlayer() == true 
		&& npc->sameRoomCheck(npc->getPlayerPtr()) == true
		&& npc->seeing() == true) {
		npc->updateGaze(npc->GazingSpeed(dt));
	}
	else {
		npc->updateGaze(-1.);
		if (npc->getGaze() < 0) {
			npc->setGaze(0.);
		}
	}
	
}//will take taskCompo and update
void NPC_Worker_AI::State_Wandering::TestForExit(NPC_Worker_AI* ai) {
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	if (npc->getGaze() > 10) {
		ai->ChangeState(&ai->stateSuspicion);
	}
}

void NPC_Worker_AI::State_Suspicion::Enter(NPC_Worker_AI* ai) {
	//Engine::GetLogger().LogDebug("Enter " + GetName());
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	//Enemy->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
	npc->SetVelocity(glm::vec2{ 0,0 });
	
}//
void NPC_Worker_AI::State_Suspicion::Update(NPC_Worker_AI* ai, double dt) {
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	PM::GameObject* player = npc->getPlayerPtr();
	npc->setSight(glm::vec3{ glm::vec2{ player->GetPosition() - npc->GetPosition() },0 });
	if (npc->seeing() == true && npc->canSeePlayer() == true) {
		npc->updateGaze(npc->GazingSpeed(dt));
	}
	else {
		npc->updateGaze(-0.3);
	}
	npc->setMarkState(1);
}//check the dist player to NPC and player's clothes to increase individual warrning level.
void NPC_Worker_AI::State_Suspicion::TestForExit(NPC_Worker_AI* ai) {
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	const double nowGaze{ npc->getGaze() };
	if (nowGaze > 50) {
		ai->ChangeState(&ai->stateAdditionalSuspicion);
	}
	else if (nowGaze <= 10) {
		ai->ChangeState(&ai->stateWandering);
	}
}

void NPC_Worker_AI::State_Additional_Suspicion::Enter(NPC_Worker_AI* /*ai*/)
{
	//Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	
}

void NPC_Worker_AI::State_Additional_Suspicion::Update(NPC_Worker_AI* ai, double dt)
{
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	PM::GameObject* player = npc->getPlayerPtr();
	npc->setMarkState(1);
	glm::vec2 direc{ player->GetPosition() - npc->GetPosition() };
	if (direc.x* direc.x > npc->GetSize().x* npc->GetSize().x ) {
		npc->go(player->GetPosition(), npc->getSpeed());
	}
	else {
		npc->SetVelocity({});
		npc->setSight(glm::vec3{ direc,0 });
	}

	if (npc->seeing() == true && npc->canSeePlayer() == true) {
		npc->updateGaze(npc->GazingSpeed(dt));
	}
	else {
		npc->updateGaze(-0.3);
	}

}

void NPC_Worker_AI::State_Additional_Suspicion::TestForExit(NPC_Worker_AI* ai)
{
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);

	const double nowGaze{ npc->getGaze() };
	if (nowGaze >=100) {
		npc->setGaze(100.);
		ai->ChangeState(&ai->stateRunAway);
		//->indiIncreaseGuardLevelSpeed += 0.1;
	}
	else if (nowGaze <= 50) {
		ai->ChangeState(&ai->stateSuspicion);

	}
}


void NPC_Worker_AI::State_RunAway::Enter(NPC_Worker_AI* ai) {
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	Engine::GetSoundManager().PlaySound("scream",npc->GetPosition());
	PM::GameObject* player = npc->getPlayerPtr();
	
	glm::vec2 direc{ player->GetPosition() - npc->GetPosition() };
	npc->SetVelocity({ -direc.x / std::abs(direc.x) * npc->getRunSpeed(),0 });
	npc->setSight({ npc->GetVelocity().x,0,0 });


	new PM::Wave({ npc->GetCenter().x, npc->GetCenter().y + npc->GetSize().y/3 }, 6.5f, 2.5);
	timer = timerMax;
	//npc->SetVelocity({});
	//npc->indiIncreaseGuardLevelSpeed += 1.;
	// TODO Engine::GetGSComponent<Master_AI>()->callEnemy(npc,static_cast<int>(/*npc->indiIncreaseGuardLevelSpeed*/1));
	//npc->GetGOComponent<PM::Sprite>()->PlayAnimation(3);
}//increase the entire warrning level
void NPC_Worker_AI::State_RunAway::Update(NPC_Worker_AI* ai, double dt) {
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	PM::GameObject* player = npc->getPlayerPtr();
	npc->setMarkState(2);
	glm::vec2 direc{ player->GetPosition() - npc->GetPosition() };
	if (direc.x * npc->GetVelocity().x < 0) {
		//npc->SetVelocity({ -direc.x / std::abs(direc.x) * npc->getRunSpeed(),0 });
		//npc->setSight({ npc->GetVelocity().x,0,0 });
		//if (npc->GetGOComponent<PM::Sprite>()->GetCurrentAnim() != 5) {
		//	npc->GetGOComponent<PM::Sprite>()->PlayAnimation(5);
		//}
	}
	else {
		{
			npc->SetVelocity({});

			//TODO screamming
			if (npc->GetGOComponent<PM::Sprite>()->GetCurrentAnim() != 3) {
				npc->GetGOComponent<PM::Sprite>()->PlayAnimation(3);
			}
		}
		if (timer >0.) {
			timer -= dt;
		}
	}
}
void NPC_Worker_AI::State_RunAway::TestForExit(NPC_Worker_AI* ai) {
	Basic_NPC* npc = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	//PM::GameObject* player = npc->player;
	if (timer <= 0) {
		ai->ChangeState(&ai->stateAdditionalSuspicion);
		npc->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
	}
}