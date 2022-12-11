/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#include "ChapterData.h"
#include "LevelIOManager.h"
PM::Stage::~Stage()
{
}

PM::ChapterData::ChapterData(int Number)
{
	chapterNumber = Number;
}

PM::ChapterData::~ChapterData()
{
	for(auto level : levels)
	{
		delete level;
	}
	levels.clear();
}
