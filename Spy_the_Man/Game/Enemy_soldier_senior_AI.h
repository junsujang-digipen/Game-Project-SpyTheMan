#pragma once
/*--------------------------------------------------------------
File Name: Enemy_soldier_senior_AI.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 11/10/2021
-----------------------------------------------------------------*/
#include "Enemy_soldier_AI.h"
class Enemy_soldier_senior_AI :public Enemy_soldier_AI {
	void taskSelectAlgo(double dt)override;
public:
	Enemy_soldier_senior_AI(PM::GameObject* AIOwner);

};