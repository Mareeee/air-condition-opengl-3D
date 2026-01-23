#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    unsigned int TextureID;
    int SizeX, SizeY;
    int BearingX, BearingY;
    unsigned int Advance;
};

extern std::map<char, Character> Characters;
extern unsigned int textVAO, textVBO;

void createOrthographicMatrix(float left, float right, float bottom, float top,
    float near, float far, float* mat);

void initTextRendering(const char* fontPath, GLuint fontSize, unsigned int textShader);

void RenderText(unsigned int shader, std::string text, float x, float y, float scale,
    float r, float g, float b, float alpha,
    int screenWidth, int screenHeight);
