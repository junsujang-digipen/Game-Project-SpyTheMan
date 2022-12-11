/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Poodle.cpp
Project: Spy the Man
Author: Sangbeom Kim
Creation date: 9/28/2021
-----------------------------------------------------------------*/
#define GLM_FORCE_SILENT_WARNINGS
#include "Poodle.h"
#include "Engine.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

void PM::set_line_width(double width) {
	Engine::GetPoodle().glsetting.LW = static_cast<float>(width);
}

void PM::set_line_color(double r, double g, double b, double a) {
	Engine::GetPoodle().glsetting.LR = static_cast<float>(r) / 255.f;
	Engine::GetPoodle().glsetting.LG = static_cast<float>(g) / 255.f;
	Engine::GetPoodle().glsetting.LB = static_cast<float>(b) / 255.f;
	Engine::GetPoodle().glsetting.LA = static_cast<float>(a) / 255.f;
}

void PM::no_outline() {
	Engine::GetPoodle().glsetting.noline = GL_TRUE;
}

void PM::yes_outline() {
	Engine::GetPoodle().glsetting.noline = GL_FALSE;
}

void PM::no_fill() {
	Engine::GetPoodle().glsetting.nofill = GL_TRUE;
}

void PM::yes_fill() {
	Engine::GetPoodle().glsetting.nofill = GL_FALSE;
}

void PM::set_fill_color(double r, double g, double b, double a) {
	Engine::GetPoodle().glsetting.FR = static_cast<float>(r) / 255.f;
	Engine::GetPoodle().glsetting.FG = static_cast<float>(g) / 255.f;
	Engine::GetPoodle().glsetting.FB = static_cast<float>(b) / 255.f;
	Engine::GetPoodle().glsetting.FA = static_cast<float>(a) / 255.f;
}

void PM::draw_line(double x1, double y1, double x2, double y2) {
	glm::vec2 length = { x2 - x1, y2 - y1 };
	double angle = atan(length.y / length.x);
	double len = 0;
	if (length.x < 0) {
		len = - sqrt(pow(length.x, 2) + pow(length.y, 2));
	}
	else {
		len = sqrt(pow(length.x, 2) + pow(length.y, 2));
	}
	glm::mat3 R = { {cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}, {0, 0, 1} };
	glm::mat3 S = { {len, 0, 0}, {0, len, 0}, {0, 0, 1} };
	glm::mat3 T = { {1, 0, 0}, {0, 1, 0}, {x1, y1, 1} };
	Engine::GetPoodle().globject.color = { Engine::GetPoodle().glsetting.LR, Engine::GetPoodle().glsetting.LG, 
		Engine::GetPoodle().glsetting.LB, Engine::GetPoodle().glsetting.LA };
	Engine::GetPoodle().globject.width = Engine::GetPoodle().glsetting.LW;
	Engine::GetPoodle().globject.mdl_xform = T * R * S;
	Engine::GetPoodle().globject.xform = Engine::GetPoodle().glsetting.transformmat * Engine::GetPoodle().glsetting.translatemat * 
		Engine::GetPoodle().glsetting.rotatemat * Engine::GetPoodle().glsetting.scalemat;
	Engine::GetPoodle().globject.mdl_ref = "line";
	Engine::GetPoodle().globject.draw();
}

