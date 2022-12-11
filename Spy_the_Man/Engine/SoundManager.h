#pragma once
/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SoundManager.h
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 10/3/2021
-----------------------------------------------------------------*/
#include <SFML/Audio.hpp>
#include <glm/glm.hpp>
#include <map>
#include <queue>
#include "Component.h"
#include "World.h"
#include "TransformMatrix.h"

namespace PM {
	class SoundManager {

		std::map<std::string, sf::SoundBuffer> SoundBuffers;
		std::map<std::string, sf::Sound> Sounds;

		std::queue<std::string> soundQue;

		std::map<std::string, sf::Music> BGM;
		sf::Music* nowBGM = nullptr;
		sf::Music* tempBGM = nullptr;
		glm::vec2 BGMdistance = { 400, 3000 };

		float sfxvolume = 1;
		float bgmvolume = 1;

		bool stop = false;

	public:
		virtual void Setup();
		void LoadSound(const std::string& path, std::string soundname);
		void LoadBGM(const std::string& path, std::string soundname);

		void PlaySound(std::string soundname, glm::vec2 pos = {}, float z = -2);
		void PlayBGM(std::string soundname);

		void Pause();

		bool SoundFinished(std::string soundname);

		//1 ~ 10
		void setGlobalVolume(int volume);
		void setBGMVolume(int volume);

		int getGlobalVolume() { return static_cast<int>(sfxvolume * 10); }
		int getBGMVolume() { return static_cast<int>(bgmvolume * 10); }

		void setSoundVolume(std::string soundname, float);
		void setSoundLoop(std::string soundname, bool);
		void setSoundPitch(std::string soundname, float);
		void setBGMOffset(std::string soundname, float);
		void setSoundListener(glm::vec2 pos);
		void setStop(bool s) { stop = s; }

		void Update();
		void Reset();
	};
};