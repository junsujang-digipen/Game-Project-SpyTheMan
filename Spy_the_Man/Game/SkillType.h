/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SkillType.h
Project: CS200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#pragma once

enum class SkillType
{
	Drone,
	DroneSound,
	Chameleon,
	Invisible,
	Visualize,
	SilentRun,
	Assassinate,
	SlowTrap,
	Detect,
	Push,
	Dash,
	Flashbomb,
	Null,
};

enum class HowUse {
	Active,
	Passive,
	Null,
};

enum class WhichType {
	Ninja,
	Talker,
	Hacker,
	Null,
};