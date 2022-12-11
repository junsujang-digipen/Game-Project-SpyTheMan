/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Window.h
Project: Spy The Man
Author: Junsu Jang
Creation date: 9/28/2021
-----------------------------------------------------------------*/
#pragma once
#include <string>
#include <glm/glm.hpp>

struct GLFWwindow;

namespace PM {
    class Window {
    public:
        bool Init(std::string windowName, int w, int h);
        void Resize(int newWidth, int newHeight);
        glm::ivec2 GetSize();
        void SetBackground(float red, float green, float blue, float alpha);
        void Clear();
        
        void Update();
        void windowDown();
        void swapBuffers();
        int windowShouldClosed();
       
        GLFWwindow* getWindow() { return ptr_window; }
        glm::dvec2 GetMousePos() { return mousePos; }
    	private:
        glm::ivec2 windowSize;
        glm::dvec2 mousePos;
        GLFWwindow* ptr_window;
    };
}
