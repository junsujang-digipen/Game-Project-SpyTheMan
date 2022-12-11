/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Font.h
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <ft2build.h>
#include <freetype/freetype.h>

#include <map>
#include <string>

#include "glslshader.h"
#include "TransformMatrix.h"

namespace PM {
	struct Character
	{
		unsigned int ID;
		glm::ivec2 size;
		glm::ivec2 offset;
		unsigned int advance;
	};

	enum class AlignType
	{
		left,
		center,
		right
	};

	class Font
	{
	public:
		Font(unsigned int width, unsigned int height);
		~Font() {};
		void Load(std::string font, unsigned int fontSize);
		void Draw( std::string text, float x, float y, float scale, bool matrix, AlignType align = AlignType::left, glm::vec3 color = {0.f,0.f,0.f});
		void setup_shdrpgm();

	private:
		std::map<GLchar, Character> Characters;
		std::map<GLchar, Character>::iterator iter;
		unsigned int VAO = 0;
		unsigned int VBO = 0;
		GLSLShader shdr_pgm;

		FT_Library ft_lib = nullptr;
		FT_Face ft_face = nullptr;

		float proj_width;
		float proj_height;
	};
}
