/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: TextureManager.cpp
Purpose: This is the service provider for our games engine services
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 10/23/2021
-----------------------------------------------------------------*/
#include "Texture.h"
#include "TextureManager.h"

PM::Texture* PM::TextureManager::Load(std::string filePath) {
    if (pathToTexture.find(filePath) == pathToTexture.end()) {
        pathToTexture[filePath] = new Texture(filePath, texnum);
        texnum++;
    }
    return pathToTexture[filePath];
}

void PM::TextureManager::Unload() {
    for (std::pair<std::string, Texture*> pathTexturePair : pathToTexture) {
        delete pathTexturePair.second;
    }
    texnum = 1;
    pathToTexture.clear();
}