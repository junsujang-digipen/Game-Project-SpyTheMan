#pragma once
#include "../Engine/Component.h"

namespace PM {
	namespace UI {
		class Text;
		class Rect;
		class Image;
		class Button;
	}
}


class ClearManager : public PM::Component
{
public:
	ClearManager();
	~ClearManager();
	void Update(double) override;

private:
	void ClearAnimationUpdate(double dt);
	void ClearUpdate(double dt);
	void SkipClearAnimation();
	double PlayTime{ 0 };
	double ClearAnimationTimer{ 0 };
	double lerpvalue{0};
	const double ClearAnimationTime{3};
	PM::UI::Rect* Background{ nullptr };
	PM::UI::Text* ClearTimeText{ nullptr };
	PM::UI::Button* ReplayButton{nullptr};
	PM::UI::Button* LevelSelectButton{ nullptr };
	PM::UI::Button* NextLevelButton{ nullptr };

	int minute{99};
	int second{99};
	int millisecond{99};
	bool isClear{ false };
	bool isAnimationEnd{false};
};
