#pragma once
#include "../Engine/TransformMatrix.h"
#include "..\Engine\GameState.h"
#include "..\Engine\Texture.h"
#include "../Engine/UI/Button.h"

namespace PM
{
	namespace UI
	{
		class Rect;
		class Text;
	}
}

class PM::UI::Mouse;

class LevelSelect : public PM::GameState {
public:
	LevelSelect();
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void Draw() override;
	std::string GetName() override { return "LevelSelect"; }
	enum class SelectState
	{
		Chapter, Level, Count
	};

	void ChangeState(SelectState stat);
private:
	void ChapterUpdate(double dt);
	void LevelUpdate(double dt);
	void ChangeAnimationUpdate(double dt);

	void LevelStateLoad();
	void LevelStateUnload();
	void ChapterStateLoad();
	void ChapterStateUnload();

	std::vector<SkillType> GetLevelSkills();

	std::vector<SkillType> skills;
	std::string latestlevelName;
	
	math::TransformMatrix cam{};

	SelectState state{SelectState::Chapter};
	int sfxvolume = 10;
	int bgmvolume = 10;

	bool openingAni = false;
	bool ChangeAni = false;
	bool EndAni = false;

	glm::dvec2 OpeningRectPos{ 0,0 };
	glm::dvec2 EndRectPos{ 1600,0 };
	PM::ChapterData* SelectChapter{ nullptr };
	//°ø¿ë

	PM::UI::Text* TitleText{nullptr}; // write what chapter and what level name

	// Chapter Select
	std::vector<PM::UI::ImageButton*> ChapterButtons;
	std::vector<PM::ChapterData*> AllChapterData;
	glm::dvec2 ChapterPos{ 0,0 };
	int SelectChapterNum{0};
	// Level Select
	std::vector<PM::UI::ImageButton*> LevelButtons;
	std::vector<PM::UI::TextButton*> ChangeLevelButtons;
	glm::dvec2 LevelPos={ 0,0 };
	PM::UI::ImageButton* Back={nullptr};
	PM::UI::Text* ClearTime={ nullptr };

	std::vector<PM::UI::Button*> SkillButtons;
	SkillType currskill = SkillType::Null;
	
	int SelectStage{0};

	bool LeftMove={false};
	bool RightMove={ false };
	double zoom={ 2. };

	PM::Texture* BackgroundBackPtr={ nullptr };
	PM::Texture* BackgroundMiddlePtr={ nullptr };
	PM::Texture* BackgroundFrontPtr={ nullptr };
	
	glm::dvec2 backgroundPos={ 0,0 };

	PM::UI::Button* settings = { nullptr };
	PM::UI::Button* sfxadd = { nullptr };
	PM::UI::Button* sfxmin = { nullptr };
	PM::UI::Button* bgmadd = { nullptr };
	PM::UI::Button* bgmmin = { nullptr };
	PM::UI::Button* settingsexit = { nullptr };

	bool setting = false;
}; 
