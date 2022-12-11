/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/21/2021
-----------------------------------------------------------------*/
//#include "Input.h"
#include "Engine.h"		//GetInput
#include "Input_GL.h"
#include <json/json.h>

PM::InputKey::Keyboard PM::Input::NowButton;
glm::dvec2 PM::Input::mouseScrollVal{};

PM::Input::Input() {
	keyDown.resize(static_cast<int>(InputKey::Keyboard::Count));
	wasKeyDown.resize(static_cast<int>(InputKey::Keyboard::Count));
	bindingKeys =  KeyFileLoad("assets/KeydataFiles/default_data.json");

}
PM::Input::~Input() {
	KeyFileSave(bindingKeys);
}

std::map<std::string, PM::InputKey> PM::Input::KeyFileLoad(const std::string fileName)
{
	std::map<std::string, InputKey> loadKeys{};
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		//Engine::GetLogger().LogError("Can't open " + fileName);
		abort();
	}
	Json::Value root;
	file >> root;
	auto endItr = root.end();
	for (auto itr = root.begin();itr != endItr;++itr) {
		//auto a = itr.key().asString();
		//auto b = itr.name();
		loadKeys[itr.name()] = static_cast<PM::InputKey::Keyboard>(root[itr.name()].asInt());
			//GLKeyToPMKey(itr.key().asInt());
	}

	file.close();
	return loadKeys;
}

void PM::Input::KeyFileSave(  std::map<std::string, InputKey>&targetMap)
{
	//bindingKeys.clear();
	const std::string filename{"assets/KeydataFiles/user_customized_data.json"};
	std::ofstream file(filename);
	if (!file.is_open())
	{
		Engine::GetLogger().LogError("Can't open " + filename);
		abort();
	}
	Json::Value root;
	auto endItr = targetMap.end();
	for (auto itr = targetMap.begin();itr != endItr;++itr) {
		root[itr->first] = static_cast<int>(itr->second.get_key());
	}
	file << root;
	file.close();
}

const PM::InputKey PM::Input::GetActionKey(const std::string key)
{
	//check does action key exist
	return bindingKeys[key];
}
PM::InputKey::Keyboard PM::Input::NowKey() {
	return NowButton;
}
glm::dvec2 PM::Input::getMouseScrollValue() {
	return mouseScrollVal;
}
bool PM::Input::IsKeyDown(InputKey::Keyboard key) const {
	if(lockInput == true) {return false;}
	return keyDown[static_cast<int>(key)];
}
bool PM::Input::IsWasKeyDown(InputKey::Keyboard key) const
{
	if (lockInput == true) { return false; }
	return wasKeyDown[static_cast<int>(key)];
}
bool PM::Input::IsKeyReleased(InputKey::Keyboard key) const {
	if (lockInput == true) { return false; }
	return keyDown[static_cast<int>(key)] == false && wasKeyDown[static_cast<int>(key)] == true;
}
PM::InputKey::Keyboard PM::InputKey::get_key() {
	return button;
}

void PM::Input::SetKeyDown(InputKey::Keyboard key, bool value) {
	keyDown[static_cast<int>(key)] = value;
}

void PM::Input::Update() {
	wasKeyDown = keyDown;
	
	mouseScrollVal = {};
	LMouseWasPressed = LMousePressed;
	RMouseWasPressed = RMousePressed;
	LMousePressed = glfwGetMouseButton(PM::getWindowPtr(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	RMousePressed = glfwGetMouseButton(PM::getWindowPtr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
	glfwGetCursorPos(Engine::GetWindow().getWindow(), &mousePos.x, &mousePos.y);

}


PM::InputKey::InputKey(PM::InputKey::Keyboard button) : button(button) {}

void PM::InputKey::ReSetKey(Keyboard b)
{
	button = b;
}

bool PM::InputKey::IsKeyDown() const {
	return Engine::GetInput().IsKeyDown(button);
}
bool PM::InputKey::IsKeyDownOnce() const
{
	return Engine::GetInput().IsKeyDown(button) == true && Engine::GetInput().IsWasKeyDown(button) == false;
}
bool PM::InputKey::IsKeyReleased() const {
	return Engine::GetInput().IsKeyReleased(button);
}


void PM::Input::Key_callback_func(GLFWwindow* /*pwin*/, int key, int /*scancode*/, int action, int /*mod*/)
{
	 NowButton = GLKeyToPMKey(key);
	if (NowButton != PM::InputKey::Keyboard::None) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			Engine::GetLogger().LogVerbose("on_key_pressed");
			Engine::GetInput().SetKeyDown(NowButton, true);
		}
		else if (action == GLFW_RELEASE) {
			Engine::GetLogger().LogVerbose("on_key_released");
			Engine::GetInput().SetKeyDown(NowButton, false);
		}
	}
}
void PM::Input::MouseButton_callback_func(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/) {

}
void PM::Input::MouseScroll_callback_func(GLFWwindow* /*window*/, double xoffset, double yoffset) {
	mouseScrollVal = glm::dvec2{xoffset,yoffset};
	//Engine::GetLogger().LogDebug("X: " + std::to_string(xoffset) + "y: " + std::to_string(yoffset));
}

bool PM::Input::LeftMouseIsClicked() const
{
	return LMousePressed && !LMouseWasPressed;
}

bool PM::Input::LeftMouseIsPressed() const
{
	return  LMousePressed;
}

bool PM::Input::LeftMouseIsReleased() const
{
	return LMouseWasPressed && !LMousePressed;
}

bool PM::Input::RightMouseIsClicked() const
{
	return RMousePressed && !RMouseWasPressed;
}

bool PM::Input::RightMouseIsPressed() const
{
	return RMousePressed;
}

bool PM::Input::RightMouseIsReleased() const
{
	return RMouseWasPressed && !RMousePressed;
}


glm::vec2 PM::Input::GetMousePos()
{
	return mousePos;
}

glm::vec2 PM::Input::GetDeviceMousePos()
{
	return glm::vec2{ mousePos.x,Engine::GetWindow().GetSize().y - mousePos.y };
}

glm::vec2 PM::Input::GetWorldMousePos()
{
	return  DevicePosToWorldPos(GetDeviceMousePos());
}


