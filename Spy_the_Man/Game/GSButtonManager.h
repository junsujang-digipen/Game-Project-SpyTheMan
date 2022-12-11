#pragma once
#include "../Engine/Component.h"
class GSButtonManager : public PM::Component
{
public:
	GSButtonManager() : Component("GSButtonManager") {}

	void Update(double) override;
};