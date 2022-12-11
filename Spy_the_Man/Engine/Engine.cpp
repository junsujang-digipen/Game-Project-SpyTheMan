/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Engine.cpp
Purpose: This is the service provider for our games engine services
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#include <iostream>
#include <random>
#include "Engine.h"


Engine::Engine() : frameCount(0), lastTick(std::chrono::system_clock::now()),
#ifdef _DEBUG				
logger(PM::Logger::Severity::Debug, true, lastTick, "Trace.log")
#else 						
logger(PM::Logger::Severity::Event, false, lastTick,"Trace.log")
#endif
{
std::cout << "engine make" << std::endl;
std::cout << "end" << std::endl;
}

Engine::~Engine() {
}

void Engine::Init(std::string windowName) {
	logger.LogEvent("Engine Init");
	
	Seed = static_cast<unsigned int>(time(NULL));
	if (!window.Init(windowName,1600,900)) {
		std::cout << "Failed to window init" << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	fpsCalcTime = lastTick;

	int seed = static_cast<unsigned int>(time(NULL));
	srand(seed);
	Engine::GetLogger().LogEvent("Seed = " + std::to_string(seed));
	camera.init();
	Engine::GetPoodle().shape_init();
	font = new PM::Font(GetWindow().GetSize().x, GetWindow().GetSize().y);
	Engine::GetSkillManager().LoadGS();
	Engine::GetSoundManager().Setup();
}

void Engine::Shutdown() {
	logger.LogEvent("Engine Shutdown");
	delete font;
	textureManager.Unload();
	window.windowDown();
	
}

void Engine::Update() {
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	double dt = std::chrono::duration<double>(now - lastTick).count();
	deltaTime = dt;
#ifdef _DEBUG
	if (dt > 2 / Engine::Target_FPS) {
		dt = 1 / Engine::Target_FPS;
	}
#endif

	if (dt >= 1. / Engine::Target_FPS) 	{
		logger.LogVerbose("Engine Update");
		times += dt;
		lastTick = now;

		if (++frameCount >= Engine::FPS_IntervalFrameCount) {
			double actualTime = std::chrono::duration<double>((now - fpsCalcTime)).count();
			logger.LogEvent("FPS:  " + std::to_string(frameCount / actualTime));
			frameCount = 0;
			fpsCalcTime = now;
		}
		window.Update();
		camera.Update(dt);
		gameStateManager.Update(dt);
		input.Update();
		soundManager.Update();
	}

	if(window.windowShouldClosed() && WindowClosed == false)
	{
		gameStateManager.Shutdown();
		WindowClosed = true;
	}
}

double Engine::RanGenerator(double min, double max)
{
	static std::knuth_b knuthrand(Seed);
	std::uniform_real_distribution<double> distribution(min, max);
	return distribution(knuthrand);
}

bool Engine::HasGameEnded() {
	return gameStateManager.HasGameEnded(); 
}