void PM::draw_triangle(double x1, double y1, double x2, double y2, double x3, double y3) {
	if (Engine::GetPoodle().glsetting.nofill == GL_FALSE) {
		if (((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) <= 0) {
			std::swap(x2, x3);
			std::swap(y2, y3);
		}
		glm::vec2 length12 = { x2 - x1, y2 - y1 };
		glm::vec2 length13 = { x3 - x1, y3 - y1 };
		double inc = length12.y / length12.x;
		double angle = atan(inc);
		if (length12.x == 0) {
			angle = PI / 2;
		}
		if ((angle < 0 && length12.y > 0) || (angle > 0 && length12.y < 0)) {
			angle = PI + angle;
		}
		glm::mat3 R = { {cos(angle), sin(angle), 0}, {-sin(angle), cos(angle), 0}, {0, 0, 1} };
		glm::mat3 IR = { {cos(angle), -sin(angle), 0}, {sin(angle), cos(angle), 0}, {0, 0, 1} };
		glm::mat3 pos1 = IR * glm::mat3{ {x1, y1, 1}, {0, 0, 0}, {0, 0, 0} };
		glm::mat3 pos2 = IR * glm::mat3{ {x2, y2, 1}, {0, 0, 0}, {0, 0, 0} };
		glm::mat3 pos3 = IR * glm::mat3{ {x3, y3, 1}, {0, 0, 0}, {0, 0, 0} };
		double height = static_cast<double>(pos3[0][1]) - static_cast<double>(pos1[0][1]);
		double width = static_cast<double>(pos2[0][0]) - static_cast<double>(pos1[0][0]);
		double shear = static_cast<double>(pos3[0][0]) - static_cast<double>(pos1[0][0]);
		glm::mat3 S = { {width, 0, 0}, {shear, height, 0}, {0, 0, 1} };
		glm::mat3 T = { {1, 0, 0}, {0, 1, 0}, {x1, y1, 1} };
		Engine::GetPoodle().globject.mdl_xform = T * R * S;
		Engine::GetPoodle().globject.xform = Engine::GetPoodle().glsetting.transformmat * Engine::GetPoodle().glsetting.translatemat * 
			Engine::GetPoodle().glsetting.rotatemat * Engine::GetPoodle().glsetting.scalemat;
		Engine::GetPoodle().globject.color = { Engine::GetPoodle().glsetting.FR, Engine::GetPoodle().glsetting.FG,
			Engine::GetPoodle().glsetting.FB, Engine::GetPoodle().glsetting.FA };
		Engine::GetPoodle().globject.mdl_ref = "triangle";
		Engine::GetPoodle().globject.draw();
	}
	if (Engine::GetPoodle().glsetting.noline == GL_FALSE) {
		draw_line(x1, y1, x2, y2);
		draw_line(x2, y2, x3, y3);
		draw_line(x3, y3, x1, y1);
	}
}

void PM::draw_rect(double x1, double y1, double width, double height) {
	glm::vec2 pos1{0, 0};
	glm::vec2 pos2{0, 0};
	glm::vec2 pos3{0, 0};
	glm::vec2 pos4{0, 0};
	if (Engine::GetPoodle().glsetting.rectcenter == GL_FALSE) {
		pos1 = { x1, y1 };
		pos2 = { x1 + width, y1 };
		pos3 = { x1 + width, y1 + height };
		pos4 = { x1, y1 + height };
	}
	else {
		pos1 = { x1 - width / 2, y1 - height / 2 };
		pos2 = { x1 + width / 2, y1 - height / 2 };
		pos3 = { x1 + width / 2, y1 + height / 2 };
		pos4 = { x1 - width / 2, y1 + height / 2 };
	}
	if (Engine::GetPoodle().glsetting.nofill == GL_FALSE) {
		if (Engine::GetPoodle().glsetting.noline == GL_FALSE) {
			no_outline();
			draw_triangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y);
			draw_triangle(pos1.x, pos1.y, pos3.x, pos3.y, pos4.x, pos4.y);
			yes_outline();
		}
		else {
			draw_triangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y);
			draw_triangle(pos1.x, pos1.y, pos3.x, pos3.y, pos4.x, pos4.y);
		}
	}
	if (Engine::GetPoodle().glsetting.noline == GL_FALSE) {
		draw_line(pos1.x, pos1.y, pos2.x, pos2.y);
		draw_line(pos2.x, pos2.y, pos3.x, pos3.y);
		draw_line(pos3.x, pos3.y, pos4.x, pos4.y);
		draw_line(pos4.x, pos4.y, pos1.x, pos1.y);
	}
}

