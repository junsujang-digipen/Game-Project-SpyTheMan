/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SoundManager.cpp
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 10/3/2021
-----------------------------------------------------------------*/
#include "SoundManager.h"
#include "Engine.h"
#include "../Game/Master_AI.h"

namespace PM {
	void SoundManager::Setup() {
		//SFX
		LoadSound("assets/sounds/radio.wav", "radio");
		LoadSound("assets/sounds/yeet.wav", "yeet");
		LoadSound("assets/sounds/test1.wav", "dronesound");
		LoadSound("assets/sounds/empty.wav", "empty");

		//SFX add
		LoadSound("assets/sounds/item2A.wav", "rootClear");
		LoadSound("assets/sounds/Menu2A.wav", "checkpointClear");
		LoadSound("assets/sounds/bubble_01.ogg", "invisible");
		LoadSound("assets/sounds/click13.wav", "Chameleon");
		LoadSound("assets/sounds/click7.wav", "droneStart");
		LoadSound("assets/sounds/metal_button_press1.wav", "closet");
		LoadSound("assets/sounds/door-02.flac", "doorOpen");
		LoadSound("assets/sounds/leather_inventory.wav", "disguise");
		LoadSound("assets/sounds/fall.wav", "kill");
		LoadSound("assets/sounds/1KeyStrokeONE.ogg", "UIButton");
		LoadSound("assets/sounds/door-04.flac", "vent");
		LoadSound("assets/sounds/beep_02.ogg", "notice");
		LoadSound("assets/sounds/Clic07.flac", "skillButton");
		LoadSound("assets/sounds/Clic05.flac", "skillDecide");
		LoadSound("assets/sounds/door-06.flac", "BuildingInside");
		LoadSound("assets/sounds/door-03.flac", "stair");
		LoadSound("assets/sounds/door-08.flac", "RoomInside");
		LoadSound("assets/sounds/click35.wav", "SlowTrap");
		LoadSound("assets/sounds/click23.wav", "Dash");
		LoadSound("assets/sounds/machine_03.ogg", "FlashBomb");
		LoadSound("assets/sounds/wscream_2.wav", "scream");
		LoadSound("assets/sounds/splash_08.ogg", "die");
		LoadSound("assets/sounds/wind_4.wav", "Push");
		LoadSound("assets/sounds/stepstone_3.wav", "run2");
		LoadSound("assets/sounds/click11.wav", "holo");

		//BGM
		LoadBGM("assets/sounds/heartbeat.wav", "heart");

		LoadBGM("assets/sounds/back.wav", "back");
		LoadBGM("assets/sounds/BGM/bgm_8.wav", "BGM0");
		LoadBGM("assets/sounds/BGM/bgm_16.wav", "BGM1");

		LoadBGM("assets/sounds/BGM/01 The Treasure.wav", "Title");

		LoadBGM("assets/sounds/BGM/bgm_20.wav", "Detected");

		LoadBGM("assets/sounds/BGM/15 The Restless Chicken.wav", "SelectLevel");
		LoadBGM("assets/sounds/BGM/bgm_4.wav", "Clear");
	}

	void SoundManager::LoadSound(const std::string& path, std::string soundname) {
		if (SoundBuffers.find(soundname) == SoundBuffers.end()) {
			if (SoundBuffers[soundname].loadFromFile(path) == false) {
				throw std::runtime_error("Failed to load " + path);
			}
			else {
				Sounds[soundname].setBuffer(SoundBuffers[soundname]);
				Sounds[soundname].setMinDistance(4);
				Sounds[soundname].setAttenuation(3);
			}
		}
		else {
			Engine::GetLogger().LogEvent(soundname + " already exist.");
		}
	}

	void SoundManager::LoadBGM(const std::string& path, std::string soundname) {
		if (BGM.find(soundname) == BGM.end()) {
			if (BGM[soundname].openFromFile(path) == false) {
				throw std::runtime_error("Failed to load " + path);
			}
			BGM[soundname].setLoop(true);
		}
		else {
			Engine::GetLogger().LogEvent(soundname + " already exist.");
		}
	}

