/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input_GL.h
Purpose: Interface between doodle and our engine
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/21/2021
-----------------------------------------------------------------*/
#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Input.h"


 PM::InputKey::Keyboard GLKeyToPMKey(int button) {
	if (button == GLFW_KEY_ENTER) {
		return PM::InputKey::Keyboard::Enter;
	}
	else if  (button == GLFW_KEY_ESCAPE) {
		return PM::InputKey::Keyboard::Escape;
	}
	else if  (button == GLFW_KEY_SPACE) {
		return PM::InputKey::Keyboard::Space;
	}
	else if (button == GLFW_KEY_LEFT_SHIFT || button == GLFW_KEY_RIGHT_SHIFT) {
		return PM::InputKey::Keyboard::Shift;
	}
	else if  (button == GLFW_KEY_LEFT) {
		return PM::InputKey::Keyboard::Left;
	}
	else if  (button == GLFW_KEY_RIGHT) {
		return PM::InputKey::Keyboard::Right;
	}
	else if  (button == GLFW_KEY_UP) {
		return PM::InputKey::Keyboard::Up;
	}
	else if  (button == GLFW_KEY_DOWN) {
		return PM::InputKey::Keyboard::Down;
	}
	else if  (button == GLFW_KEY_DOWN) {
		return PM::InputKey::Keyboard::Down;
	}
	else if  (button == GLFW_KEY_GRAVE_ACCENT) {
		return PM::InputKey::Keyboard::Tilde;
	}
	else if  (button >= GLFW_KEY_A && button <= GLFW_KEY_Z) {
		const int offset = static_cast<int>(button) - static_cast<int>(GLFW_KEY_A);
		return static_cast<PM::InputKey::Keyboard>(static_cast<int>(PM::InputKey::Keyboard::A) + offset);
	}
	else if (button >= GLFW_KEY_0 && button <= GLFW_KEY_9) {
		const int offset = static_cast<int>(button) - static_cast<int>(GLFW_KEY_0);
		
		return static_cast<PM::InputKey::Keyboard>(static_cast<int>(PM::InputKey::Keyboard::Num0) + offset);
	}

	return PM::InputKey::Keyboard::None;
}
 //template<int button>
 //struct GLToPMKey {
	// static constexpr PM::InputKey::Keyboard key = PM::InputKey::Keyboard::None;
 //};
 //template<>
 //struct GLToPMKey {
	// static constexpr PM::InputKey::Keyboard key = PM::InputKey::Keyboard::None;
 //};