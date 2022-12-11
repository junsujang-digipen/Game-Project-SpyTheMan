/*--------------------------------------------------------------
File Name: SilentRunSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 4/13/2021
-----------------------------------------------------------------*/
#include "SilentRunSequence.h"
#include "../Player.h"
#include "../Engine/Engine.h"

SilentRunSequence::SilentRunSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateSilentRun);
}

void SilentRunSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void SilentRunSequence::State_SilentRun::Enter(SilentRunSequence*) {

}

void SilentRunSequence::State_SilentRun::Update(SilentRunSequence*, double) {

}

void SilentRunSequence::State_SilentRun::TestForExit(SilentRunSequence* sq) {
	Player* player = static_cast<Player*>(sq->obj1);
	player->SetSilent(true);
	sq->setIsValid(false);
}