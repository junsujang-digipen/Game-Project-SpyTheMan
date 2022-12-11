#pragma once
#include <vector>

#include "../../Engine/Component.h"

namespace PM {
	namespace UI {
		class Rect;
		class Text;
	}
}

class HelpText : public PM::Component
{
public:
	HelpText();
	virtual ~HelpText();
	void Update(double) override;
	void setIsValid(bool tf) override;
private:
	PM::UI::Rect* background{nullptr};
	PM::UI::Text* text{nullptr};
};
