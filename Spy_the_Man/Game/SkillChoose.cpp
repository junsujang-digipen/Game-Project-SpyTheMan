/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: SkillChoose.cpp
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"	//GetGameStateManager GetSpriteFont
#include "../Engine/Poodle.h"
#include "../Engine/Font.h"
#include "../Engine/UI/UIManager.h"
#include "SkillChoose.h"
#include "Screens.h"
#include "SkillString.h"

SkillChoose::SkillChoose() : loading(false), finish(nullptr), mainmenu(nullptr), decide(nullptr), drone(nullptr), dronesound(nullptr),
	chameleon(nullptr),	invisible(nullptr), visualize(nullptr), silentrun(nullptr), assassinate(nullptr), slowtrap(nullptr), 
	detect(nullptr), push(nullptr), dash(nullptr), flashbomb(nullptr), currskill(SkillType::Null)
{}

void SkillChoose::Load() {
	Engine::GetFont().Load("fonts/ABeeZee-Regular.ttf", 50);
	AddGSComponent(new PM::UI::UIManager());
	currskill = SkillType::Null;
	mainmenu = new PM::UI::TextButton("mainmenu", { 30, 790 }, { 170, 80 }, "Title",{ 255, 255, 255, 100 },{0,0,0});
	finish = new PM::UI::TextButton("finish", { 1400, 790 }, { 170, 80 }, "Finish", { 255, 255, 255, 100 },{0,0,0});
	decide = new PM::UI::TextButton("decide", { 1205, 40 }, { 195, 105 }, "Decide", { 255, 255, 255, 100 },{0,0,0});
	drone = new PM::UI::ImageButton("drone", "assets/UI/Skill/Drone.png", { 1040, 640 }, { 80, 80 });
	dronesound = new PM::UI::ImageButton("dronesound", "assets/UI/Skill/Drone-sound.png", { 1040, 540 }, { 80, 80 });
	chameleon = new PM::UI::ImageButton("chameleon", "assets/UI/Skill/Chameleon.png", { 340, 640 }, { 80, 80 });
	invisible = new PM::UI::ImageButton("invisible", "assets/UI/Skill/disappear.png", { 480, 640 }, { 80, 80 });
	visualize = new PM::UI::ImageButton("visualize", "assets/UI/Skill/witness.png", { 1180, 640 }, { 80, 80 });
	silentrun = new PM::UI::ImageButton("silentrun", "assets/UI/Skill/silence.png", { 340, 540 }, { 80, 80 });
	assassinate = new PM::UI::ImageButton("assassinate", "assets/UI/Skill/knife.png", { 480, 540 }, { 80, 80 });
	slowtrap = new PM::UI::ImageButton("assassinate", "assets/UI/Skill/trap.png", { 340, 440 }, { 80, 80 });
	detect = new PM::UI::ImageButton("assassinate", "assets/UI/Skill/radar-point.png", { 480, 440 }, { 80, 80 });
	push = new PM::UI::ImageButton("assassinate", "assets/UI/Skill/wind.png", { 340, 340 }, { 80, 80 });
	dash = new PM::UI::ImageButton("assassinate", "assets/UI/Skill/shoes.png", { 480, 340 }, { 80, 80 });
	flashbomb = new PM::UI::ImageButton("assassinate", "assets/UI/Skill/flash.png", { 340, 240 }, { 80, 80 });
}

