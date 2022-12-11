/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Poodle.h
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 9/28/2021
-----------------------------------------------------------------*/
#pragma once
#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <iostream>
#include "glslshader.h"
#include "TransformMatrix.h"
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include <vector>

#define PI 3.1415926535897932384626433832795

namespace PM {
	//width values from 1 to 10
	void set_line_width(double width);

	//RGBA values from 0 to 255
	void set_line_color(double r, double g, double b, double a = 255);

	//draw polygon without line
	void no_outline();

	//draw polygon with line
	void yes_outline();

	//draw polygon without fill
	void no_fill();

	//draw polygon with fill
	void yes_fill();

	//RGBA values from 0 to 255
	void set_fill_color(double r, double g, double b, double a = 255);

	//starting point to ending point
	void draw_line(double x1, double y1, double x2, double y2);

	//3 points to make triangle
	void draw_triangle(double x1, double y1, double x2, double y2, double x3, double y3);

	//initial point and width, height
	void draw_rect(double x1, double y1, double width, double height);

	//center and radius
	void draw_ellipse(double x, double y, double width, double height);

	//image path
	void Image(std::string filePath);

	//image draw
	void draw_image(std::string filePath, math::TransformMatrix transformmatrix, glm::vec2 position, glm::vec2 size);

	//rectmode center
	void rect_center();

	//rectmode corner
	void rect_corner();

	//save settings
	void push_settings();

	//get settings
	void pop_settings();

	//use shader
	void use_shader(std::string shadname);

	//apply ndc
	void apply_NDC();

	//scale by x1, y1 respect to point x2, y2
	void apply_scale(double x1, double y1, double x2, double y2);

	//rotate by angle of degrees counter-clockwise respect to point x, y
	void apply_rotate(double angle, double x, double y);

	//translate by x, y
	void apply_translate(double x, double y);

	//scale, rotate, translate by matrix
	void apply_matrix(glm::mat3 srtmat);

	glm::vec2 DevicePosToWorldPos(glm::vec2 devicePos);

	glm::vec2 WorldPosToDevicePos(glm::vec2 WorldPos);

	class Poodle {
	public:
		void shape_init();
		void poly_s_init();
		void tex_s_init();
		void holo_s_init();
		void title_s_init();
		void player_s_init();

	private:
		struct Vertex {
			GLfloat x;
			GLfloat y;

			GLfloat tnum;
		};

		struct GLModel {
			GLenum primitive_type;
			GLuint primitive_cnt;
			GLuint vaoid;
			GLuint draw_cnt;
		};

		std::map<std::string, GLModel> models;
		std::map<std::string, GLSLShader> shd_ref;

		void linesetup();
		void trisetup();
		void texsetup();
		void ellipsetup();

		struct GLObject {
			glm::vec4 color;
			glm::mat3 mdl_xform;
			glm::mat3 mdl_to_ndc_xform;
			glm::mat3 xform;
			std::string mdl_ref;
			GLfloat width = 0;

			void draw();
			void SetUniform(GLuint handle);
			void holoUniform(GLuint handle);
			void titleUniform(GLuint handle);
		
		};

	public:
		GLModel GetModel(std::string modname);
		GLSLShader GetShader(std::string shadname);

		GLObject globject;

		struct GLSetting{
			//colors
			GLfloat LR = 0.f;
			GLfloat LG = 0.f;
			GLfloat LB = 0.f;
			GLfloat LA = 1.f;
			GLfloat FR = 0.f;
			GLfloat FG = 0.f;
			GLfloat FB = 0.f;
			GLfloat FA = 1.f;

			//line width
			GLfloat LW = 1.f;

			GLboolean noline = GL_FALSE;
			GLboolean nofill = GL_FALSE;

			//rectmode
			GLboolean rectcenter = GL_FALSE;

			//matrix
			glm::mat3 scalemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
			glm::mat3 rotatemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
			glm::mat3 translatemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
			glm::mat3 transformmat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };

			//shader
			std::string shader = "null";

			void Clear();
		};

		GLSetting glsetting;
		std::vector<GLSetting> tempsettings;
	};
}