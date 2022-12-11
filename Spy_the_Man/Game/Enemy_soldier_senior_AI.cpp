/*--------------------------------------------------------------
File Name: Enemy_soldier_senior_AI.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/10/2021
-----------------------------------------------------------------*/
#include "Enemy_soldier_senior_AI.h"

Enemy_soldier_senior_AI::Enemy_soldier_senior_AI(PM::GameObject* AIOwner):Enemy_soldier_AI(AIOwner)
{
}
void Enemy_soldier_senior_AI::taskSelectAlgo(double /*dt*/)
{
	//EnemySoldier* Enemy = dynamic_cast<EnemySoldier*>(AIOwner);

	//if (taskAI != nullptr) {
	//	taskAI->Update(dt);
	//	//Enemy->sight = { ai->AIOwner->GetVelocity(),0 };//시야 처리
	//	if (ai->taskAI->getIsValid() == false) {
	//		Engine::GetLogger().LogDebug("End Task AI");
	//		ai->taskAI = nullptr;
	//	}
	//}
	//else {
	//	for (auto& t : Enemy->TaskAIComponents) {//task selection
	//		if (t->getIsValid() == true) {
	//			Engine::GetLogger().LogDebug("Change Task AI ");
	//			taskAI = t;

	//			break;
	//		}
	//	}
	//}
}