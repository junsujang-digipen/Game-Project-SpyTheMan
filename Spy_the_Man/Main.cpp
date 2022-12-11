/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: main.cpp
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include <Windows.h>
#include <stdexcept>	//std::exception
#include <iostream>		//std::cerr

#include <imgui.h>
#include "Engine/Engine.h"
#include "Game/Splash.h"


#include "Game/SkillChoose.h"
#include "Game/MainMenu.h"


#define _CRTDBG_MAP_ALLOC

#include <crtdbg.h>

#include "Game/Level/Tuto.h"
#include "Game/LevelEditor.h"
#include "Game/Level/LoadLevel.h"
#include "Game/Credit.h"
#include "Game/LevelSelect.h"

#ifdef _DEBUG
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif

int main(void) {
	try {

#ifndef _DEBUG
		auto hWndConsole = GetConsoleWindow();
		ShowWindow(hWndConsole, SW_HIDE);
#endif
		Engine& engine = Engine::Instance();
		engine.Init("Spy the Man");
		Splash splash;
		
		SkillChoose skill_choose;
		MainMenu mainmenu;
		Tuto Tuto;

		
		LevelEditor level_editor;
		LoadLevel Load_Level;
		LevelSelect levelSelect;
		Credit credit;
		
		engine.GetGameStateManager().AddGameState(splash);
		engine.GetGameStateManager().AddGameState(mainmenu);
		engine.GetGameStateManager().AddGameState(skill_choose);
		engine.GetGameStateManager().AddGameState(Tuto); // tuto
		engine.GetGameStateManager().AddGameState(level_editor);
		engine.GetGameStateManager().AddGameState(Load_Level);
		engine.GetGameStateManager().AddGameState(credit);
		engine.GetGameStateManager().AddGameState(levelSelect);
		/* original

		 */
		while (engine.HasGameEnded() == false) {
			engine.Update();
		}
		engine.Shutdown();
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
		return 0;
	} catch (std::exception& e) {
		std::cerr << e.what() << "\n";
		return -1;
	}
}