void PM::draw_ellipse(double x, double y, double width, double height) {
	if (Engine::GetPoodle().glsetting.nofill == GL_FALSE) {
		glm::mat3 S = { {width / 2, 0, 0}, {0, height / 2, 0}, {0, 0, 1} };
		glm::mat3 T = { {1, 0, 0}, {0, 1, 0}, {x, y, 1} };
		Engine::GetPoodle().globject.mdl_xform = T * S;
		Engine::GetPoodle().globject.xform = Engine::GetPoodle().glsetting.transformmat * Engine::GetPoodle().glsetting.translatemat * 
			Engine::GetPoodle().glsetting.rotatemat * Engine::GetPoodle().glsetting.scalemat;
		Engine::GetPoodle().globject.color = { Engine::GetPoodle().glsetting.FR, Engine::GetPoodle().glsetting.FG, 
			Engine::GetPoodle().glsetting.FB, Engine::GetPoodle().glsetting.FA };
		Engine::GetPoodle().globject.mdl_ref = "ellipse";
		Engine::GetPoodle().globject.draw();
	}
	if (Engine::GetPoodle().glsetting.noline == GL_FALSE) {
		double slice = PI / 25;
		for (int i = 0; i < 50; ++i) {
			int j = i + 1;
			double x1 = x + width / 2 * cos(slice * i);
			double y1 = y + height / 2 * sin(slice * i);
			double x2 = x + width / 2 * cos(slice * j);
			double y2 = y + height / 2 * sin(slice * j);
			draw_line(x1, y1, x2, y2);
		}
	}
}

void PM::Image(std::string filePath) {
	Engine::GetTextureManager().Load(filePath);
}

void PM::draw_image(std::string filePath, math::TransformMatrix transformmatrix, glm::vec2 position, glm::vec2 size) {
	Engine::GetTextureManager().pathToTexture[filePath]->Draw(transformmatrix, position, size);
}

void PM::rect_center() {
	Engine::GetPoodle().glsetting.rectcenter = GL_TRUE;
}

void PM::rect_corner() {
	Engine::GetPoodle().glsetting.rectcenter = GL_FALSE;
}

void PM::push_settings() {
	Poodle::GLSetting tempsetting;
	tempsetting = Engine::GetPoodle().glsetting;
	Engine::GetPoodle().tempsettings.push_back(tempsetting);
	Engine::GetPoodle().glsetting.Clear();
}

void PM::pop_settings() {
	Engine::GetPoodle().glsetting = Engine::GetPoodle().tempsettings.back();
	Engine::GetPoodle().tempsettings.pop_back();
}

void PM::use_shader(std::string shadname) {
	Engine::GetPoodle().glsetting.shader = shadname;
}

void PM::apply_NDC() {
	glm::mat3 NDC{ {2.f / Engine::GetWindow().GetSize().x, 0, 0}, {0, 2.f / Engine::GetWindow().GetSize().y, 0}, {-1.f, -1.f, 1.f} };
	Engine::GetPoodle().glsetting.transformmat = NDC * Engine::GetPoodle().glsetting.transformmat;
}

void PM::apply_scale(double x1, double y1, double x2, double y2) {
	Engine::GetPoodle().glsetting.scalemat = glm::mat3{ {x1, 0, 0}, {0, y1, 0}, {x2, y2, 1} }
	* glm::mat3{ {1, 0, 0}, {0, 1, 0}, { - x2, - y2, 1} } * Engine::GetPoodle().glsetting.scalemat;
}

void PM::apply_rotate(double angle, double x, double y) {
	Engine::GetPoodle().glsetting.rotatemat = glm::mat3{ {cos(angle), sin(angle), 0}, { -sin(angle), cos(angle), 0}, {x, y, 1} }
	* glm::mat3{ {1, 0, 0}, {0, 1, 0}, { - x, - y, 1} } * Engine::GetPoodle().glsetting.rotatemat;
}

void PM::apply_translate(double x, double y) {
	Engine::GetPoodle().glsetting.translatemat = glm::mat3{ {1, 0, 0}, {0, 1, 0}, {x, y, 1} } * Engine::GetPoodle().glsetting.translatemat;
}

void PM::apply_matrix(glm::mat3 srtmat) {
	Engine::GetPoodle().glsetting.transformmat = srtmat * Engine::GetPoodle().glsetting.transformmat;
}

glm::vec2 PM::DevicePosToWorldPos(glm::vec2 devicePos) //device == left down
{
	devicePos.y = -(devicePos.y - Engine::GetWindow().GetSize().y);
	glm::vec3 result = Engine::GetCamera().getScreenToWorld() * glm::vec3{ devicePos.x, devicePos.y, 1.f }; 
	return glm::vec2(result.x,result.y);
}

glm::vec2 PM::WorldPosToDevicePos(glm::vec2 WorldPos) //device == left down
{
	glm::vec3 result = glm::inverse(Engine::GetCamera().getScreenToWorld()) * 
		glm::vec3(WorldPos.x,WorldPos.y,1.f);
	result.y = Engine::GetWindow().GetSize().y - result.y;
	return glm::vec2(result.x,result.y);
}

