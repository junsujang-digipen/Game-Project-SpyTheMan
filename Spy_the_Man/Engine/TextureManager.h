#pragma once
/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.h
Purpose: This is the service provider for our games engine services
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 10/23/2021
-----------------------------------------------------------------*/
#include <glm/glm.hpp>
#include <map>

namespace PM {
	class Texture;
	
	class TextureManager {
	public:
		Texture* Load(std::string filePath);
		void Unload();
		std::map<std::string, Texture*> pathToTexture;

	private:
		int texnum = 1;
	};
}