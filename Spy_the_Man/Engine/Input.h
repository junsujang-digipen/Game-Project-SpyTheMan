/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Input.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/21/2021
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
struct GLFWwindow;
namespace PM {
	class InputKey {
	public:
		enum class Keyboard {
			None, Enter, Escape, Space, Shift, Left, Up, Right, Down,
			A, B, C, D, E, F, G, H, I, J,
			K, L, M, N, O, P, Q, R, S, T,
			U, V, W, X, Y, Z, Num0, Num1, 
			Num2, Num3, Num4, Num5, Num6, 
			Num7, Num8, Num9, Tilde,
			Count
		};
		InputKey(Keyboard button = Keyboard::None);
		void ReSetKey(Keyboard b);
		bool IsKeyDown() const;
		bool IsKeyDownOnce() const;
		bool IsKeyReleased() const;
		Keyboard get_key();
	private:
		Keyboard button;
	};

	class Input {
	public:
		Input();
		std::map<std::string, InputKey> KeyFileLoad(const std::string fileName);
		void KeyFileSave(std::map<std::string, InputKey>& targetMap);
		const InputKey GetActionKey(const std::string);
		InputKey::Keyboard NowKey();
		glm::dvec2 getMouseScrollValue();
		~Input();
		
		bool IsKeyDown(InputKey::Keyboard key) const;
		bool IsWasKeyDown(InputKey::Keyboard key) const;
		bool IsKeyReleased(InputKey::Keyboard key) const;
		void SetKeyDown(InputKey::Keyboard key, bool value);
		static void Key_callback_func(GLFWwindow* pwin, int key, int scancode, int action, int mod);
		static void MouseButton_callback_func(GLFWwindow*, int, int, int);
		static void MouseScroll_callback_func(GLFWwindow*, double, double);
		bool LeftMouseIsClicked() const;
		bool LeftMouseIsPressed() const;
		bool LeftMouseIsReleased() const;

		bool RightMouseIsClicked() const;
		bool RightMouseIsPressed() const;
		bool RightMouseIsReleased() const;

		glm::vec2 GetMousePos();
		glm::vec2 GetDeviceMousePos();
		glm::vec2 GetWorldMousePos();
		//static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		void Update();

		void SetLockInput(bool lock) { lockInput = lock; }
		bool IsLocked() { return lockInput; }
	private:
		static glm::dvec2 mouseScrollVal;
		std::vector<bool> keyDown;
		std::vector<bool> wasKeyDown;
		std::map<std::string, InputKey> bindingKeys{};
		static InputKey::Keyboard NowButton;

		glm::dvec2 mousePos;
		bool LMousePressed = false;
		bool LMouseWasPressed = false;
		bool RMousePressed = false;
		bool RMouseWasPressed = false;
		bool lockInput = false;
	};

	 
}