void PM::Poodle::linesetup() {
	GLModel glmodel;
	std::vector<glm::vec2> pos_vtx{ {0, 0}, {1, 0} };
	GLuint pos_cnt = static_cast<int>(pos_vtx.size());
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	glBindVertexArray(0);
	glmodel.vaoid = vao_hdl;
	glmodel.draw_cnt = pos_cnt;
	glmodel.primitive_type = GL_LINES;
	glmodel.primitive_cnt = pos_cnt / 2;
	models["line"] = glmodel;
}

void PM::Poodle::trisetup() {
	GLModel glmodel;
	std::vector<glm::vec2> pos_vtx{ {0, 0}, {1, 0}, {0, 1} };
	std::vector<GLushort> idx_vtx{ 0, 1, 2 };
	GLuint idx_elem_cnt = static_cast<int>(idx_vtx.size());
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	std::cout << vao_hdl << std::endl;
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_elem_cnt,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);
	glmodel.vaoid = vao_hdl;
	glmodel.draw_cnt = idx_elem_cnt;
	glmodel.primitive_type = GL_TRIANGLES;
	glmodel.primitive_cnt = idx_elem_cnt - 2;
	models["triangle"] = glmodel;
}

void PM::Poodle::ellipsetup() {
	GLModel glmodel;
	std::vector<glm::vec2> pos_vtx;
	std::vector<GLushort> idx_vtx;
	double slice = PI / 25;
	pos_vtx.push_back({ 0, 0 });
	for (int i = 0; i < 50; i++) {
		double x1 = cos(slice * i);
		double y1 = sin(slice * i);
		pos_vtx.push_back({ x1, y1 });
	}
	for (GLushort j = 0; j <= 50; j++) {
		idx_vtx.push_back(j);
	}
	idx_vtx.push_back(1);
	GLuint idx_elem_cnt = static_cast<int>(idx_vtx.size());
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data(), GL_DYNAMIC_STORAGE_BIT);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	std::cout << vao_hdl << std::endl;
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * idx_elem_cnt,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);
	glmodel.vaoid = vao_hdl;
	glmodel.draw_cnt = idx_elem_cnt;
	glmodel.primitive_type = GL_TRIANGLE_FAN;
	glmodel.primitive_cnt = idx_elem_cnt - 2;
	models["ellipse"] = glmodel;
}

void PM::Poodle::texsetup() {
	GLModel glmodel;
	std::vector<glm::vec2> pos_vtx{ {-1, -1}, {1, -1}, {-1, 1}, {1, 1} };
	std::vector<GLshort> idx_vtx{ 0, 1, 2, 3 };
	static const Vertex Vertices[] = { Vertex{pos_vtx[0].x, pos_vtx[0].y, 0},
		Vertex{pos_vtx[1].x, pos_vtx[1].y, 1},
		Vertex{pos_vtx[2].x, pos_vtx[2].y, 2},
		Vertex{pos_vtx[3].x, pos_vtx[3].y, 3} };
	GLuint vbo_hdl;
	glCreateBuffers(1, &vbo_hdl);
	glNamedBufferStorage(vbo_hdl, sizeof(Vertex) * 4, Vertices, 0);
	GLuint vao_hdl;
	glCreateVertexArrays(1, &vao_hdl);
	glEnableVertexArrayAttrib(vao_hdl, 0);
	glVertexArrayAttribBinding(vao_hdl, 0, 0);
	glVertexArrayAttribFormat(vao_hdl, 0, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, x));
	glEnableVertexArrayAttrib(vao_hdl, 1);
	glVertexArrayAttribBinding(vao_hdl, 1, 0);
	glVertexArrayAttribFormat(vao_hdl, 1, 1, GL_FLOAT, GL_FALSE, offsetof(Vertex, tnum));
	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl, sizeof(GLushort) * 4,
		reinterpret_cast<GLvoid*>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(Vertex));
	glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
	glBindVertexArray(0);
	glmodel.vaoid = vao_hdl;
	glmodel.draw_cnt = 4;
	glmodel.primitive_type = GL_TRIANGLE_STRIP;
	glmodel.primitive_cnt = 2;
	models["tex"] = glmodel;
}

