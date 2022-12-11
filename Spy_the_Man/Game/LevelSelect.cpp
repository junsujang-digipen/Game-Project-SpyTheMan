/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MainmenuTemp.cpp
Project: Spy the man
Author: Sangbeom Kim
Creation date: 12/8/2021
-----------------------------------------------------------------*/
#include "../Engine/Engine.h"	//GetGameStateManager	GetTextureManager
#include "Screens.h"
#include "LevelSelect.h"

#include <filesystem>

#include "../Engine/UI/UIManager.h"
#include "../Engine/Effect/Effect.h"
#include "../Engine/UI/Mouse.h"
#include "../Engine/Helper_func_map.h"
#include "../Engine/UI/Text.h"
#include "../Engine/UI/Image.h"
#include "../Engine/Declaration.h"
#include "SkillString.h"

LevelSelect::LevelSelect() {}

void LevelSelect::Load() {

	BackgroundBackPtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundBack.png");
	BackgroundMiddlePtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundMiddle.png");
	BackgroundFrontPtr = Engine::GetTextureManager().Load("assets/Background/cityBackgroundFront.png");
	AddGSComponent(new PM::UI::UIManager());
	Engine::GetFont().Load("fonts/ABeeZee-Regular.ttf", 50);

	cam.matrix = PM::getCameraMat();
	glm::ivec2 winSize = Engine::GetWindow().GetSize();
	
	Engine::GetSoundManager().PlayBGM("SelectLevel");
	Engine::GetSoundManager().setBGMOffset("SelectLevel", 0.f);
	openingAni = true;
	ChangeAni = false;
	EndAni = false;
	OpeningRectPos = { 0,0 };
	EndRectPos = { 1600,0 };

	for(int i = 0; i < ChapterNum; i++)
	{
		PM::ChapterData* data = Engine::GetLevelIOManager().ReadChaterData(i);
		if(data == nullptr)
		{
			Engine::GetLogger().LogError("What the xxxx It doesn't have a Chapter data");
			Engine::GetGameStateManager().Shutdown();
			return;
		}
		std::string path = { "assets/Level/Chapter" + to_string(i) + "/Chapter" + to_string(i) + ".png" };
		if (filesystem::exists(path) == false)
		{
			path = "assets/Level/NoImage.png";
		}
		AllChapterData.push_back(data);
		PM::UI::ImageButton* ib = new PM::UI::ImageButton("", path, { 100 + (i + 1) * 500, 400 }, { 400,225 });
		if(i > 0 && AllChapterData[i-1]->isCleard == false)
		{
			ib->SetLock(true);
		}

		ChapterButtons.push_back(ib);
	}
	Back = new PM::UI::ImageButton("", "assets/UI/Button/Left.png", { 1400,700 }, { 100,100 }, UILayer::DEFAULT, false, false);
	SelectChapterNum = 0;
	SelectStage = 0;
	state = SelectState::Chapter;
	TitleText = new PM::UI::Text("", "Title", {800, 700}, 100, PM::AlignType::center, UILayer::PHASE, { 255,255,255 });
	ClearTime = new PM::UI::Text("", "ClearTime", { 800, 630 }, 60, PM::AlignType::center, UILayer::PHASE, { 255,255,255 });
	ClearTime->Setvalid(false);
	SkillButtons.clear();
	SkillButtons.push_back(new PM::UI::Button("skill1", { 250, 180 }, { 70, 70 }, {255, 255, 255, 200}, UILayer::DEFAULT, false, false));
	SkillButtons.push_back(new PM::UI::Button("skill2", { 320, 180 }, { 70, 70 }, { 255, 255, 255, 200 }, UILayer::DEFAULT, false, false));
	SkillButtons.push_back(new PM::UI::Button("skill3", { 390, 180 }, { 70, 70 }, { 255, 255, 255, 200 }, UILayer::DEFAULT, false, false));
	SkillButtons.push_back(new PM::UI::Button("skill4", { 460, 180 }, { 70, 70 }, { 255, 255, 255, 200 }, UILayer::DEFAULT, false, false));
	SkillButtons.push_back(new PM::UI::Button("skill5", { 530, 180 }, { 70, 70 }, { 255, 255, 255, 200 }, UILayer::DEFAULT, false, false));
	PM::Image("assets/UI/Skill/Drone.png");
	PM::Image("assets/UI/Skill/Drone-sound.png");
	PM::Image("assets/UI/Skill/Chameleon.png");
	PM::Image("assets/UI/Skill/disappear.png");
	PM::Image("assets/UI/Skill/witness.png");
	PM::Image("assets/UI/Skill/silence.png");
	PM::Image("assets/UI/Skill/knife.png");
	PM::Image("assets/UI/Skill/trap.png");
	PM::Image("assets/UI/Skill/radar-point.png");
	PM::Image("assets/UI/Skill/wind.png");
	PM::Image("assets/UI/Skill/shoes.png");
	PM::Image("assets/UI/Skill/flash.png");

	settings = new PM::UI::ImageButton("", "assets/UI/Button/settings.png", { 100,700 }, { 100,100 });
	sfxadd = new PM::UI::Button("sfxadd", { winSize.x / 2 + 300, 545 }, { 50, 50 }, { 255, 255, 255, 255 });
	sfxmin = new PM::UI::Button("sfxmin", { winSize.x / 2 - 200, 545 }, { 50, 50 }, { 255, 255, 255, 255 });
	bgmadd = new PM::UI::Button("sfxadd", { winSize.x / 2 + 300, 445 }, { 50, 50 }, { 255, 255, 255, 255 });
	bgmmin = new PM::UI::Button("sfxmin", { winSize.x / 2 - 200, 445 }, { 50, 50 }, { 255, 255, 255, 255 });
	settingsexit = new PM::UI::TextButton("Settingsexit", { winSize.x / 2 - 150, 200 }, { 300, 100 }, "Exit", { 255, 255, 255, 255 }, { 0, 0, 0 });
}

