/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Project: Spy the Man
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include <json/value.h>


namespace PM
{
	class LevelData;

	struct Stage
	{
		Stage() = default;
		~Stage();
		std::string levelName{ "Empty" };
		bool IsCleard{ false };
		double ClearTime{ -1. };
	};

	struct ChapterData
	{
		ChapterData(int ChapterNumber);
		~ChapterData();
	public:
		std::vector<Stage*> levels;
		int chapterNumber{ -1 };
		bool isCleard{ false };
	};

}

