/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SkillManager.cpp
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#include "SkillManager.h"
#include "../Engine/Engine.h"

PM::SkillManager::SkillData::SkillData(string name, HowUse how, WhichType which, int pointneed, int use, float cooldown, string imagepath) :
	name(name),
	howuseE(how),
	whichtypeE(which),
	pointneed(pointneed),
	cooldown(cooldown),
imagePath(imagepath),
use(use)
{
	if (howuseE == HowUse::Active) {
		howuse = "Active";
	}
	else if (howuseE == HowUse::Passive) {
		howuse = "Passive";
	}
	else if (howuseE == HowUse::Null) {
		howuse = "";
	}
	if (whichtypeE == WhichType::Ninja) {
		whichtype = "Ninja";
	}
	else if (whichtypeE == WhichType::Talker) {
		whichtype = "Talker";
	}
	else if (whichtypeE == WhichType::Hacker) {
		whichtype = "Hacker";
	}
	else if (whichtypeE == WhichType::Null) {
		whichtype = "";
	}
};

PM::SkillManager::~SkillManager()
{
	for (pair<const SkillType, SkillData*> &m : data)
	{
		delete m.second;
		m.second = nullptr;
	}
}

void PM::SkillManager::Add(SkillType skilltype, SkillData* skill)
{
	data[skilltype] = skill;
}

bool PM::SkillManager::GetValid(SkillType skilltype)
{
	for (auto const& iter : data)
	{
		if (iter.first == skilltype)
		{
			return iter.second->valid;
		}
	}
	return false;
}


void PM::SkillManager::UnLockSkill(SkillType skilltype)
{
	for(auto const& iter : data)
	{
		if(iter.first == skilltype)
		{
			if (iter.second->valid == false) {
				if (iter.second->pointneed > usedpoint) {
					notyet = true;
				}
				if (skillpoint <= 0) {
					nopoint = true;
				}
				else if (iter.second->pointneed <= usedpoint && skillpoint > 0) {
					iter.second->valid = true;
					skillpoint--;
					usedpoint++;
					currskills.push_back(iter.first);
				}
			}
		}
		
	}
}

void PM::SkillManager::LoadGS()
{
	Add(SkillType::Null, new SkillData("", HowUse::Null, WhichType::Null, 0));
	Add(SkillType::Drone, new SkillData("Drone", HowUse::Active, WhichType::Hacker, 0, 3, 3, "assets/UI/Skill/Drone.png"));
	Add(SkillType::DroneSound, new SkillData("DroneSound", HowUse::Passive, WhichType::Hacker, 0, 3, 3));
	Add(SkillType::Chameleon, new SkillData("Chameleon", HowUse::Passive, WhichType::Ninja, 0, 1));
	Add(SkillType::Invisible, new SkillData("Invisible", HowUse::Active, WhichType::Ninja, 1, 3, 4, "assets/UI/Skill/disappear.png"));
	Add(SkillType::Visualize, new SkillData("Visualize", HowUse::Active, WhichType::Hacker, 2, 3, 3, "assets/UI/Skill/witness.png"));
	Add(SkillType::SilentRun, new SkillData("SilentRun", HowUse::Passive, WhichType::Ninja, 1, 1));
	Add(SkillType::Assassinate, new SkillData("Assasinate", HowUse::Active, WhichType::Ninja, 0, 3, 10, "assets/UI/Skill/knife.png"));
	Add(SkillType::SlowTrap, new SkillData("SlowTrap", HowUse::Active, WhichType::Ninja, 0, 3, 10, "assets/UI/Skill/trap.png"));
	Add(SkillType::Detect, new SkillData("Detect", HowUse::Active, WhichType::Ninja, 0, 3, 5, "assets/UI/Skill/radar-point.png"));
	Add(SkillType::Push, new SkillData("Push", HowUse::Active, WhichType::Ninja, 0, 3, 5, "assets/UI/Skill/wind.png"));
	Add(SkillType::Dash, new SkillData("Dash", HowUse::Active, WhichType::Ninja, 0, 3, 5, "assets/UI/Skill/shoes.png"));
	Add(SkillType::Flashbomb, new SkillData("Flashbomb", HowUse::Active, WhichType::Ninja, 0, 3, 5, "assets/UI/Skill/flash.png"));
}

PM::SkillManager::SkillData* PM::SkillManager::GetSkillData(SkillType skilltype) { 
	return data[skilltype]; 
}