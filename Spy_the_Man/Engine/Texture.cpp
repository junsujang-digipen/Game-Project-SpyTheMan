/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Texture.cpp
Purpose: This is the service provider for our games engine services
Project: Spy The Man
Author: Sangbeom Kim
Creation date: 10/23/2021
-----------------------------------------------------------------*/
#define GLM_FORCE_SILENT_WARNINGS
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
#include "TransformMatrix.h"
#include "Engine.h"

PM::Texture::Texture(std::string filePath, int texnum) : tex_hdl(0), texnum(texnum) {
	image = new sf::Image;
	if (!image->loadFromFile(filePath)) {
		throw std::runtime_error("Failed to load " + filePath);
	}
	Texture::Setup();
}

PM::Texture::~Texture() {
	glDeleteTextures(texnum, &tex_hdl);
	delete image;
}


void PM::Texture::Setup() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCreateTextures(GL_TEXTURE_2D, 1, &tex_hdl);
	glTextureParameteri(tex_hdl, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(tex_hdl, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureStorage2D(tex_hdl, 1, GL_RGBA8, image->getSize().x, image->getSize().y);
	glTextureSubImage2D(tex_hdl, 0, 0, 0, image->getSize().x, image->getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image->getPixelsPtr());
	glBindTextureUnit(texnum, tex_hdl);
	image->~Image();
}

void PM::Texture::Draw(math::TransformMatrix displayMatrix, glm::vec2 position, glm::vec2 size, glm::vec2 texelPos, glm::vec2 frameSize) {
	glm::mat3 S = { {size.x / 2, 0, 0}, {0, size.y / 2, 0}, {0, 0, 1} };
	glm::mat3 T = { {1, 0, 0}, {0, 1, 0}, {position.x + size.x / 2, position.y + size.y / 2, 1} };
	xform = displayMatrix.matrix * T * S;

	if (frameSize != glm::vec2{ 0, 0 }) {
		texel = { texelPos.x / image->getSize().x, texelPos.y / image->getSize().y, 
			(texelPos.x + frameSize.x) / image->getSize().x, (texelPos.y + frameSize.y) / image->getSize().y };
	}
	else {
		texel = { 0.f, 0.f, 1.f, 1.f };
	}
	if (type == 1) {
		if (transparent == true && Ttime < 70) {
			Ttime += 1;
		}
		else if (transparent == false && Ttime > 0) {
			Ttime -= 2;
		}
	}
	GLuint shd_hdl;
	if (Engine::GetPoodle().glsetting.shader != "null") {
		shd_hdl = Engine::GetPoodle().GetShader(Engine::GetPoodle().glsetting.shader).GetHandle();
	}
	else {
		shd_hdl = Engine::GetPoodle().GetShader("tex").GetHandle();
	}
	glUseProgram(shd_hdl);
	GLuint tex_loc = glGetUniformLocation(shd_hdl, "uTex2d");
	glUniform1i(tex_loc, texnum);

	GLint uniform_var_loc1 = glGetUniformLocation(shd_hdl, "uModel_to_NDC");
	if (uniform_var_loc1 >= 0) {
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(Texture::xform));
	}
	GLint uniform_var_loc2 = glGetUniformLocation(shd_hdl, "texel");
	if (uniform_var_loc2 >= 0) {
		glUniform4fv(uniform_var_loc2, 1, glm::value_ptr(Texture::texel));
	}
	GLint uniform_var_loc3 = glGetUniformLocation(shd_hdl, "Type");
	if (uniform_var_loc3 >= 0) {
		glUniform1i(uniform_var_loc3, type);
	}
	GLint uniform_var_loc4 = glGetUniformLocation(shd_hdl, "Ttime");
	if (uniform_var_loc4 >= 0) {
		glUniform1i(uniform_var_loc4, Ttime);
	}
	GLint uniform_var_loc5 = glGetUniformLocation(shd_hdl, "Closet");
	if (uniform_var_loc5 >= 0) {
		glUniform1i(uniform_var_loc5, closet);
	}

	glBindVertexArray(Engine::GetPoodle().GetModel("tex").vaoid);
	glLineWidth(1.f);
	glDrawElements(Engine::GetPoodle().GetModel("tex").primitive_type,
		Engine::GetPoodle().GetModel("tex").draw_cnt, GL_UNSIGNED_SHORT, NULL);
	glLineWidth(1.f);
	glBindVertexArray(0);
	glUseProgram(0);
}

void PM::Texture::SetType(EntityType entitytype) {
	switch (entitytype) {
	case EntityType::Player:
		type = 1;
		break;
	case EntityType::Enemy_NPC:
		type = 2;
		break;
	case EntityType::Worker_NPC:
		type = 3;
		break;
	default:
		type = 0;
		break;
	}
}