void SkillChoose::Update(double dt) {
	UpdateGSComponents(dt);
	if (Engine::GetSkillManager().GetNotYet() == true) {
		yettime += dt;
	}
	if (yettime >= 1) {
		Engine::GetSkillManager().SetNotYet(false);
		yettime = 0;
	}
	if (Engine::GetSkillManager().GetNoPoint() == true) {
		noptime += dt;
	}
	if (noptime >= 1) {
		Engine::GetSkillManager().SetNoPoint(false);
		noptime = 0;
	}
	
	if (mainmenu->IsReleased() == true) {
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenuTemp));
	}
	if (finish->IsReleased() == true) {
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenuTemp));
	}
	if (decide->IsReleased() == true && currskill != SkillType::Null) {
		Engine::GetSoundManager().PlaySound("skillDecide", {0,0});
		Engine::GetSkillManager().UnLockSkill(currskill);
	}
	if (drone->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Drone;
	}
	if (dronesound->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::DroneSound;
	}
	if (chameleon->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Chameleon;
	}
	if (invisible->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Invisible;
	}
	if (visualize->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Visualize;
	}
	if (silentrun->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::SilentRun;
	}
	if (assassinate->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Assassinate;
	}
	if (slowtrap->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::SlowTrap;
	}
	if (detect->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Detect;
	}
	if (push->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Push;
	}
	if (dash->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Dash;
	}
	if (flashbomb->IsPressed() == true) {
		Engine::GetSoundManager().PlaySound("skillButton", {0,0});
		currskill = SkillType::Flashbomb;
	}
}

void SkillChoose::Unload() {
	ClearGSComponent();
}

