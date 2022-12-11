#define GLM_FORCE_SILENT_WARNINGS
/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Font.cpp
Project: GAM200
Author: Dong-A Choi
-----------------------------------------------------------------*/
#include "Font.h"

#include <iostream>
#include <filesystem>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine.h"

PM::Font::Font(unsigned int width, unsigned int height) :proj_width(static_cast<float>(width)), proj_height(static_cast<float>(height))
{
    setup_shdrpgm();
    shdr_pgm.Use();

    //glm::mat4 proj = glm::ortho(0, Engine::GetWindow().GetSize().x, 0, Engine::GetWindow().GetSize().y);
    /*glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj));*/
    //GLuint apply = glGetUniformLocation(shdr_pgm.GetHandle(), "apply");
    GLuint text_location = glGetUniformLocation(shdr_pgm.GetHandle(), "text");
    glUniform1i(text_location, 0);
    shdr_pgm.UnUse();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void PM::Font::setup_shdrpgm()
{
    std::vector<std::pair<GLenum, std::string>> shdr_files;
    shdr_files.push_back(std::make_pair(GL_VERTEX_SHADER, "assets/shaders/FONT.vert"));
    shdr_files.push_back(std::make_pair(GL_FRAGMENT_SHADER, "assets/shaders/FONT.frag"));
    shdr_pgm.CompileLinkValidate(shdr_files);
    if (GL_FALSE == shdr_pgm.IsLinked()) {
        std::cout << "Unable to compile/link/validate shader programs" << "\n";
        std::cout << shdr_pgm.GetLog() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void PM::Font::Load(std::string font, unsigned int fontSize)
{
    this->Characters.clear();

    if (FT_Init_FreeType(&ft_lib)) {
        Engine::GetLogger().LogError("Can't init Font");
    }

    if (FT_New_Face(ft_lib, font.c_str(), 0, &ft_face)) {
        Engine::GetLogger().LogError("Can't Load Font");
    }

    FT_Set_Pixel_Sizes(ft_face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(ft_face, c, FT_LOAD_RENDER))
        {
            Engine::GetLogger().LogError("Can't Load GLyph");
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            ft_face->glyph->bitmap.width,
            ft_face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            ft_face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows),
            glm::ivec2(ft_face->glyph->bitmap_left, ft_face->glyph->bitmap_top),
            static_cast<unsigned int>(ft_face->glyph->advance.x)
        };
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(ft_face);
    FT_Done_FreeType(ft_lib);
}

void PM::Font::Draw(std::string text, float x, float y, float scale, bool matrix, AlignType align, glm::vec3 color)
{
    x = x / 2;
    y = y / 2;
    shdr_pgm.Use();

    GLuint texColor_location = glGetUniformLocation(shdr_pgm.GetHandle(), "textColor");
    glm::vec3 col = glm::vec3(color);
    glUniform3fv(texColor_location, 1, glm::value_ptr(col));
    this->shdr_pgm.SetUniform("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);


    std::string::const_iterator c;
    float total_advance = 0;
    Character character;
    for (c = text.begin(); c != text.end(); c++)
    {
        character = Characters[*c];
        total_advance += static_cast<float>(character.advance)/128.f*0.75f * scale;
    }
    if (align == AlignType::center) {

        x -= (total_advance / 2.f) ;
    }else if (align == AlignType::right) {

        x -= (total_advance);
    }

    for (c = text.begin(); c != text.end(); c++)
    {
        character = Characters[*c];
       
        //GLuint apply = glGetUniformLocation(shdr_pgm.GetHandle(), "apply");
        if (matrix == true) {
            //glUniform1i(apply, 0);
            GLuint proj_location = glGetUniformLocation(shdr_pgm.GetHandle(), "projection");

            glm::mat4 proj = glm::ortho(0.0f, proj_width, -proj_height, 0.f);
            glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj));

            glm::fvec2 winSize = Engine::GetWindow().GetSize();
            float camWidth = Engine::GetCamera().getCameraWidth();
            float camHeight = Engine::GetCamera().getCameraHeight();


            glm::mat3 rval{ {camWidth / winSize.x, 0.f, 0.f},
                {0.f, camHeight / winSize.y, 0.f},
                {-camWidth / 2, camHeight / 2, 1.f }
            };

            glm::mat3 displayToWorld = Engine::GetCamera().getCameraToWorld() * rval;

            glm::mat3 transform_matrix = { {1,0,0},{0,1,0},{x,y,1} };
            glm::mat3 final_matrix = inverse(displayToWorld) * transform_matrix;


            /*glm::mat3 transform_matrix = { {1,0,0},{0,1,0},{x,y,1} };
            glm::mat3 final_matrix = Engine::GetCamera().getScreenToWorld() * glm::inverse(Engine::GetCamera().getScreenToWorld()) * transform_matrix;*/

            GLuint tex_matrix = glGetUniformLocation(shdr_pgm.GetHandle(), "matrix");
            glUniformMatrix3fv(tex_matrix, 1, GL_FALSE, glm::value_ptr(final_matrix));
        }
        else if (matrix == false)
        {
            //glUniform1i(apply, 0);
            GLuint proj_location = glGetUniformLocation(shdr_pgm.GetHandle(), "projection");

            glm::mat4 proj = glm::ortho(0.0f, proj_width, -proj_height, 0.f);
            glUniformMatrix4fv(proj_location, 1, GL_FALSE, glm::value_ptr(proj));

            glm::fvec2 winSize = Engine::GetWindow().GetSize();
            float camWidth = Engine::GetCamera().getCameraWidth();
            float camHeight = Engine::GetCamera().getCameraHeight();

            glm::mat3 rval{ {camWidth / winSize.x, 0.f, 0.f},
                {0.f, camHeight / winSize.y, 0.f},
                {0.f, camHeight, 1.f }
            };

            glm::mat3 displayToWorld = Engine::GetCamera().getCameraToWorld() * rval;

            glm::mat3 transform_matrix = { {1,0,0},{0,1,0},{x,y - Engine::GetWindow().GetSize().y,1} };
            glm::mat3 final_matrix = transform_matrix;


            /*glm::mat3 transform_matrix = { {1,0,0},{0,1,0},{x,y,1} };
            glm::mat3 final_matrix = Engine::GetCamera().getScreenToWorld() * glm::inverse(Engine::GetCamera().getScreenToWorld()) * transform_matrix;*/

            GLuint tex_matrix = glGetUniformLocation(shdr_pgm.GetHandle(), "matrix");
            glUniformMatrix3fv(tex_matrix, 1, GL_FALSE, glm::value_ptr(final_matrix));

        }
   
        float xpos = x + character.offset.x * scale * 0.75f;
        float ypos = y - (character.size.y - character.offset.y ) * scale * 0.75f;

        float w = character.size.x * scale * 0.75f;
        float h = character.size.y * scale * 0.75f;

        float vertices[6][4] = {
            /*{ xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos,       0.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f }*/
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos, ypos,       0.0f, 1.0f },
            { xpos + w,     ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,   1.0f, 1.0f },
            { xpos + w, ypos + h,       1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, character.ID);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (character.advance >> 7) * scale * 0.75f;

    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    shdr_pgm.UnUse();

}

////gl_Position = projection * vec4(vVertexPosition.x,vVertexPosition.y, 0.0, 1.0);