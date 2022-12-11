/*--------------------------------------------------------------
File Name: Enemy_soldier_AI.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 9/24/2021
-----------------------------------------------------------------*/
#include "Enemy_soldier_AI.h"
#include "Basic_NPC.h"
#include "Player.h"
#include "../Engine/Helper_func_map.h"
#include "../Engine/Helper_func_vec.h"
#include "../Engine/Sprite.h"
#include "../Engine/Engine.h"

#include "WorldGuardLevel.h"

#include "Task_AI_pointToPoint.h"

void Enemy_soldier_AI::start()
{
	ChangeState(&stateWandering);
}

void Enemy_soldier_AI::taskSelectAlgo(double dt)
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

Enemy_soldier_AI::Enemy_soldier_AI(PM::GameObject* AIOwner) :AI(AIOwner) {
	ChangeState(&stateStart);
}

//void Enemy_soldier_AI::Update(double){}


void Enemy_soldier_AI::State_Wandering::Enter(Enemy_soldier_AI* ai) {
	
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	//Enemy->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
	Enemy->SetVelocity(glm::vec2{});
	Engine::GetLogger().LogDebug("Enter "+GetName());
	Enemy->setTalkResult(0);
	Enemy->setMarkState(0);
}
void Enemy_soldier_AI::State_Wandering::Update(Enemy_soldier_AI* ai , double dt) {
	//Enemy_soldier_AI* ai = dynamic_cast<Enemy_soldier_AI*>(esa);
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	
	//PM::GameObject* owner = ai->AIOwner;
	ai->taskSelectAlgo(dt);
	//take taskAI and act by using fsm 

	if (Enemy->canSeePlayer() == true
		&& Enemy->sameRoomCheck(Enemy->getPlayerPtr()) == true
		&& Enemy->seeing() == true) {
		Enemy->updateGaze(Enemy->GazingSpeed(dt));
	}
	else {
		Enemy->updateGaze(-1.);
		if (Enemy->getGaze()<0) {
			Enemy->setGaze(0.);
		}
	}
}//will take taskCompo and update
void Enemy_soldier_AI::State_Wandering::TestForExit(Enemy_soldier_AI* ai) {
	PM::GameObject* owner = ai->AIOwner;
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(owner);
	
	//sight check
	if (Enemy->getGaze() > 10) {
		ai->ChangeState(&ai->stateSuspicion);
	}
	if (Enemy->getCheckPos().z == 1) {
		ai->ChangeState(&ai->stateGo);
		ai->stateGo.setDest(glm::vec2{ Enemy->getCheckPos() });
	}
	
	//Sound check -> supicion
	//warning level check -> about chasing
	//calling check -> chasing 

}//if player is on same Room and the NPC is seeing player -> suspicion

void Enemy_soldier_AI::State_Suspicion::Enter(Enemy_soldier_AI* ai) {

	Engine::GetLogger().LogDebug("Enter " + GetName());

	PM::GameObject* owner = ai->AIOwner;
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(owner);
	//Enemy->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
	Enemy->SetVelocity(glm::vec2{0,0});

}//
void Enemy_soldier_AI::State_Suspicion::Update(Enemy_soldier_AI* ai, double dt) {
	PM::GameObject* owner = ai->AIOwner;
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(owner);
	PM::GameObject* player = dynamic_cast<Basic_NPC*>(owner)->getPlayerPtr();
	Enemy->setSight(glm::vec3{ glm::vec2{ player->GetPosition() - Enemy->GetPosition() },0 });
	if (Enemy->getTalkResult() != 1 
		&& Enemy->seeing()==true
		&& Enemy->canSeePlayer() == true) {
		Enemy->updateGaze(Enemy->GazingSpeed(dt));
	}
	else {
		Enemy->updateGaze( -0.3 );
	}
	Enemy->setMarkState(1);
	//Enemy->setSight({player->GetPosition()- Enemy->GetPosition(),0 });
	//const double max{ dynamic_cast<EnemySoldier*>(owner)->maxGaze };
	//Enemy->warning1Color.r = (float)(Sgaze / max * 255);
	//Enemy->warning1Color.g = (float)(Sgaze / max * 255);

}//check the dist player to NPC and player's clothes to increase individual warrning level.
void Enemy_soldier_AI::State_Suspicion::TestForExit(Enemy_soldier_AI* ai) {
	PM::GameObject* owner = ai->AIOwner;
	//Player* player = dynamic_cast<Player*>(dynamic_cast<EnemySoldier*>(owner)->player);
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(owner);

	//player->GetDisguise() == DisguiseList::Worker
	const double nowGaze{ Enemy->getGaze() };
	if (nowGaze > 50) {
		ai->ChangeState(&ai->stateAdditionalSuspicion);
	}
	else if (nowGaze <=10) {
		ai->ChangeState(&ai->stateWandering);

	}

}// if the individual warrning level over the max num -> attack // if the player go to another Room or behind of the NPC -> wandering

void Enemy_soldier_AI::State_Additional_Suspicion::Enter(Enemy_soldier_AI* /*ai*/)
{
	//Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);

	Engine::GetLogger().LogDebug("Enter " + GetName());
	//Enemy->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
	

}

void Enemy_soldier_AI::State_Additional_Suspicion::Update(Enemy_soldier_AI* ai, double dt)
{
	PM::GameObject* owner = ai->AIOwner;
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(owner);
	PM::GameObject* player = dynamic_cast<Basic_NPC*>(owner)->getPlayerPtr();

	glm::vec2 direc{ player->GetPosition() - Enemy->GetPosition() };
	if (glm::length(direc) > Enemy->GetSize().x) {
		Enemy->go(player->GetPosition(), Enemy->getSpeed());
	}
	else {
		Enemy->SetVelocity({});
		Enemy->setSight(glm::vec3{ direc,0 });
	}
	
	if (Enemy->getTalkResult() != 1 
		&& Enemy->seeing() == true
		&& Enemy->canSeePlayer() == true) {
		Enemy->updateGaze(Enemy->GazingSpeed(dt));
	}
	else {
		Enemy->updateGaze(-0.3);
	}
	Enemy->setMarkState(1);
}

