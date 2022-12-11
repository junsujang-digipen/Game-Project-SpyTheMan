/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameStateManager.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include "Engine.h"			//logger
#include "GameStateManager.h"
#include "GameState.h"
#include <iostream>

#include "World.h"
#include "../Game/Screens.h"
PM::GameStateManager::GameStateManager() : currGameState(nullptr), nextGameState(nullptr), state(State::START) {}

void PM::GameStateManager::AddGameState(GameState& gameState) {
	gameStates.push_back(&gameState);
}

void PM::GameStateManager::Update(double dt) {
	switch (state) {
	case State::START:
		if (gameStates.size() == 0) {
			Engine::GetLogger().LogError("No States have been loaded");
			state = State::SHUTDOWN;
		} else {
			nextGameState = gameStates[0];
			state = State::LOAD;
		}
		break;
	case State::LOAD:
		currGameState = nextGameState;
		Engine::GetLogger().LogEvent("Load " + currGameState->GetName());
		currGameState->Load();
		Engine::GetLogger().LogEvent("Load Complete");
		state = State::UPDATE;
		break;
	case State::UPDATE:
		if (currGameState != nextGameState) {
			state = State::UNLOAD;
		}
		else {
			Engine::GetLogger().LogVerbose("Update " + currGameState->GetName());
			currGameState->Update(dt);
			if (GetGSComponent<PM::World>() != nullptr) {
				GetGSComponent<PM::World>()->CollideTest(); 
			}
			currGameState->Draw();
			
			Engine::GetWindow().swapBuffers();
			Engine::GetPoodle().glsetting.Clear();
		}
		break;
	case State::UNLOAD:
		Engine::GetLogger().LogEvent("Unload " + currGameState->GetName());
		currGameState->Unload();
		if (nextGameState != currGameState) {
			Engine::GetTextureManager().Unload();
		}
		if (nextGameState == nullptr) {
			state = State::SHUTDOWN;
			break;
		}
		Engine::GetSoundManager().Reset();
		Engine::GetSkillManager().CurrSkills().clear();
		state = State::LOAD;
		break;
	case State::SHUTDOWN:
		state = State::EXIT;
		break;
	case State::EXIT:
		break;
	}
}

void PM::GameStateManager::SetNextState(int initState) {
	nextGameState = gameStates[initState];
}

void PM::GameStateManager::SetLevelState(std::string name, std::string path)
{
	if(Engine::GetLevelIOManager().HaveLevel(name, path) == false)
	{
		Engine::GetLogger().LogError("Can't Find Level: " + name);
		return;
	}
	Engine::GetLevelIOManager().SetNextLevel(name, path);
	
	nextGameState = gameStates[static_cast<int>(Screens::DataToLevel)];
}


void PM::GameStateManager::ReloadState() {
	state = State::UNLOAD;
}

void PM::GameStateManager::Shutdown() {
	state = State::UNLOAD;
	nextGameState = nullptr;
}
