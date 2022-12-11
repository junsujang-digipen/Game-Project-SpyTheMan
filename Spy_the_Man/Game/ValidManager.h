#pragma once
#include "../Engine/Component.h"
class ValidManager : public PM::Component
{
public:
	void SetPhone(bool set);
	void SetButton(bool set);
	void SetLockInput(bool set);
	void Setdt(double percent);
};