	void SoundManager::PlaySound(std::string soundname, glm::vec2 pos, float z) {
		if (Sounds.find(soundname) != Sounds.end()) {
			if (pos == glm::vec2{ 0, 0 } && z == -2) {
				Sounds[soundname].setVolume(100 * sfxvolume);
				Sounds[soundname].setPosition(sf::Listener::getPosition());
			}
			else {
				Sounds[soundname].setPosition(pos.x / 100, pos.y / 100, -2 + z / 100);
				float distance = static_cast<float>(sqrt(pow(Sounds[soundname].getPosition().x - sf::Listener::getPosition().x, 2) +
					pow(Sounds[soundname].getPosition().y - sf::Listener::getPosition().y, 2) +
					pow(Sounds[soundname].getPosition().z + 2, 2)));
				if (distance >= 12) {
					Sounds[soundname].setVolume(0);
				}
				else if (distance < 12 && distance >= 10) {
					Sounds[soundname].setVolume(35 * sfxvolume);
				}
				else if (distance < 10 && distance <= 8) {
					Sounds[soundname].setVolume(50 * sfxvolume);
				}
				else {
					Sounds[soundname].setVolume(100 * sfxvolume);
				}
			}
			Sounds[soundname].play();
		}
		else {
			throw std::runtime_error(soundname + " does not exist");
		}
	}

	void SoundManager::PlayBGM(std::string soundname) {
		if (BGM.find(soundname) != BGM.end()) {
			if (nowBGM != nullptr)
			{
				if (nowBGM->getStatus() == sf::SoundSource::Status::Playing) {
					nowBGM->pause();
				}
			}
			nowBGM = &BGM[soundname];
			if (nowBGM->getStatus() != sf::SoundSource::Status::Playing) {
				nowBGM->play();
			}
			nowBGM->setVolume(100 * bgmvolume);
		}
		else {
			throw std::runtime_error(soundname + " does not exist");
		}
	}

	void SoundManager::Pause() {
		for (auto& sound : Sounds) {
			if (sound.second.getStatus() == sf::SoundSource::Status::Playing) {
				sound.second.pause();
			}
		}
		if (nowBGM->getStatus() == sf::SoundSource::Status::Playing) {
			nowBGM->pause();
		}
	}

	bool SoundManager::SoundFinished(std::string soundname) {
		if (Sounds.find(soundname) != Sounds.end()) {
			if (Sounds[soundname].getStatus() != sf::SoundSource::Status::Playing) {
				return true;
			}
			return false;
		}
		else {
			throw std::runtime_error(soundname + " does not exist");
		}
	}

	void SoundManager::setGlobalVolume(int volume) {
		sfxvolume = static_cast<float>(volume) / 10;
	}

	void SoundManager::setBGMVolume(int volume) {
		bgmvolume = static_cast<float>(volume) / 10;
	}

	void SoundManager::setSoundVolume(std::string soundname, float value) {
		if (Sounds.find(soundname) != Sounds.end()) {
			Sounds[soundname].setVolume(value);
		}
		else {
			throw std::runtime_error(soundname + " does not exist");
		}
	}

	void SoundManager::setSoundLoop(std::string soundname, bool loop) {
		if (Sounds.find(soundname) != Sounds.end()) {
			Sounds[soundname].setLoop(loop);
		}
		else {
			throw std::runtime_error(soundname + " does not exist");
		}
	}

	void SoundManager::setSoundPitch(std::string soundname, float value) {
		if (Sounds.find(soundname) != Sounds.end()) {
			Sounds[soundname].setPitch(value);
		}
		else {
			throw std::runtime_error(soundname + " does not exist");
		}
	}

	void SoundManager::setBGMOffset(std::string soundname, float time)
	{
		if (BGM.find(soundname) != BGM.end()) {
			BGM[soundname].setPlayingOffset(sf::seconds(time));
		}
		else {
			throw std::runtime_error(soundname + " does not exist");
		}
	}


