#pragma once
/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.h
Purpose: This is the service provider for our games engine services
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 10/23/2021
-----------------------------------------------------------------*/
#include <glm/glm.hpp>
#include <SFML/Graphics.hpp>
#include "glslshader.h"
#include "Entity.h"

namespace math {
	class TransformMatrix;
}

namespace PM {
	class Texture {
		friend class TextureManager;

	public:
		Texture(std::string filePath, int texnum);
		~Texture();
		void Setup();
		void Draw(math::TransformMatrix displayMatrix, glm::vec2 position, glm::vec2 size, glm::vec2 texelPos = { 0, 0 }, glm::vec2 frameSize = { 0, 0 });
		void SetType(EntityType entitytype);
		void SetTrans(bool trans) { transparent = trans; }
		void SetCloset(bool clo) { closet = clo; }

	private:
		int texnum;
		bool transparent;
		bool closet;
		int Ttime;
		sf::Image* image;
		glm::mat3 xform{};
		glm::vec4 texel;
		GLuint tex_hdl = 0;
		int type;
	};
}