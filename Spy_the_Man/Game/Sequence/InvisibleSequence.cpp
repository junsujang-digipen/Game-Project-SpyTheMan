/*--------------------------------------------------------------
File Name: InvisibleSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 3/15/2021
-----------------------------------------------------------------*/
#include "InvisibleSequence.h"
#include "../Player.h"
#include "../Engine/Engine.h"
#include "../Engine/Effect/EffectManager.h"
#include "../Engine/Effect/Effect.h"
#include "../Object/Action/ActionManager.h"
#include "../../Engine/SoundManager.h"

InvisibleSequence::InvisibleSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateInvisible);
}

void InvisibleSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void InvisibleSequence::State_Invisible::Enter(InvisibleSequence* sq) {
	Engine::GetSoundManager().PlaySound("invisible", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
	Player* player = static_cast<Player*>(sq->obj1);
	player->SetInvisible(true);
	player->SetHide(true);
	Engine::GetGSComponent<EffectManager>()->Add(new InvisibleEffect({player->GetPosition(),player->GetPosition() + player->GetSize()}));
}

void InvisibleSequence::State_Invisible::Update(InvisibleSequence*, double) {
}

void InvisibleSequence::State_Invisible::TestForExit(InvisibleSequence* sq) {
	Engine::GetGSComponent<ActionManager>()->GetAction(SkillType::Invisible)->SetTimer(0);
	sq->setIsValid(false);
}