	void SoundManager::setSoundListener(glm::vec2 pos) {
		sf::Listener().setPosition(pos.x / 100, pos.y / 100, 0);
	}

	void SoundManager::Update() {
		if (stop == false) {
			if (nowBGM != nullptr)
			{
				PM::World* world = Engine::GetGSComponent<PM::World>();

				if (world != nullptr)
				{
					Master_AI* MAI = Engine::GetGSComponent<Master_AI>();

					if (world->GetIsClear() == true)
					{
						nowBGM->setVolume(100 * bgmvolume);
						if (nowBGM->getStatus() != sf::SoundSource::Status::Playing)
						{
							nowBGM->play();
						}
						if (BGM["heart"].getStatus() != sf::SoundSource::Status::Stopped)
						{
							BGM["heart"].stop();
						}
						if (BGM["Detected"].getStatus() != sf::SoundSource::Status::Stopped)
						{
							BGM["Detected"].stop();
						}
					}
					else if (MAI != nullptr && MAI->getIsRedEnemyDetact() == true)
					{
						if (BGM["heart"].getStatus() != sf::SoundSource::Status::Stopped)
						{
							BGM["heart"].stop();
						}
						if (nowBGM->getStatus() != sf::SoundSource::Status::Stopped)
						{
							nowBGM->stop();
						}
						if (BGM["Detected"].getStatus() != sf::SoundSource::Status::Playing) {
							BGM["Detected"].play();
						}
						BGM["Detected"].setVolume(50 * bgmvolume);
					}
					else
					{
						if (BGM["Detected"].getStatus() == sf::SoundSource::Status::Playing)
						{
							nowBGM->play();
							BGM["Detected"].stop();
						}
						float closePos = FLT_MAX;
						for (auto const& i : world->GetNearEnemy(3, 3)) //find near NPC
						{
							if (closePos >= abs(i->GetCenter().x - world->GetPlayerPtr()->GetCenter().x) &&
								i->GetPosition().y == world->GetPlayerPtr()->GetPosition().y)
							{
								closePos = abs(i->GetCenter().x - world->GetPlayerPtr()->GetCenter().x); //set near enemy length
							}
						}
						if (closePos >= BGMdistance.y) {
							BGM["heart"].stop();
							nowBGM->setVolume(100 * bgmvolume);
							if (nowBGM->getStatus() != sf::SoundSource::Status::Playing) {
								nowBGM->play();
							}
						}
						else if (closePos <= BGMdistance.y && closePos >= BGMdistance.x) {

							float volume = 100.f * (closePos - BGMdistance.x) / (BGMdistance.y - BGMdistance.x);
							nowBGM->setVolume(volume * bgmvolume);
							float hvolume;
							if (volume >= 70) {
								hvolume = 0;
							}
							else {
								hvolume = 70.f - volume;
							}
							BGM["heart"].setVolume(hvolume * bgmvolume);
							if (nowBGM->getStatus() != sf::SoundSource::Status::Playing) {
								nowBGM->play();
							}
							if (BGM["heart"].getStatus() != sf::SoundSource::Status::Playing) {
								BGM["heart"].play();
							}
						}
						else if (closePos <= BGMdistance.x) {
							if (nowBGM->getStatus() == sf::SoundSource::Status::Playing)
							{
								nowBGM->stop();
							}
							BGM["heart"].setVolume(70 * bgmvolume);
							if (BGM["heart"].getStatus() != sf::SoundSource::Status::Playing) {
								BGM["heart"].play();
							}
						}
					}
				}
				else
				{
					if (BGM["Detected"].getStatus() == sf::SoundSource::Status::Playing)
					{
						BGM["Detected"].stop();
					}
				}
			}
		}
	}

	void SoundManager::Reset() {
		for (auto& s : BGM) {
			s.second.stop();
		}
	}
}
