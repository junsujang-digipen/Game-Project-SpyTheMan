/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Window.cpp
Project: Spy The Man
Author: Junsu Jang
Creation date: 9/28/2021
-----------------------------------------------------------------*/
#include "Engine.h"
#include "Window.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

bool PM::Window::Init(std::string windowName,int w,int h ) {
    //doodle::create_window(windowName, 1440, 810);
    //doodle::set_frame_of_reference(doodle::FrameOfReference::RightHanded_OriginBottomLeft);
    windowSize.x = w;
    windowSize.y = h;

    if (!glfwInit()) {
        std::cout << "GLFW init has failed - In window init()" << std::endl;
        return false;
    }

    //glfwSetErrorCallback();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

    ptr_window = glfwCreateWindow(w, h, windowName.c_str(), NULL, NULL);
    if (!ptr_window) {
        std::cerr << "GLFW unable to create OpenGL context - abort program - In window init()\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(ptr_window);

    //glfwSetFramebufferSizeCallback(ptr_window, fbsize_cb);
    glfwSetKeyCallback(ptr_window,  Input::Key_callback_func);
    glfwSetMouseButtonCallback(ptr_window, Input::MouseButton_callback_func);
    //glfwSetCursorPosCallback(ptr_window, GLHelper::mousepos_cb);
    glfwSetScrollCallback(ptr_window, Input::MouseScroll_callback_func);

    glfwSetInputMode(ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Unable to initialize GLEW - error: "
            << glewGetErrorString(err) << " abort program" << std::endl;
        return false;
    }
    if (GLEW_VERSION_4_5) {
        std::cout << "Using glew version: " << glewGetString(GLEW_VERSION) << std::endl;
        std::cout << "Driver supports OpenGL 4.5\n" << std::endl;
    }
    else {
        std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
        return false;
    }

    return true;
}

void PM::Window::Resize(int newWidth, int newHeight) {
    windowSize.x = newWidth;
    windowSize.y = newHeight;

    glfwSetWindowSize(ptr_window,windowSize.x,windowSize.y);
}

glm::ivec2 PM::Window::GetSize() {
    return windowSize;
}

void PM::Window::SetBackground(float red, float green, float blue, float alpha)
{
    glClearColor(red, green, blue, alpha);
}

void PM::Window::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
    //doodle::clear_background(doodle::HexColor{ color });
}

void PM::Window::Update() {
    glfwPollEvents();
    glfwGetCursorPos(ptr_window, &mousePos.x, &mousePos.y);
    //doodle::update_window();
    //no need

}

void PM::Window::windowDown()
{
    glfwDestroyWindow(ptr_window);
    glfwTerminate();
}

void PM::Window::swapBuffers()
{
    glfwSwapBuffers(ptr_window);
}

int PM::Window::windowShouldClosed()
{
    
    return glfwWindowShouldClose(ptr_window);
}

/*void on_window_resized(int new_width, int new_height) {
    Engine::GetLogger().LogEvent("Window Resized");
    Engine::GetWindow().Resize(new_width, new_height);
}*///???? doodle?