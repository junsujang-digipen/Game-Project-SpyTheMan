#include "ClearManager.h"

#include "ValidManager.h"
#include "../Engine/Engine.h"
#include "../Engine/World.h"
#include "../Game/Screens.h"
#include "../Engine/UI/Button.h"
#include "../Engine/UI/Image.h"
#include "../Engine/UI/Text.h"
#include "../Engine/Helper_func_map.h"


ClearManager::ClearManager() : Component("ClearManager")
{
	Background = new PM::UI::Rect("", { 0,0 }, { 1600,900 }, UILayer::DEFAULT, { 0,0,0,80 });
	Background->Setvalid(false);
	
	LevelSelectButton = new PM::UI::ImageButton("", "assets/UI/Button/LevelSelect.png", { 250,-600 }, { 300,300 }, UILayer::PHASE, false, false);
	ReplayButton = new PM::UI::ImageButton("", "assets/UI/Button/Replay.png", { 650,-900 }, { 300,300 }, UILayer::PHASE, false, false);
	NextLevelButton = new PM::UI::ImageButton("", "assets/UI/Button/NextLevel.png", { 1050,-300 }, { 300,300 }, UILayer::PHASE, false, false);

	ClearTimeText = new PM::UI::Text("","ctt",{Engine::GetWindow().GetSize().x/2,600},200,PM::AlignType::center,UILayer::PHASE,{255,255,255});
	ClearTimeText->Setvalid(false);
	PlayTime = 0;
}
ClearManager::~ClearManager()
{
	
}

void ClearManager::Update(double dt)
{
	dt = Engine::GetDeltaTime();
	if(isClear == true)
	{
		if (isAnimationEnd)
		{
			ClearUpdate(dt);
		}
		else
		{
			ClearAnimationUpdate(dt);
			ClearAnimationTimer += dt;
		}
	}
	else if(Engine::GetGSComponent<PM::World>()->GetIsClear() == true)
	{
		Engine::GetGSComponent<ValidManager>()->Setdt(0);
		Engine::GetSoundManager().Pause();
		Engine::GetSoundManager().PlaySound("checkpointClear", Engine::GetGSComponent<PM::World>()->GetPlayerPtr()->GetPosition());
		Engine::GetSoundManager().Reset();
		Engine::GetSoundManager().PlayBGM("Clear");
		//Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::MainMenuTemp));
		Background->Setvalid(true);
		NextLevelButton->Setvalid(true);
		LevelSelectButton->Setvalid(true);
		ReplayButton->Setvalid(true);
		ClearTimeText->Setvalid(true);
		Engine::GetLevelIOManager().ClearStage(PlayTime);
		isClear = true;
	}
	else
	{
		PlayTime += dt;
	}
	//minute = static_cast<int>(PlayTime) / 60;
	//second = static_cast<int>(PlayTime) % 60;
	//millisecond = static_cast<int>((PlayTime - static_cast<int>(PlayTime)) * 100);
	ClearTimeText->SetText(to_string(minute) + " : " + to_string(second) + " : " + to_string(millisecond));
}

void ClearManager::ClearUpdate(double )
{
	if (NextLevelButton->IsReleased() == true)
	{
		PM::ChapterData* data = Engine::GetLevelIOManager().ReadChaterData(Engine::GetLevelIOManager().GetChapterNumber());
		int nextlevelNumber = Engine::GetLevelIOManager().GetLevelNumber() + 1;
		if(static_cast<int>(data->levels.size()) <= nextlevelNumber)
		{
			Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::LevelSelect));
		}
		else
		{
			if (Engine::GetLevelIOManager().HaveLevel(data->levels[nextlevelNumber]->levelName, "assets/Level/Chapter" + to_string(data->chapterNumber) + "/") == false)
			{
				Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::LevelSelect));
			}
			else
			{
				Engine::GetLevelIOManager().SetChapter(Engine::GetLevelIOManager().GetChapterNumber(), nextlevelNumber);
				Engine::GetGameStateManager().SetLevelState
				(data->levels[nextlevelNumber]->levelName, "assets/Level/Chapter" + to_string(data->chapterNumber) + "/");
				Engine::GetGameStateManager().ReloadState();
			}
		}
		delete data;
	}
	else if (LevelSelectButton->IsReleased() == true)
	{
		Engine::GetGameStateManager().SetNextState(static_cast<int>(Screens::LevelSelect));
	}
	else if (ReplayButton->IsReleased() == true)
	{
		Engine::GetGameStateManager().ReloadState();
	}
}

void ClearManager::ClearAnimationUpdate(double dt)
{
	lerpvalue += dt/ ClearAnimationTime;
	NextLevelButton->SetPosition({ NextLevelButton->GetPosition().x,helper::Lerp(NextLevelButton->GetPosition().y, 150.f, dt *3) });
	LevelSelectButton->SetPosition({ LevelSelectButton->GetPosition().x,helper::Lerp(LevelSelectButton->GetPosition().y, 150.f, dt*3) });
	ReplayButton->SetPosition({ ReplayButton->GetPosition().x,helper::Lerp(ReplayButton->GetPosition().y, 150.f, dt*3) });

	int m = static_cast<int>(PlayTime) / 60;
	if (minute > m)
	{
		minute = helper::Lerp(minute, m, dt * 5);
	}
	else
	{
		minute = m;
	}
	int s = static_cast<int>(PlayTime) % 60;
	if (second > s)
	{


		second = helper::Lerp(second, s, dt);
	}
	else
	{
		second = s;
	}
	int ms = static_cast<int>((PlayTime - static_cast<int>(PlayTime)) * 100);
	if (millisecond > ms)
	{
		millisecond = helper::Lerp(millisecond, ms, dt);
	}
	else
	{
		millisecond = ms;
	}
	if(Engine::GetInput().LeftMouseIsReleased() == true || ClearAnimationTimer >= ClearAnimationTime)
	{
		minute = m;
		second = s;
		millisecond = ms;
		NextLevelButton->SetPosition({ NextLevelButton->GetPosition().x, 150.f });
		LevelSelectButton->SetPosition({ LevelSelectButton->GetPosition().x,150.f});
		ReplayButton->SetPosition({ ReplayButton->GetPosition().x,150.f });
		ClearAnimationTimer = ClearAnimationTime;
		isAnimationEnd = true;
	}
}
