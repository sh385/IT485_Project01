#pragma once
#include <stdio.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

typedef struct
{
	glm::vec2 position;
}UIVertex;

typedef struct
{
	glm::vec2 position;
	GLuint width;
	GLuint height;
	char* text;
	glm::vec4 textColor;
	bool isButton;
	bool isHighlighted;
}UIElement;

typedef struct
{
	UIVertex* vertices;
	GLuint* indices;
	GLuint numVertices;
	GLuint vao, vbo, ebo;
	UIElement* uiElements;
	GLuint numElements;
	GLuint numButtons;
	GLint mouseX;
	GLint mouseY;
}UICanvas;

UICanvas* createUI();
bool contains(UIElement uiElement, glm::vec2 position);
void initUI(UICanvas* canvas);
void setText(UIElement* uiElement, char* text);
void setTextColor(UIElement* uiElement, glm::vec4 color);
void updateUI(UICanvas* canvas);
void drawUI(UICanvas* canvas);
void addUIElement(UICanvas* canvas, UIElement* uiElement);
void drawUI(UICanvas* hud);
UIElement* createUIElement(GLuint width, GLuint height, glm::vec2 position, bool isButton);