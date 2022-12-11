/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Animation.cpp
Project: CS200
Author: Sangbeom Kim
-----------------------------------------------------------------*/
#include "Animation.h"
#include "json/json.h"
#include "Engine.h"

PM::Animation::Animation() {
	animation.push_back(new PlayFrame(0, 10));
	animation.push_back(new Loop(0));
	ResetAnimation();
}

PM::Animation::Animation(std::string fileName, int animnum) : animSequenceIndex(0) {
	std::ifstream inFile(fileName, std::ifstream::binary);

	if (inFile.is_open() == false) {
		throw std::runtime_error("Failed to load " + fileName);
	}

	Json::Value text;
	inFile >> text;

	Json::Value anims = text["anim"];

	for (int i = anims[animnum]["start"].asInt(); i <= anims[animnum]["finish"].asInt(); i++) {
		animation.push_back(new PlayFrame(i, text["frames"][std::to_string(i)]["duration"].asDouble() / 1000));
	}

	if (anims[animnum]["loop"]["looping"].asBool() == true) {
		animation.push_back(new Loop(anims[animnum]["loop"]["loopnum"].asInt()));
	}
	if (anims[animnum]["end"].asBool() == true) {
		animation.push_back(new End());
	}
	ResetAnimation();
}

PM::Animation::~Animation() {
	for (CommandData* command : animation) {
		delete command;
	}
	animation.clear();
}

void PM::Animation::Update(double dt) {
	currPlayFrameData->Update(dt);
	if (currPlayFrameData->IsFrameDone() == true) {
		currPlayFrameData->ResetTime();
		++animSequenceIndex;
		if (animation[animSequenceIndex]->GetType() == Command::PlayFrame) {
			currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
		}
		else if (animation[animSequenceIndex]->GetType() == Command::Loop) {
			Loop* loopData = static_cast<Loop*>(animation[animSequenceIndex]);
			animSequenceIndex = loopData->GetLoopToIndex();
			if (animation[animSequenceIndex]->GetType() == Command::PlayFrame) {
				currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
			}
			else {
				Engine::GetLogger().LogError("Loop does not go to PlayFrame");
				ResetAnimation();
			}
		}
		else if (animation[animSequenceIndex]->GetType() == Command::End) {
			isAnimationDone = true;
			return;
		}
	}
}

int PM::Animation::GetDisplayFrame() {
	return currPlayFrameData->GetFrameNum();
}

void PM::Animation::ResetAnimation() {
	animSequenceIndex = 0;
	currPlayFrameData = static_cast<PlayFrame*>(animation[animSequenceIndex]);
	isAnimationDone = false;
}

bool PM::Animation::IsAnimationDone() {
	return isAnimationDone;
}

PM::Animation::PlayFrame::PlayFrame(int frame, double targetTime) : frame(frame), targetTime(targetTime), timer(0) {}
void PM::Animation::PlayFrame::Update(double dt) { timer += dt; }
bool PM::Animation::PlayFrame::IsFrameDone() { return timer >= targetTime; }
void PM::Animation::PlayFrame::ResetTime() { timer = 0; }
int PM::Animation::PlayFrame::GetFrameNum() { return frame; }

PM::Animation::Loop::Loop(int loopToIndex) : loopToIndex(loopToIndex) {}
int PM::Animation::Loop::GetLoopToIndex() { return loopToIndex; }