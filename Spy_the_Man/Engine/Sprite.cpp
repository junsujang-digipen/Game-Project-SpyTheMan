/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Sprite.cpp
Purpose: This is the service provider for our games engine services
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 10/27/2021
-----------------------------------------------------------------*/
#include "Engine.h"
#include "TransformMatrix.h"
#include "Sprite.h"
#include "Animation.h"
#include "Texture.h"
#include "ComponentManager.h"
#include "GameObject.h"
#include <json/json.h>

PM::Sprite::Sprite(std::string spriteInfoFile, EntityType entitytype) : type(entitytype) {
	Load(spriteInfoFile);
}

PM::Sprite::~Sprite() {
	for(auto a : animations)
	{
		delete a;
	}
	animations.clear();
}

void PM::Sprite::Load(std::string spriteInfoFile) {
	animations.clear();
	hotSpotList.clear();
	frameTexel.clear();

	std::ifstream inFile(spriteInfoFile, std::ifstream::binary);

	if (inFile.is_open() == false) {
		throw std::runtime_error("Failed to load " + spriteInfoFile);
	}

	Json::Value text;
	inFile >> text;
	texturePtr = Engine::GetTextureManager().Load("assets/" + text["meta"]["image"].asString());

	frameSize = glm::vec2{ text["frames"]["0"]["frame"]["w"].asInt(), text["frames"]["0"]["frame"]["h"].asInt() };

	for (int i = 0; i < text["meta"]["framecount"].asInt(); i++) {
		frameTexel.push_back(glm::vec2{ text["frames"][std::to_string(i)]["frame"]["x"].asInt(), text["frames"][std::to_string(i)]["frame"]["y"].asInt() });
	}

	animations.push_back(new Animation());

	Json::Value anims = text["anim"];
	for (unsigned int j = 0; j < anims.size(); j++) {
		animations.push_back(new Animation(spriteInfoFile, j));
	}

}

void PM::Sprite::PlayAnimation(int anim) {
	if (anim < 0 || anim >= animations.size()) {
		Engine::GetLogger().LogError("Animation " + std::to_string(anim) + " not found");
		currAnim = 0;
	}
	else {
		currAnim = anim;
		animations[currAnim]->ResetAnimation();
	}
}

void PM::Sprite::SetType(EntityType entitytype) {
	type = entitytype;
}

int PM::Sprite::GetCurrentAnim() {
	return currAnim;
}

bool PM::Sprite::IsAnimationDone() {
	return animations[currAnim]->IsAnimationDone();
}

void PM::Sprite::Update(double dt) {
	animations[currAnim]->Update(dt);
}

glm::vec2 PM::Sprite::GetFrameSize() const {
	return frameSize;
}

glm::vec2 PM::Sprite::GetFrameTexel(int frameNum) const {
	if (frameNum < 0 || frameNum > frameTexel.size()) {
		Engine::GetLogger().LogError("Bad frame request: " + std::to_string(frameNum));
		return { 0, 0 };
	}
	else {
		return frameTexel[frameNum];
	}
}

void PM::Sprite::Draw(math::TransformMatrix displayMatrix, glm::vec2 position, glm::vec2 size) {
	texturePtr->SetType(type);
	texturePtr->SetTrans(transparent);
	texturePtr->SetCloset(closet);
	texturePtr->Draw(displayMatrix, position, size, GetFrameTexel(animations[currAnim]->GetDisplayFrame()), GetFrameSize());
}