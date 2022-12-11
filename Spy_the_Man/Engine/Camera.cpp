/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.cpp
Purpose: This is the service provider for our games engine services
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/5/2021
-----------------------------------------------------------------*/
#include "Engine.h"
#include "Camera.h"
#include <gsl/gsl>
namespace PM {

	void Camera::init()
	{
		isLockAr = false;
		setCameraHeight(heightValues[2]);
		//setCameraHeight(static_cast<float>(Engine::GetWindow().GetSize().y));
	}
	void Camera::bindTarget(GameObject* targetO)
	{
		target = targetO;
		bindingPosition = (const_cast<glm::vec2*>(&target->GetPosition()));
		isBinding = true; 
		//setPosition(*bindingPosition);
	}
	void Camera::bindTargetPos(glm::vec2& point)
	{
		target = nullptr;
		bindingPosition = &point;
		isBinding = true;
	}
	void Camera::pointBinding(glm::vec2 point)
	{
		target = nullptr;
		pointBindingPosition = point; 
		bindingPosition = &pointBindingPosition;
		isBinding = true;
		//setPosition(*bindingPosition);
	}
	void Camera::unBinding(glm::vec2 point)
	{
		target = nullptr;
		pointBindingPosition = point;
		bindingPosition = &pointBindingPosition;
		isBinding = false;
	}
	void Camera::Update(double dt) {
		//zoom in,out
		arUpdate();

		cameraMoveFunc(dt);
		bindingToTarget();
		bindingToExtend();

		matrixUpdateFunc();
	}

	glm::mat3 Camera::getCameraToWorld() {
		glm::vec2 cameraOrigin{ getCameraOrigin() };

		return glm::mat3{ {1.f, 0.f, 0.f},
			{0.f, 1.f, 0.f},
			{cameraOrigin.x, cameraOrigin.y, 1.f}
	};
	}
	// worldPos -> devicePos
	// devicePos -> WorldPos
	glm::mat3 Camera::getScreenToCamera()
	{
		glm::fvec2 winSize = Engine::GetWindow().GetSize();

		glm::mat3 rval{ {width / winSize.x, 0.f, 0.f},
			{0.f, -height / winSize.y, 0.f},
			{-width / 2,height/2, 1.f }
		};
		return rval;
	}

	glm::mat3 Camera::getScreenToWorld()
	{
		return getCameraToWorld()*getScreenToCamera();
	}

	void Camera::arUpdate() {
		if (isLockAr == false) {
			const glm::dvec2 nowMouseScrollVal{ Engine::GetInput().getMouseScrollValue() };
			//Engine::GetLogger().LogDebug("X: " + std::to_string(nowMouseScrollVal.x) + "y: " + std::to_string(nowMouseScrollVal.y));
			glm::vec2 winSize = { Engine::GetWindow().GetSize().x,Engine::GetWindow().GetSize().y };
			ar = width / height;
			if (ar != winSize.x / winSize.y || nowMouseScrollVal.y != 0) {
				ar = winSize.x / winSize.y;
				if (nowMouseScrollVal.y < 0 && nowHeight < 4) {
					prevHeight = nowHeight;
					++nowHeight;
				}
				else if (nowMouseScrollVal.y > 0 && nowHeight > 0) {
					prevHeight = nowHeight;
					--nowHeight;
				}

				const float previousHeight = width / ar;
				width = ar * height;
				setPosition(position - glm::vec2{ width - previousHeight * ar ,height - previousHeight } / 2.f);
				matrixUpdate = true;
			}
		}
		if (prevHeight != nowHeight) {
			const float scrollSpeed{ (heightValues[nowHeight] - height) / 10.f };
			height += scrollSpeed;

			if (height > heightValues[4]) {//Change
				height = heightValues[4];
			}
			else if (height < heightValues[0]) {//Change
				height = heightValues[0];
			}

			if (height == heightValues[nowHeight]) {
				prevHeight = nowHeight;
			}

			const float previousHeight = width / ar;
			width = ar * height;
			setPosition(position - glm::vec2{ width - previousHeight * ar ,height - previousHeight } / 2.f);
			matrixUpdate = true;
		}
	}
	void Camera::matrixUpdateFunc()
	{
		if (matrixUpdate==true) {
			//compute matrix
			//input matrix
			glm::vec2 cameraOrigin{ getCameraOrigin() };
			worldToCameraToNDC = glm::mat3{ {2.f / width,0.f,0.f},{0.f,2.f / height,0.f},{0.f,0.f,1.f} } *glm::mat3{ {1.f, 0.f,0.f},{ 0.f, 1.f, 0.f},{-cameraOrigin.x ,-cameraOrigin.y, 1.f} };
			//worldToCameraToNDC = glm::mat3{ {1, 0, 0}, {0, 1, 0}, {0.5, 0.5, 1} };
			matrixUpdate = false;
		}
	}
	void Camera::cameraMoveFunc(double dt)
	{
		// chase targetddd
		isReached = true;
		glm::vec2 cameraOrigin{ getCameraOrigin() };
		constexpr double minDist{10.};
		glm::vec2 direc{ *bindingPosition - cameraOrigin };
		if (target != nullptr) {
			direc += target->GetSize() / 2.f;
		}
		const double dist{glm::length(direc) };
		if (dist < 10.) {
			direc = glm::normalize(direc)*10.f;
		}
		if (isBinding == true && (nowCameraMoveTime <=0. &&  dist > minDist || prevBindingPos != *bindingPosition)) {
			position = position + direc * glm::vec2{dt,dt};
			matrixUpdate = true;
			nowCameraMoveTime = 0.;
			isReached = false;
		}
		
		int mouseLeft = glfwGetMouseButton(PM::getWindowPtr(),GLFW_MOUSE_BUTTON_LEFT);
		glfwGetCursorPos(Engine::GetWindow().getWindow(), &curMousePos.x, &curMousePos.y);
		if (isMouseMoveLocked == false && mouseLeft == GLFW_PRESS) {
			glfwGetCursorPos(Engine::GetWindow().getWindow(), &curMousePos.x, &curMousePos.y);
			const double distX = (curMousePos.x - prevMousePos.x);
			const double distY = (curMousePos.y - prevMousePos.y);

			if (distX != 0 || distY != 0) {
				//position - mouse_move
				position = position - glm::vec2{ getScreenToCamera() * glm::vec3{static_cast<float>(distX),static_cast<float>(distY),0} };
				matrixUpdate = true;
				nowCameraMoveTime = toCameraMoveTime;
			}
			prevMousePos = curMousePos;
		}
		else {
			nowCameraMoveTime -= dt;
		}
		prevMousePos = curMousePos;
		prevBindingPos = *bindingPosition;
		// mouse move
		// matrixUpdate on
	}
	void Camera::bindingToTarget()
	{
		//binding camera move to target when binding is true
		
	}

	void Camera::bindingToExtend()
	{
		//binding camera move to extend 
		if (extend[0].x != extend[1].x) {
			if (position.x < extend[0].x) {
				position.x = extend[0].x;
			}
			else if (position.x + width > extend[1].x) {
				position.x = extend[1].x - width;
			}
			if (position.y < extend[0].y) {
				position.y = extend[0].y;
			}
			else if (position.y + height > extend[1].y) {
				position.y = extend[1].y - height;
			}
		}

	}

	const glm::vec2 Camera::getCameraOrigin()
	{
		glm::vec2 cameraOrigin{ (position.x + width / 2) ,  (position.y + height / 2) };

		return cameraOrigin;
	}

	const glm::vec2 Camera::getCameraPos()
	{
		return position;
	}

}