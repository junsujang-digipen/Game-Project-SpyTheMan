/*--------------------------------------------------------------
File Name: FindSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/29/2021
-----------------------------------------------------------------*/
#include "FindSequence.h"
#include "../Player.h"
#include "../Engine/Engine.h"
#include "../Engine/World.h"
#include "../Object/Action/ActionManager.h"

FindSequence::FindSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateFind);
}

void FindSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void FindSequence::State_Find::Enter(FindSequence* sq) {
	Player* player = static_cast<Player*>(sq->obj1);
	sq->find = new Find(player->GetCenter());
	Engine::GetGSComponent<PM::World>()->Add(sq->find);
}

void FindSequence::State_Find::Update(FindSequence*, double) {
}

void FindSequence::State_Find::TestForExit(FindSequence* sq) {
	Engine::GetGSComponent<ActionManager>()->GetAction(SkillType::Detect)->SetTimer(0);
	sq->setIsValid(false);
}