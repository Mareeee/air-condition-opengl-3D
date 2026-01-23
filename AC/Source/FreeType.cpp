#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include "../Header/util.h"
#include "../Header/FreeType.h"

std::map<char, Character> Characters;
unsigned int textVAO, textVBO;

void createOrthographicMatrix(float left, float right, float bottom, float top, float near, float far, float* mat) {
    mat[0] = 2.0f / (right - left); mat[4] = 0.0f; mat[8] = 0.0f; mat[12] = -(right + left) / (right - left);
    mat[1] = 0.0f; mat[5] = 2.0f / (top - bottom); mat[9] = 0.0f; mat[13] = -(top + bottom) / (top - bottom);
    mat[2] = 0.0f; mat[6] = 0.0f; mat[10] = -2.0f / (far - near); mat[14] = -(far + near) / (far - near);
    mat[3] = 0.0f; mat[7] = 0.0f; mat[11] = 0.0f; mat[15] = 1.0f;
}

void initTextRendering(const char* fontPath, GLuint fontSize, unsigned int textShader) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        endProgram("ERROR::FREETYPE: Could not init FreeType Library");

    FT_Face face;
    if (FT_New_Face(ft, fontPath, 0, &face))
        endProgram("ERROR::FREETYPE: Failed to load font");

    FT_Set_Pixel_Sizes(face, 0, fontSize);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Characters[c] = {
            texture,
            (int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows,
            (int)face->glyph->bitmap_left, (int)face->glyph->bitmap_top,
            (unsigned int)face->glyph->advance.x
        };
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void RenderText(unsigned int shader, std::string text, float x, float y, float scale, float r, float g, float b, float alpha, int screenWidth, int screenHeight) {
    glUseProgram(shader);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    float projection[16];
    createOrthographicMatrix(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, -1.0f, 1.0f, projection);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, projection);

    glUniform3f(glGetUniformLocation(shader, "textColor"), r, g, b);
    glUniform1f(glGetUniformLocation(shader, "alpha"), alpha);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.BearingX * scale;
        float ypos = y - (ch.SizeY - ch.BearingY) * scale;

        float w = ch.SizeX * scale;
        float h = ch.SizeY * scale;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
