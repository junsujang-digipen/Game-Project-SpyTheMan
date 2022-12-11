#pragma once
/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Camera.h
Purpose: This is the service provider for our games engine services
Project: Spy The Man
Author: Junsu Jang
Creation date: 10/5/2021
-----------------------------------------------------------------*/
#include <glm/glm.hpp>
namespace PM {
	class GameObject;
	class Camera {
		glm::vec2 position{};
		glm::vec2 upDirection{ 0,1 };
		glm::vec2 rightDirection{ 1,0 };
		glm::vec2 pointBindingPosition{};
		glm::vec2* bindingPosition{ &pointBindingPosition };
		GameObject* target{nullptr};
		
		glm::mat2x2 extend{};

		glm::mat3 worldToCameraToNDC{};
		//extend variable
		//bool WidthUpdate{false};
		bool matrixUpdate{false};
		bool isBinding{false};
		bool isReached{false};
		bool isMouseMoveLocked{false};
		float height{};
		float width{};
		float ar{};

		int prevHeight{3};
		int nowHeight{3};
		float heightValues[5]{500.f,1150.f,1700.f,2350.f,3000.f};

		bool isLockAr{true};

		glm::vec2 prevBindingPos{};

		glm::dvec2 prevMousePos{};
		glm::dvec2 curMousePos{};
		const double toCameraMoveTime{2.};
		double nowCameraMoveTime{ 2. };
		void arUpdate();
		void matrixUpdateFunc();
		void cameraMoveFunc(double dt);
		void bindingToTarget();
		void bindingToExtend();
		

	public:
		Camera()=default;
		void init();
		const glm::vec2 getCameraOrigin();
		const glm::vec2 getCameraPos();
		void bindTarget(GameObject* targetO);
		void bindTargetPos(glm::vec2& point);
		void pointBinding(glm::vec2 point);
		void unBinding(glm::vec2 point = {});
		void Update(double dt);
		bool IsReached() { return isReached; }
		void setIsMouseMoveLocked(bool TF) { isMouseMoveLocked = TF; }
		//rotate function?
		void setPosition(glm::vec2 pos) { position = pos; matrixUpdate = true; }
		glm::mat3 getWorldToCameraToNDC() { return worldToCameraToNDC; }
		glm::mat3 getCameraToWorld();
		glm::mat3 getScreenToCamera();
		glm::mat3 getScreenToWorld();
		void setCameraHeight(float hei) { height = hei; }
		void updateCameraHeight(float AH) { height += AH; }
		void setExtend(glm::mat2x2 ex) { extend = ex; }

		float getCameraWidth() { return width; }
		float getCameraHeight() { return height; }

		void LockAr(bool state) { isLockAr = state; }
	};
}