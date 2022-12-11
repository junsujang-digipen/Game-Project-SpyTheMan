/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Engine.h
Purpose: This is the service provider for our games engine services
Project: CS230
Author: Kevin Wright
Creation date: 2/10/2021
-----------------------------------------------------------------*/
#pragma once
#include <chrono>
#include "GameStateManager.h"
#include "Input.h"
#include "Window.h"
#include "Logger.h"
#include "Poodle.h"
#include "Camera.h"
#include "Font.h"
#include "TextureManager.h"
#include "SoundManager.h"
#include "IOManager/LevelIOManager.h"
#include "Skill/SkillManager.h"

class Engine {
public:
	static Engine& Instance() {
        static Engine instance; return instance; }
    static PM::Logger& GetLogger() { return Instance().logger; };
    static PM::Input& GetInput() { return Instance().input; }
    static PM::Window& GetWindow() { return Instance().window; }
    static PM::GameStateManager& GetGameStateManager() { return Instance().gameStateManager; }
    static PM::Poodle& GetPoodle() { return Instance().poodle; }
    static PM::Camera& GetCamera() { return Instance().camera; }
    static PM::Font& GetFont() { return *Instance().font; }
    static PM::TextureManager& GetTextureManager() { return Instance().textureManager; }
    static PM::SkillManager& GetSkillManager() { return Instance().skillManager; }
    static PM::SoundManager& GetSoundManager() { return Instance().soundManager; }
    static PM::LevelIOManager& GetLevelIOManager() { return Instance().levelIOManager; }
	static double GetRandomNum(double min, double max) { return Instance().RanGenerator(min, max); }
    void Init(std::string windowName);
    void Shutdown();
    void Update();
    bool HasGameEnded();
    static double GetTime() { return Instance().times; }
	static double GetDeltaTime() { return Instance().deltaTime; }
    static bool GetTitleSkip() { return Instance().TitleSkip; }
	
    static void SetTitleSkip(bool set) { Instance().TitleSkip = set; }
    template<typename T>
    static T* GetGSComponent() { return GetGameStateManager().GetGSComponent<T>(); }
private:
    Engine();
    ~Engine();
   
    unsigned int Seed{};
    double RanGenerator(double min, double max);

    std::chrono::system_clock::time_point lastTick;
    std::chrono::system_clock::time_point fpsCalcTime;
    int frameCount;

    PM::Logger logger;
    PM::GameStateManager gameStateManager;
    PM::Input input;
    PM::Window window;
    PM::Poodle poodle;
    PM::Camera camera;
    PM::Font* font;
    PM::TextureManager textureManager;
	PM::SkillManager skillManager;
    PM::SoundManager soundManager;
    PM::LevelIOManager levelIOManager;
	
    static constexpr double Target_FPS = 60.0;
    static constexpr int FPS_IntervalSec = 5;
    static constexpr int FPS_IntervalFrameCount = static_cast<int>(FPS_IntervalSec * Target_FPS);
	
	double deltaTime = 0;
    double times = 0;
    bool TitleSkip = false;
    bool WindowClosed{ false };
};


namespace PM {
    inline glm::mat3 getCameraMat() { return Engine::GetCamera().getWorldToCameraToNDC(); }
    inline glm::mat3 getSToWMat() { return Engine::GetCamera().getScreenToWorld(); }
    inline GLFWwindow* getWindowPtr() { return Engine::GetWindow().getWindow(); }
}