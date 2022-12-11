/*--------------------------------------------------------------
File Name: VisualSequence.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 3/15/2021
-----------------------------------------------------------------*/
#include "VisualSequence.h"
#include "../Player.h"
#include "../Engine/Engine.h"
#include "../Engine/World.h"
#include "../Engine/Wave.h"
#include "../Object/Action/ActionManager.h"

VisualSequence::VisualSequence(PM::SequenceType type, PM::GameObject* player, PM::GameObject* obj)
	: Sequence_base(type, player, obj)
{
	ChangeState(&stateVisual);
}

void VisualSequence::Update(double dt) {
	currState->Update(this, dt);
	currState->TestForExit(this);
}

void VisualSequence::State_Visual::Enter(VisualSequence*) {

}

void VisualSequence::State_Visual::Update(VisualSequence*, double) {

}

void VisualSequence::State_Visual::TestForExit(VisualSequence* sq) {
//	Player* player = static_cast<Player*>(sq->obj1);
	for (const auto& e : Engine::GetGSComponent<PM::World>()->GetAllNPC()) {
		new PM::Wave(e, 5, 2, e->GetLayer(), false);
	}
	Engine::GetGSComponent<ActionManager>()->GetAction(SkillType::Visualize)->SetTimer(0);
	sq->setIsValid(false);
}