void SkillChoose::Draw() {
	Engine::GetWindow().SetBackground(255.f, 255.f, 255.f, 255.f);
	Engine::GetWindow().Clear();

	math::TransformMatrix cam;
	cam.matrix = PM::getCameraMat();
	PM::push_settings();
	PM::use_shader("Hologram");
	PM::apply_NDC();
	PM::set_fill_color(45,61,131, 100);
	PM::draw_rect(0, 0, Engine::GetWindow().GetSize().x, Engine::GetWindow().GetSize().y);
	PM::pop_settings();
	Engine::GetFont().Draw("Choose skill", (float)Engine::GetWindow().GetSize().x / 2, (float)Engine::GetWindow().GetSize().y - 60, 1.5f, false, PM::AlignType::center, { 0.f,0.f,0.f });

	PM::SkillManager::SkillData* skilldata = Engine::GetSkillManager().GetSkillData(currskill);

	PM::push_settings();
	PM::apply_NDC();
	PM::set_line_width(3);
	PM::set_fill_color(255, 255, 255);

	//PM::draw_rect(300, 295, 300, 480);
	//PM::draw_rect(650, 295, 300, 480);
	//PM::draw_rect(1000, 295, 300, 480);
	glm::vec3 f_color= { 255,255,255 };
	PM::set_fill_color(39, 191, 238, 100);
	PM::draw_rect(350, 745, 200, 60);
	Engine::GetFont().Draw("Ninja", 450, 762, 1, false, PM::AlignType::center, { 255.f,255.f,255.f });
	PM::draw_rect(700, 745, 200, 60);
	Engine::GetFont().Draw("Talker", 800, 762, 1, false, PM::AlignType::center, f_color);
	PM::draw_rect(1050, 745, 200, 60);
	Engine::GetFont().Draw("Hacker", 1150, 762, 1, false, PM::AlignType::center, f_color);
	PM::set_fill_color(39, 191, 238, 100);
	PM::draw_rect(200, 40, 750, 150);
	PM::draw_rect(200, 190, 250, 70);
	Engine::GetFont().Draw(skilldata->name, 325, 210, 1, false, PM::AlignType::center, f_color);
	PM::set_fill_color(39, 191, 238, 100);
	PM::draw_rect(450, 190, 250, 70);
	Engine::GetFont().Draw(skilldata->howuse, 575, 210, 1, false, PM::AlignType::center, f_color);
	PM::set_fill_color(39, 191, 238, 100);
	PM::draw_rect(700, 190, 250, 70);
	Engine::GetFont().Draw(skilldata->whichtype, 825, 210, 1, false, PM::AlignType::center, f_color);
	PM::set_fill_color(39, 191, 238, 100);
	PM::draw_rect(1000, 215, 200, 45);
	Engine::GetFont().Draw("Required Skills", 1103, 230, 0.6f, false, PM::AlignType::center, f_color);
	PM::draw_rect(1000, 105, 200, 45);
	Engine::GetFont().Draw("Opened Skills", 1103, 120, 0.6f, false, PM::AlignType::center, f_color);
	PM::draw_rect(1200, 215, 200, 45);
	Engine::GetFont().Draw("Skill Points", 1303, 230, 0.6f, false, PM::AlignType::center, f_color);
	PM::set_fill_color(39, 191, 238, 100);
	PM::draw_rect(1000, 150, 200, 65);
	Engine::GetFont().Draw(to_string(skilldata->pointneed), 1103, 170, 0.8f, false, PM::AlignType::center, f_color);
	PM::draw_rect(1000, 40, 200, 65);
	Engine::GetFont().Draw(to_string(Engine::GetSkillManager().GetUsedPoint()), 1103, 60, 0.8f, false, PM::AlignType::center, f_color);
	PM::draw_rect(1200, 150, 200, 65);
	Engine::GetFont().Draw(to_string(Engine::GetSkillManager().GetSkillPoint()), 1303, 170, 0.8f, false, PM::AlignType::center, f_color);
	PM::pop_settings();

	Engine::GetFont().Draw(PM::SkillString(currskill).SS1, 215, 150, 0.8f, false, PM::AlignType::left, f_color);
	Engine::GetFont().Draw(PM::SkillString(currskill).SS2, 215, 105, 0.8f, false, PM::AlignType::left, f_color);
	Engine::GetFont().Draw(PM::SkillString(currskill).SS3, 215, 60, 0.8f, false, PM::AlignType::left, f_color);

	Engine::GetGSComponent<PM::UI::UIManager>()->Draw();

	PM::push_settings();
	PM::apply_NDC();
	PM::set_fill_color(0, 0, 0, 200);
	for (SkillType& it : Engine::GetSkillManager().CurrSkills()) {
		if (Engine::GetSkillManager().GetValid(it) == true) {
			switch (it) {
			case SkillType::Drone:
				PM::draw_rect(1040, 640, 80, 80);
				break;
			case SkillType::DroneSound:
				PM::draw_rect(1040, 540, 80, 80);
				break;
			case SkillType::Chameleon:
				PM::draw_rect(340, 640, 80, 80);
				break;
			case SkillType::Invisible:
				PM::draw_rect(480, 640, 80, 80);
				break;
			case SkillType::Visualize:
				PM::draw_rect(1180, 640, 80, 80);
				break;
			case SkillType::SilentRun:
				PM::draw_rect(340, 540, 80, 80);
				break;
			case SkillType::Assassinate:
				PM::draw_rect(480, 540, 80, 80);
				break;
			case SkillType::SlowTrap:
				PM::draw_rect(340, 440, 80, 80);
				break;
			case SkillType::Detect:
				PM::draw_rect(480, 440, 80, 80);
				break;
			case SkillType::Push:
				PM::draw_rect(340, 340, 80, 80);
				break;
			case SkillType::Dash:
				PM::draw_rect(480, 340, 80, 80);
				break;
			case SkillType::Flashbomb:
				PM::draw_rect(340, 240, 80, 80);
				break;
			default:
				break;
			}
		}
	}
	if (Engine::GetSkillManager().GetNotYet() == true) {
		PM::set_fill_color(225, 225, 225);
		PM::set_line_width(3);
		PM::draw_rect(550, 325, 500, 200);
		Engine::GetFont().Draw("Skill not unlocked yet", 785, 415, 1.1f, false, PM::AlignType::center, { 0.f,0.f,0.f });
	}
	if (Engine::GetSkillManager().GetNoPoint() == true) {
		PM::set_fill_color(225, 225, 225);
		PM::set_line_width(3);
		PM::draw_rect(550, 325, 500, 200);
		Engine::GetFont().Draw("Not enough skill points", 790, 415, 1.1f, false, PM::AlignType::center, { 0.f,0.f,0.f });
	}
	PM::pop_settings();
}

