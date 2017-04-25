#pragma once
#include <stdio.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>

typedef struct
{
	glm::vec2 position;
}UIVertex;

typedef struct
{
	UIVertex* vertices;
	GLuint* indices;
	GLuint numVertices;
	GLuint numIndices;
	glm::vec2 position;
	GLuint width;
	GLuint height;
	GLuint vao, vbo, ebo;
	char* text;
	glm::vec4 textColor;
}Icon;

typedef struct
{
	Icon* icons;
	GLuint numIcons;
}HUD;

HUD* createHUD();
void initHUD(HUD* hud);
void setText(Icon* icon, char* text);
void setTextColor(Icon* icon, glm::vec4 color);
void updateHUD(HUD* hud);
void drawHUD(HUD* hud);
void addHUDIcon(HUD* hud, Icon* icon);
void drawIcon(Icon* icon);
Icon* createIcon(GLuint width, GLuint height, glm::vec2 position);