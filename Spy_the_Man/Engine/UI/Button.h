/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Seongwon Jang
-----------------------------------------------------------------*/
#pragma once
#include "UIEntity.h"

namespace PM {
	class Font;
	class Texture;
}

namespace PM
{	
	namespace UI
	{
		enum ButtonState : int//Image state
		{
			Normal,
			Pressed,
			Hovering,
			Lock
		};
		class Button : public UIEntity
		{
		private:

			glm::vec4 color;
			bool hovering;
			bool locked;
			bool isPressed;
			bool isReleased;
		public:
			Button(std::string name, glm::vec2 pos, glm::vec2 size, glm::vec4 color = { 0,0,0, 150 }, UILayer layer = UILayer::DEFAULT, bool locked = false ,bool isavalid = true);
			virtual ~Button(){}
			void Update(double dt) override;
			void Draw(math::TransformMatrix& displayMatrix) const override;
			void SetSize(glm::vec2 newSize) override;
			void SetLock(bool lock) {locked = lock;}
			bool GetLock() const { return locked; }
			bool IsReleased() const;
			bool IsPressed() const;
			void Setvalid(bool set) override;
			ButtonState GetButtonState() const;
			void CollisionStay(Entity*) override;
			void CollisionExit(Entity*) override;
			
		};

		
		class ImageButton : public Button
		{
		public:
			ImageButton(std::string name, std::string filepath, glm::vec2 pos, glm::vec2 size, UILayer layer = UILayer::DEFAULT, bool locked = false, bool isvalid = true);
			virtual ~ImageButton();
			void Draw(math::TransformMatrix& displayMatrix) const override;
		protected:
			virtual glm::vec4 GetStateColor() const;
			PM::Texture* texturePtr;
		};

		class HoloImageButton : public  ImageButton
		{
		public:
			HoloImageButton(std::string name, std::string filepath, glm::vec2 pos, glm::vec2 size, UILayer layer = UILayer::DEFAULT, bool locked = false, bool isvalid = true);
			virtual ~HoloImageButton();
		protected:
			glm::vec4 GetStateColor() const override;
		};

		class TextButton : public Button
		{
		public:
			TextButton(std::string name, glm::vec2 pos, glm::vec2 size, std::string text, glm::vec4 color = { 0,0,0,150 }, glm::vec3 txtcolor = { 255,255,255 },  UILayer layer = UILayer::DEFAULT, bool locked = false, bool isavalid = true);

			virtual ~TextButton();
			void Update(double dt) override;
			void Draw(math::TransformMatrix& displayMatrix) const override;
			Font* font;
			std::string text;
			glm::vec3 tColor;
		};


	}
}