PM::Poodle::GLModel PM::Poodle::GetModel(std::string modname) {
	if (models.find(modname) != models.end()) {
		return models[modname];
	}
	else {
		throw std::runtime_error(modname + " does not exist");
	}
}

GLSLShader PM::Poodle::GetShader(std::string shadname) {
	if (shd_ref.find(shadname) != shd_ref.end()) {
		return shd_ref[shadname];
	}
	else {
		throw std::runtime_error(shadname + " does not exist");
	}
}

void PM::Poodle::GLObject::draw() {
	GLuint shd_hdl;
	if (Engine::GetPoodle().glsetting.shader != "null") {
		shd_hdl = Engine::GetPoodle().GetShader(Engine::GetPoodle().glsetting.shader).GetHandle();
	}
	else {
		shd_hdl = Engine::GetPoodle().GetShader("polygon").GetHandle();
	}
	glUseProgram(shd_hdl);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	mdl_to_ndc_xform = xform * mdl_xform;

	SetUniform(shd_hdl);
	if(Engine::GetPoodle().glsetting.shader == "Hologram")
	{
		holoUniform(shd_hdl);
	}
	else if(Engine::GetPoodle().glsetting.shader == "Title")
	{
		titleUniform(shd_hdl);
	}
	glBindVertexArray(Engine::GetPoodle().models[mdl_ref].vaoid);
	if (Engine::GetPoodle().models[mdl_ref].primitive_type == GL_LINES) {
		glLineWidth(width);
		glDrawArrays(GL_LINES, 0, Engine::GetPoodle().models[mdl_ref].draw_cnt);
		Engine::GetPoodle().globject.width = 1.f;
		glLineWidth(1.f);
	}
	else if (Engine::GetPoodle().models[mdl_ref].primitive_type == GL_TRIANGLES) {
		glLineWidth(1.f);
		glDrawElements(GL_TRIANGLES, Engine::GetPoodle().models[mdl_ref].draw_cnt, GL_UNSIGNED_SHORT, NULL);
		glLineWidth(1.f);
	}
	else if (Engine::GetPoodle().models[mdl_ref].primitive_type == GL_TRIANGLE_FAN) {
		glLineWidth(1.f);
		glDrawElements(GL_TRIANGLE_FAN, Engine::GetPoodle().models[mdl_ref].draw_cnt, GL_UNSIGNED_SHORT, NULL);
		glLineWidth(1.f);
	}
	glUseProgram(0);
}

void PM::Poodle::GLObject::SetUniform(GLuint shd_hdl)
{
	GLint uniform_var_loc1 = glGetUniformLocation(shd_hdl, "uModel_to_NDC");
	if (uniform_var_loc1 >= 0) {
		glUniformMatrix3fv(uniform_var_loc1, 1, GL_FALSE, glm::value_ptr(Poodle::GLObject::mdl_to_ndc_xform));
	}
	else {
		std::cout << "uModel_to_NDC variable doesn't exist!!!\n";
	}
	GLint uniform_var_loc2 = glGetUniformLocation(shd_hdl, "uColor");
	if (uniform_var_loc2 >= 0) {
		glUniform4fv(uniform_var_loc2, 1, glm::value_ptr(Poodle::GLObject::color));
	}
	else {
		std::cout << "uColor variable doesn't exist!!!\n";
	}
}
void PM::Poodle::GLObject::holoUniform(GLuint shd_hdl)
{
	GLint uniform_var = glGetUniformLocation(shd_hdl, "u_time");
	if (uniform_var >= 0) {
		float time = (float)Engine::GetTime();
		glUniform1f(uniform_var,time);
	}
	else {
		std::cout << "u_time variable doesn't exist!!!\n";
	}

	GLint uniform_var2 = glGetUniformLocation(shd_hdl, "u_resolution");
	if (uniform_var2 >= 0) {
		glm::vec2 resolution = Engine::GetWindow().GetSize();
		glUniform2fv(uniform_var2, 1, glm::value_ptr(resolution));
	}
	else {
		std::cout << "u_resolution variable doesn't exist!!!\n";
	}
}

