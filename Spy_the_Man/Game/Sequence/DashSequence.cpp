/*--------------------------------------------------------------
File Name: DashSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/24/2021
-----------------------------------------------------------------*/
#include "DashSequence.h"
#include "../Player.h"
#include "../Engine/Engine.h"
#include "../../Engine/SoundManager.h"

DashSequence::DashSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateDash);
}

void DashSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void DashSequence::State_Dash::Enter(DashSequence*) {

}

void DashSequence::State_Dash::Update(DashSequence*, double) {
	
}

void DashSequence::State_Dash::TestForExit(DashSequence* sq) {
	Player* player = static_cast<Player*>(sq->obj1);
	player->SetDash(true);
	sq->setIsValid(false);
}