void LevelSelect::Update(double dt) {
	UpdateGSComponents(dt);
#if _DEBUG
	if(Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::E) && Engine::GetInput().IsKeyDown(PM::InputKey::Keyboard::L))
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::LevelEditor));
	}
#endif
	if (Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::Escape))
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenuTemp));
	}

	if (openingAni == true)
	{
		if (OpeningRectPos.x < -1700)
		{
			openingAni = false;
		}
		else
		{
			
			OpeningRectPos.x -= Engine::GetWindow().GetSize().x * 2.3 * dt;
		}
	}
	else if(ChangeAni)
	{
		ChangeAnimationUpdate(dt);
	}
	else if(EndAni)
	{
		if (EndRectPos.x < 0)
		{
			EndRectPos.x = 0;
			Engine::GetGameStateManager().SetLevelState
			(SelectChapter->levels[SelectStage]->levelName, "assets/Level/Chapter" + to_string(SelectChapter->chapterNumber) + "/");
		}
		else
		{
			EndRectPos.x -= Engine::GetWindow().GetSize().x * 2.3 * dt;
		}
	}
	else
	{
		switch (state) {
		case SelectState::Chapter:
			ChapterUpdate(dt);
			break;
		case SelectState::Level:
			LevelUpdate(dt);
			break;
		}
	}
	zoom = helper::Lerp(zoom, (state == SelectState::Chapter) ? 2. : 1, dt);
	backgroundPos = { LevelPos.x + ChapterPos.x,0 };

	for (int i = 0; i < SkillButtons.size(); ++i) {
		if (SkillButtons[i]->IsPressed() == true) {
			currskill = GetLevelSkills()[i];
			Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
		}
		if (i < GetLevelSkills().size()) {
			SkillButtons[i]->Setvalid(true);
		}
		else {
			SkillButtons[i]->Setvalid(false);
		}
	}
	if (settings->IsReleased() == true) {
		setting = !setting;
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	if (sfxadd->IsReleased() == true && sfxvolume < 10) {
		sfxvolume++;
		Engine::GetSoundManager().setGlobalVolume(sfxvolume);
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	if (sfxmin->IsReleased() == true && sfxvolume > 0) {
		sfxvolume--;
		Engine::GetSoundManager().setGlobalVolume(sfxvolume);
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	if (bgmadd->IsReleased() == true && bgmvolume < 10) {
		bgmvolume++;
		Engine::GetSoundManager().setBGMVolume(bgmvolume);
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
		Engine::GetSoundManager().PlayBGM("SelectLevel");
	}
	if (bgmmin->IsReleased() == true && bgmvolume > 0) {
		bgmvolume--;
		Engine::GetSoundManager().setBGMVolume(bgmvolume);
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
		Engine::GetSoundManager().PlayBGM("SelectLevel");
	}
	if (settingsexit->IsReleased() == true) {
		setting = false;
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	if (setting == true) {
		for (auto &b : ChapterButtons) {
			b->Setvalid(false);
		}
		for (auto& b : LevelButtons) {
			b->Setvalid(false);
		}
		for (auto& b : ChangeLevelButtons) {
			b->Setvalid(false);
		}
		for (auto& b : SkillButtons) {
			b->Setvalid(false);
		}
		Back->Setvalid(false);
		sfxadd->Setvalid(true);
		sfxmin->Setvalid(true);
		bgmadd->Setvalid(true);
		bgmmin->Setvalid(true);
		settingsexit->Setvalid(true);
		TitleText->Setvalid(false);
	}
	else {
		if (state == SelectState::Chapter) {
			for (auto& b : ChapterButtons) {
				b->Setvalid(true);
			}
		}
		if (state == SelectState::Level) {
			for (auto& b : LevelButtons) {
				b->Setvalid(true);
			}
			for (auto& b : ChangeLevelButtons) {
				b->Setvalid(true);
			}
			for (int i = 0; i < SkillButtons.size(); ++i) {
				if (i < GetLevelSkills().size()) {
					SkillButtons[i]->Setvalid(true);
				}
				else {
					SkillButtons[i]->Setvalid(false);
				}
			}
			Back->Setvalid(true);
		}
		sfxadd->Setvalid(false);
		sfxmin->Setvalid(false);
		bgmadd->Setvalid(false);
		bgmmin->Setvalid(false);
		settingsexit->Setvalid(false);
		TitleText->Setvalid(true);
	}
}

void LevelSelect::Draw() {
	Engine::GetWindow().SetBackground(88.f / 255.f, 90.f / 255.f, 191.f/255.f, 1.f);
	Engine::GetWindow().Clear();

	glm::ivec2 winSize = Engine::GetWindow().GetSize();
	//math::TransformMatrix cameramat;
	//cameramat.matrix = PM::getCameraMat();

	glm::mat3 NDC_{ {zoom / Engine::GetWindow().GetSize().x, 0, 0}, {0, zoom / Engine::GetWindow().GetSize().y, 0}, {-1.f, -1.f, 1.f} };
	cam.matrix = NDC_;

	glm::mat3 transform1 = { {1, 0, 0},{0, 1, 0},{ backgroundPos.x * 3 / 4,900 * backgroundPos.y * 10 / 100, 1} };
	glm::mat3 transform2 = { {1, 0, 0},{0, 1, 0},{ backgroundPos.x * 2 / 4, 900 * backgroundPos.y * 5 / 100, 1} };
	glm::mat3 transform3 = { {1, 0, 0},{0, 1, 0},{	 backgroundPos.x * 1 / 4, 0, 1} };

	BackgroundBackPtr->Draw(cam * transform3, { -5000, -50 }, { 20000 , 2000 });
	BackgroundMiddlePtr->Draw(cam * transform2, { -5000, -50 }, { 20000 , 2000 });
	BackgroundFrontPtr->Draw(cam * transform1, { -5000, -50 }, { 20000 , 2000 });

	if (setting == true) {
		PM::push_settings();
		PM::apply_NDC();
		//PM::use_shader("Title");
		PM::set_fill_color(0, 0, 0, 150);
		PM::draw_rect(0, 0, winSize.x, winSize.y);
		PM::pop_settings();

		PM::push_settings();
		PM::apply_NDC();
		PM::set_fill_color(255, 255, 255, 230);
		PM::draw_rect(winSize.x / 2 - 500, winSize.y / 2 - 300, 1000, 600);
		PM::set_fill_color(255, 255, 255);
		for (int i = 0; i < 10; ++i) {
			PM::draw_rect(winSize.x / 2 - 150 + i * 45, 545, 45, 50);
			PM::draw_rect(winSize.x / 2 - 150 + i * 45, 445, 45, 50);
		}
		PM::set_fill_color(100, 100, 100);
		for (int j = 0; j < sfxvolume; ++j) {
			PM::draw_rect(winSize.x / 2 - 150 + j * 45, 545, 45, 50);
		}
		for (int k = 0; k < bgmvolume; ++k) {
			PM::draw_rect(winSize.x / 2 - 150 + k * 45, 445, 45, 50);
		}
		PM::pop_settings();
	}

	Engine::GetGSComponent<PM::UI::UIManager>()->Draw();

	if (setting == true) {
		Engine::GetFont().Draw("Volume", (float)winSize.x / 2, 650, 2, false, PM::AlignType::center, { 0.f,0.f,0.f });
		Engine::GetFont().Draw("SFX", (float)winSize.x / 2 - 350, 550, 1.5, false, PM::AlignType::center, { 0.f,0.f,0.f });
		Engine::GetFont().Draw("BGM", (float)winSize.x / 2 - 350, 450, 1.5, false, PM::AlignType::center, { 0.f,0.f,0.f });
		Engine::GetFont().Draw("+", (float)winSize.x / 2 + 325, 550, 1.5, false, PM::AlignType::center, { 0.f,0.f,0.f });
		Engine::GetFont().Draw("-", (float)winSize.x / 2 - 175, 550, 1.5, false, PM::AlignType::center, { 0.f,0.f,0.f });
		Engine::GetFont().Draw("+", (float)winSize.x / 2 + 325, 450, 1.5, false, PM::AlignType::center, { 0.f,0.f,0.f });
		Engine::GetFont().Draw("-", (float)winSize.x / 2 - 175, 450, 1.5, false, PM::AlignType::center, { 0.f,0.f,0.f });
	}

	if (openingAni == true)
	{
		PM::push_settings();
		PM::apply_NDC();
		PM::set_fill_color(0, 0, 0, 255);
		PM::no_outline();
		PM::draw_rect(OpeningRectPos.x, OpeningRectPos.y, winSize.x + 100, winSize.y);
		PM::pop_settings();
	}
	else if (ChangeAni == true)
	{

	}
	else if (EndAni == true)
	{
		PM::push_settings();
		PM::apply_NDC();
		PM::set_fill_color(0, 0, 0, 255);
		PM::no_outline();
		PM::draw_rect(EndRectPos.x, EndRectPos.y, winSize.x + 300, winSize.y);
		PM::pop_settings();
	}
	else {
		if (setting == false) {
			if (state == SelectState::Level) {
				glm::mat3 digipen = { {2.f / winSize.x, 0, 0}, {0, 2.f / winSize.y, 0}, {-1.f, -1.f, 1.f} };
				for (int i = 0; i < GetLevelSkills().size(); ++i) {
					switch (GetLevelSkills()[i]) {
					case SkillType::Drone:
						PM::draw_image("assets/UI/Skill/Drone.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::DroneSound:
						PM::draw_image("assets/UI/Skill/Drone-sound.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::Chameleon:
						PM::draw_image("assets/UI/Skill/Chameleon.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::Invisible:
						PM::draw_image("assets/UI/Skill/disappear.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::Visualize:
						PM::draw_image("assets/UI/Skill/witness.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::SilentRun:
						PM::draw_image("assets/UI/Skill/silence.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::Assassinate:
						PM::draw_image("assets/UI/Skill/knife.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::SlowTrap:
						PM::draw_image("assets/UI/Skill/trap.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::Detect:
						PM::draw_image("assets/UI/Skill/radar-point.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::Push:
						PM::draw_image("assets/UI/Skill/wind.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::Dash:
						PM::draw_image("assets/UI/Skill/shoes.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					case SkillType::Flashbomb:
						PM::draw_image("assets/UI/Skill/flash.png", digipen, { 250 + 70 * i, 180 }, { 68, 68 });
						break;
					default:
						break;
					}
				}
				PM::SkillManager::SkillData* skilldata = Engine::GetSkillManager().GetSkillData(currskill);
				glm::vec3 f_color = { 0,0,0 };
				PM::push_settings();
				PM::apply_NDC();
				PM::set_line_color(255, 255, 255, 255);
				PM::set_line_width(3);
				PM::set_fill_color(255, 255, 255, 200);
				PM::draw_rect(250, 30, 1100, 150);
				Engine::GetFont().Draw(PM::SkillString(currskill).SS1, 480, 138, 0.8f, false, PM::AlignType::left, f_color);
				Engine::GetFont().Draw(PM::SkillString(currskill).SS2, 480, 93, 0.8f, false, PM::AlignType::left, f_color);
				Engine::GetFont().Draw(PM::SkillString(currskill).SS3, 480, 48, 0.8f, false, PM::AlignType::left, f_color);
				PM::set_fill_color(255, 255, 255, 150);
				PM::draw_rect(250, 30, 210, 50);
				if (currskill != SkillType::Null) {
					Engine::GetFont().Draw(std::to_string(static_cast<int>(skilldata->cooldown)) + " Sec Cool", 355, 43, 0.8f, false, PM::AlignType::center, f_color);
				}
				PM::draw_rect(250, 80, 210, 50);
				Engine::GetFont().Draw(skilldata->howuse, 355, 93, 0.8f, false, PM::AlignType::center, f_color);
				PM::draw_rect(250, 130, 210, 50);
				Engine::GetFont().Draw(skilldata->name, 355, 143, 0.8f, false, PM::AlignType::center, f_color);
				PM::pop_settings();
			}
			else {
				for (auto& sb : SkillButtons) {
					sb->Setvalid(false);
				}
			}
		}
	}
}

void LevelSelect::ChapterUpdate(double dt)
{
	if (Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::M) == true)
	{
		for(auto b : ChapterButtons)
		{
			b->SetLock(false);
		}
	}
	int bsize = static_cast<int>(ChapterButtons.size());
	
	ChapterPos.x = helper::Lerp(ChapterPos.x, static_cast<double>(SelectChapterNum) * 500., dt * 10);
	for (int i = 0; i < bsize; ++i)
	{
		ChapterButtons[i]->SetPosition({ 100 + (i + 1) * 500 - ChapterPos.x, 400 + ChapterPos.y });
	}
	;
	if (ChapterButtons[SelectChapterNum]->IsReleased() == true)
	{
		SelectChapter = AllChapterData[SelectChapterNum];
		ChangeAni = true;
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	else if (SelectChapterNum > 0 && ChapterButtons[SelectChapterNum-1]->IsReleased() == true)
	{
		SelectChapterNum--;
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	else if (SelectChapterNum < bsize - 1 && ChapterButtons[SelectChapterNum+1]->IsReleased() == true)
	{
		SelectChapterNum++;
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	TitleText->SetText("CHAPTER " + to_string(AllChapterData[SelectChapterNum]->chapterNumber));
}

void LevelSelect::ChangeAnimationUpdate(double )
{
	if (state == SelectState::Level)
	{
		ChangeState(SelectState::Chapter);
		ChangeAni = false;
	}
	else
	{
		ChangeState(SelectState::Level);
		ChangeAni = false;
	}
	
}

void LevelSelect::LevelUpdate(double dt)
{
	if(Engine::GetInput().IsKeyReleased(PM::InputKey::Keyboard::M) == true)
	{
		for(auto b : LevelButtons)
		{
			b->SetLock(false);
		}
		for(auto b : ChangeLevelButtons)
		{
			b->SetLock(false);
		}
	}
	int bsize = static_cast<int>(LevelButtons.size());
	
	LevelPos.x = helper::Lerp(LevelPos.x, static_cast<double>(SelectStage) * 500., static_cast<float>(dt * 10.));
	for(int i = 0; i < bsize;++i)
	{
		if (ChangeLevelButtons[i]->IsReleased() == true)
		{
			SelectStage = i;
			currskill = SkillType::Null;
			Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
		}
		LevelButtons[i]->SetPosition({ 100 + (i + 1) * 500 - LevelPos.x, 400 + LevelPos.y });
		ChangeLevelButtons[i]->SetPosition({ 300 + i * 100,290 + LevelPos.y });
	}
	
	if (LevelButtons[SelectStage]->IsReleased() == true)
	{
		if(SelectChapter->levels[SelectStage]->levelName == "Tutorial")
		{
			Engine::GetLevelIOManager().SetChapter(SelectChapter->chapterNumber, SelectStage);
			Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::Tuto));
		}
		else if (Engine::GetLevelIOManager().HaveLevel
		(SelectChapter->levels[SelectStage]->levelName, "assets/Level/Chapter" + to_string(SelectChapter->chapterNumber) + "/"))
		{
			Engine::GetLevelIOManager().SetChapter(SelectChapter->chapterNumber, SelectStage);
			EndAni = true;
		}
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	else if(SelectStage > 0 && LevelButtons[SelectStage-1]->IsReleased() == true)
	{
		SelectStage--;
		currskill = SkillType::Null;
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	else if (SelectStage < bsize-1 && LevelButtons[SelectStage + 1]->IsReleased() == true)
	{
		SelectStage++;
		currskill = SkillType::Null;
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	if(Back->IsPressed() == true)
	{
		ChangeAni = true;
		Engine::GetSoundManager().PlaySound("UIButton", { 0,0 });
	}
	TitleText->SetText(to_string(SelectStage + 1) + ". " + SelectChapter->levels[SelectStage]->levelName);
	if (SelectChapter->levels[SelectStage]->IsCleard == true)
	{
		double PlayTime = SelectChapter->levels[SelectStage]->ClearTime;
		int minute = static_cast<int>(PlayTime) / 60;
		int second = static_cast<int>(PlayTime) % 60;
		int ms = static_cast<int>((PlayTime - static_cast<int>(PlayTime)) * 100);
		ClearTime->SetText(to_string(minute) + " : " + to_string(second) + " : " + to_string(ms));
	}
	else
	{
		ClearTime->SetText("");
	}
}


void LevelSelect::ChangeState(SelectState stat)
{
	if (stat == state) { return; }
	switch (state)
	{
	case SelectState::Chapter:
		{
		ChapterStateUnload();
		}
		break;
	case SelectState::Level:
		LevelStateUnload();
		break;
	}
	state = stat;
	switch (state)
	{
	case SelectState::Chapter:
	{
		ChapterStateLoad();
	}
	break;
	case SelectState::Level:
	{
		LevelStateLoad();
	}
	break;
	}
}


void LevelSelect::Unload() {
	Engine::GetSoundManager().Pause();
	for(auto a : AllChapterData)
	{
		delete a;
	}
	AllChapterData.clear();

	SelectChapter = nullptr;
	ChapterButtons.clear();
	LevelButtons.clear();
	ChangeLevelButtons.clear();
	Back = nullptr;
	ClearGSComponent();
}

void LevelSelect::ChapterStateLoad()
{
	for (auto b : ChapterButtons)
	{
		b->Setvalid(true);
	}
	//SelectChapterNum = 0;
}

void LevelSelect::ChapterStateUnload()
{
	for (auto b : ChapterButtons)
	{
		b->Setvalid(false);
	}
}

void LevelSelect::LevelStateLoad()
{
	SelectStage = 0;
	auto stages = SelectChapter->levels;
	int stageSize = static_cast<int>(SelectChapter->levels.size());
	for (int i = 0; i < stageSize; ++i)
	{
		std::string path = { "assets/Level/Chapter" + to_string(SelectChapter->chapterNumber) + "/" + stages[i]->levelName + ".png" };
		if (filesystem::exists(path) == false)
		{
			path = "assets/Level/NoImage.png";
		}
		PM::UI::ImageButton* ib = new PM::UI::ImageButton("", path, { 100 + (i + 1) * 500,400 }, { 400,225 });
		PM::UI::TextButton* clb = new PM::UI::TextButton("", { 300 + i * 100,300 }, { 80,80 }, to_string(i + 1));
		if(i > 0 && SelectChapter->levels[i-1]->IsCleard == false)
		{
			ib->SetLock(true);
			clb->SetLock(true);
		}
		LevelButtons.push_back(ib);
		ChangeLevelButtons.push_back(clb);
	}
	//SelectStage = 0;
	Back->Setvalid(true);
	ClearTime->Setvalid(true);
}

void LevelSelect::LevelStateUnload()
{
	for (auto b : LevelButtons)
	{
		b->Setvalid(false);
		b->Destroy();
	}
	for (auto b : ChangeLevelButtons)
	{
		b->Setvalid(false);
		b->Destroy();
	}
	Back->Setvalid(false);
	LevelButtons.clear();
	ChangeLevelButtons.clear();
	ClearTime->Setvalid(false);
}

std::vector<SkillType> LevelSelect::GetLevelSkills()
{

	if (state == SelectState::Level)
	{
		if (latestlevelName != SelectChapter->levels[SelectStage]->levelName)
		{
			skills.clear();
			latestlevelName = SelectChapter->levels[SelectStage]->levelName;
			if (Engine::GetLevelIOManager().HaveLevel
			(SelectChapter->levels[SelectStage]->levelName, "assets/Level/Chapter" + to_string(SelectChapter->chapterNumber) + "/"))
			{
				PM::LevelData* temp = Engine::GetLevelIOManager().ReadLevel(SelectChapter->levels[SelectStage]->levelName, "assets/Level/Chapter" + to_string(SelectChapter->chapterNumber) + "/");

				for(int i = 0; i < static_cast<int>(SkillType::Null);++i)
				{
					if(temp->GetSkillset(static_cast<SkillType>(i)) == true && i != static_cast<int>(SkillType::Assassinate))
					{
						skills.push_back(static_cast<SkillType>(i));
					}
				}
			
				
				delete temp;
			}
		}
	}
	else
	{
		skills.clear();
		latestlevelName.clear();
	}

	return skills;
}