void PM::Poodle::GLObject::titleUniform(GLuint shd_hdl)
{
	GLint uniform_var = glGetUniformLocation(shd_hdl, "u_time");
	if (uniform_var >= 0) {
		float time = (float)Engine::GetTime();
		glUniform1f(uniform_var, time);
	}
	else {
		std::cout << "u_time variable doesn't exist!!!\n";
	}

	GLint uniform_var2 = glGetUniformLocation(shd_hdl, "u_resolution");
	if (uniform_var2 >= 0) {
		glm::vec2 resolution = Engine::GetWindow().GetSize();
		glUniform2fv(uniform_var2, 1, glm::value_ptr(resolution));
	}
	else {
		std::cout << "u_resolution variable doesn't exist!!!\n";
	}


	GLint uniform_var3 = glGetUniformLocation(shd_hdl, "u_skip");
	if (uniform_var3 >= 0) {
		int skip = static_cast<int>(Engine::GetTitleSkip());
		glUniform1i(uniform_var3, skip);
	}
	else {
		std::cout << "u_time variable doesn't exist!!!\n";
	}
}


void PM::Poodle::shape_init() {
	GLSLShader shdr_null;
	shd_ref["null"] = shdr_null;
	///////////////////////////////
	poly_s_init();
	//////////////////
	tex_s_init();
	holo_s_init();
	title_s_init();
	player_s_init();
	///////////////////////
	linesetup();
	trisetup();
	ellipsetup();
	texsetup();
}

void PM::Poodle::GLSetting::Clear() {
	LR = 0.f;
	LG = 0.f;
	LB = 0.f;
	LA = 1.f;
	FR = 0.f;
	FG = 0.f;
	FB = 0.f;
	FA = 1.f;
	LW = 1.f;
	noline = GL_FALSE;
	nofill = GL_FALSE;
	rectcenter = GL_FALSE;
	scalemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
	rotatemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
	translatemat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
	transformmat = { {1, 0, 0}, {0, 1, 0}, {0, 0, 1} };
	shader = "null";
}


void PM::Poodle::poly_s_init()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files1{
	std::make_pair(GL_VERTEX_SHADER, "assets/shaders/STM.vert"),
	std::make_pair(GL_FRAGMENT_SHADER, "assets/shaders/STM.frag")
	};
	GLSLShader shdr_pgm1;
	shdr_pgm1.CompileLinkValidate(shdr_files1);
	if (GL_FALSE == shdr_pgm1.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm1.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}
	shd_ref["polygon"] = shdr_pgm1;
}

void PM::Poodle::tex_s_init()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files2{
	std::make_pair(GL_VERTEX_SHADER, "assets/shaders/TEX.vert"),
	std::make_pair(GL_FRAGMENT_SHADER, "assets/shaders/TEX.frag")
	};
	GLSLShader shdr_pgm2;
	shdr_pgm2.CompileLinkValidate(shdr_files2);
	if (GL_FALSE == shdr_pgm2.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm2.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}
	shd_ref["tex"] = shdr_pgm2;
}


void PM::Poodle::holo_s_init()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files3{
	std::make_pair(GL_VERTEX_SHADER, "assets/shaders/STM.vert"),
	std::make_pair(GL_FRAGMENT_SHADER, "assets/shaders/Hologram.frag")
	};
	GLSLShader shdr_pgm3;
	shdr_pgm3.CompileLinkValidate(shdr_files3);
	if (GL_FALSE == shdr_pgm3.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm3.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}
	shd_ref["Hologram"] = shdr_pgm3;
}

void PM::Poodle::title_s_init()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files4{
std::make_pair(GL_VERTEX_SHADER, "assets/shaders/STM.vert"),
std::make_pair(GL_FRAGMENT_SHADER, "assets/shaders/Title.frag")
	};
	GLSLShader shdr_pgm4;
	shdr_pgm4.CompileLinkValidate(shdr_files4);
	if (GL_FALSE == shdr_pgm4.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm4.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}
	shd_ref["Title"] = shdr_pgm4;
}

void PM::Poodle::player_s_init()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files5{
std::make_pair(GL_VERTEX_SHADER, "assets/shaders/TEX_PLAYER.vert"),
std::make_pair(GL_FRAGMENT_SHADER, "assets/shaders/TEX_PLAYER.frag")
	};
	GLSLShader shdr_pgm5;
	shdr_pgm5.CompileLinkValidate(shdr_files5);
	if (GL_FALSE == shdr_pgm5.IsLinked()) {
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm5.GetLog() << "\n";
		std::exit(EXIT_FAILURE);
	}
	shd_ref["Player"] = shdr_pgm5;
}