void Enemy_soldier_AI::State_Additional_Suspicion::TestForExit(Enemy_soldier_AI* ai)
{
	PM::GameObject* owner = ai->AIOwner;
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(owner);

	const double nowGaze{ Enemy->getGaze() };
	if (nowGaze >= 100) {
		Enemy->setGaze(100.);
		ai->ChangeState(&ai->stateAttack);
		//Enemy->indiIncreaseGuardLevelSpeed += 0.1;
	}
	else if (nowGaze <= 50) {
		ai->ChangeState(&ai->stateSuspicion);

	}
}


void Enemy_soldier_AI::State_Attack::Enter(Enemy_soldier_AI* ai ) {
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	Engine::GetSoundManager().PlaySound("notice", Enemy->GetPosition());
}//increase the entire warrning level
void Enemy_soldier_AI::State_Attack::Update(Enemy_soldier_AI* ai, double dt) {
	PM::GameObject* owner = ai->AIOwner;
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(owner);
	PM::GameObject* player = Enemy->getPlayerPtr();
	//const glm::vec2 targetPos = player->GetPosition();
	//const double dist = targetPos.x - owner->GetPosition().x;
	//const double gazeSpeed{ 1 / dist * dist };
	//double speed = Enemy->speed;
	Enemy->setMarkState(2);
	if (Enemy->chasing(player, Enemy->getRunSpeed(), dt)==true) {
		// change state to inquire
		/*if (Enemy->getTalkResult() == 0) {
			toInquire = true;
		}
		else */{
			dynamic_cast<Player*>(player)->SetDead(true);
		}
		
	}
	//Enemy->chasing(player, Enemy->speed, dt);

	//const double max{ Enemy->maxGaze };
	//dynamic_cast<EnemySoldier*>(owner)->warning2Color.r = (float)(Wgaze/max*255);
}// chasing and attack the player
void Enemy_soldier_AI::State_Attack::TestForExit(Enemy_soldier_AI* ai) {
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	PM::GameObject* player = Enemy->getPlayerPtr();
	
	if (Enemy->seeing() == false || Enemy->canSeePlayer() == false) {
		ai->ChangeState(&ai->stateGo);
		ai->stateGo.setDest(player->GetPosition());
		Engine::GetGSComponent<WorldGuardLevel>()->Level += 0.5;
	}
}// player is hiding or run away from chasing area-> wandering

void Enemy_soldier_AI::State_go::setDest(glm::vec2 dest)
{
	destination = dest;
}

void Enemy_soldier_AI::State_go::Enter(Enemy_soldier_AI* /*ai*/)
{
	//EnemySoldier* Enemy = dynamic_cast<EnemySoldier*>(ai->AIOwner);

	//Enemy->GetGOComponent<PM::Sprite>()->PlayAnimation(2);

	destination = {};
}

void Enemy_soldier_AI::State_go::Update(Enemy_soldier_AI*ai, double)
{
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	Enemy->go(destination,Enemy->getRunSpeed());
	Enemy->setMarkState(2);
}

void Enemy_soldier_AI::State_go::TestForExit(Enemy_soldier_AI* ai)
{
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	glm::vec2 dist = glm::vec2(glm::vec2{ destination - Enemy->GetPosition() });
	if (Enemy->canSeePlayer() == true && Enemy->seeing() == true) {
		ai->ChangeState(&ai->stateAttack);
	}
	else if (dist.x* dist.x < Enemy->GetSize().x * Enemy->GetSize().x && dist.y* dist.y < (ROOMYSIZE)* (ROOMYSIZE)/4) {
		ai->ChangeState(&ai->stateCheckAround);
		destination = {};
		Enemy->checkPosition({});
	}
}

void Enemy_soldier_AI::State_checkAround::Enter(Enemy_soldier_AI*ai)
{
	checkNum = 0;
	chasingTime = 0;
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	//Enemy->GetGOComponent<PM::Sprite>()->PlayAnimation(1);
	arc = Enemy->getAroundChunk();
	setCheckAI(new Task_AI_pointToPoint{ ai->AIOwner });
	checkAroundAI->start();
	
	
}

void Enemy_soldier_AI::State_checkAround::Update(Enemy_soldier_AI* ai, double dt)
{
	//청크내에서 돌아다니기 todo
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);
	//Enemy->SetVelocity({});
	checkAroundAI->Update(dt);
	chasingTime += dt;
	Enemy->setMarkState(2);
}

void Enemy_soldier_AI::State_checkAround::TestForExit(Enemy_soldier_AI* ai)
{
	Basic_NPC* Enemy = dynamic_cast<Basic_NPC*>(ai->AIOwner);

	if (chasingTime>chaseT) {
		Enemy->setGaze(0.);
		ai->ChangeState(&ai->stateWandering);
		delete checkAroundAI;
		checkAroundAI = nullptr;
		//무전기 시퀀스??
		//월드 가드 레벨 증가
		Engine::GetGSComponent<WorldGuardLevel>()->Level += 0.1;
	}
	else if (Enemy->canSeePlayer() == true && Enemy->seeing() == true) {
		ai->ChangeState(&ai->stateAttack);
		delete checkAroundAI;
		checkAroundAI = nullptr;
	}

}

Enemy_soldier_AI::State_checkAround::~State_checkAround(){
	delete checkAroundAI;
	checkAroundAI = nullptr;
}