#include "user_interface.h"

HUD* createHUD()
{
	HUD* hud = (HUD*)malloc(sizeof(HUD));
	hud->numIcons = 0;
	return hud;
}

Icon* createIcon(GLuint width, GLuint height, glm::vec2 position)
{
	Icon* icon = (Icon*)malloc(sizeof(Icon));
	icon->width = width;
	icon->height = height;
	icon->position = position;
	icon->vertices = (UIVertex*)malloc(sizeof(UIVertex)*4);
	icon->vertices[0].position.x = icon->position.x;
	icon->vertices[0].position.y = icon->position.y;
	icon->vertices[1].position.x = (GLfloat)icon->position.x + width;
	icon->vertices[1].position.y = (GLfloat)icon->position.y;
	icon->vertices[2].position.x = (GLfloat)icon->position.x;
	icon->vertices[2].position.y = (GLfloat)icon->position.y + height;
	icon->vertices[3].position.x = (GLfloat)icon->position.x + width;
	icon->vertices[3].position.y = (GLfloat)icon->position.y + height;
	icon->numIndices = 6;
	icon->indices = (GLuint*)malloc(sizeof(GLuint) * icon->numIndices);
	icon->numVertices = 4;
	icon->indices[0] = 0;
	icon->indices[1] = 1;
	icon->indices[2] = 2;
	icon->indices[3] = 2;
	icon->indices[4] = 1;
	icon->indices[5] = 3;

	glGenVertexArrays(1, &icon->vao);
	glGenBuffers(1, &icon->vbo);
	glGenBuffers(1, &icon->ebo);
	return icon;
}

void setText(Icon* icon, char* text)
{
	icon->text = text;
}

void setTextColor(Icon* icon, glm::vec4 color)
{
	icon->textColor = color;
}

void addHUDIcon(HUD* hud, Icon* icon)
{
	hud->numIcons++;
	if (hud->numIcons == 1)
	{
		hud->icons = (Icon*)malloc(sizeof(Icon)*hud->numIcons);
	}
	else
	{
		hud->icons = (Icon*)realloc(hud->icons, sizeof(Icon)*hud->numIcons);
	}
	hud->icons[hud->numIcons - 1] = *icon;
}

void initHUD(HUD* hud)
{
	Icon* icon = createIcon(200, 20, glm::vec2(10.0f, 10.0f));
	setText(icon, "Blah");
	setTextColor(icon, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	addHUDIcon(hud, icon);
}

void updateHUD(HUD* hud)
{
}

void drawHUD(HUD* hud)
{
	for (GLuint i = 0; i < hud->numIcons; i++)
	{
		drawIcon(&hud->icons[i]);
	}
}

void drawIcon(Icon* icon)
{
	glBindVertexArray(icon->vao);
	glBindBuffer(GL_ARRAY_BUFFER, icon->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UIVertex) * 4, &icon->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, icon->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*icon->numIndices, &icon->indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), (GLvoid*)offsetof(UIVertex, position));

	glDrawElements(GL_TRIANGLES, icon->numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	if (icon->text)
	{
		glColor4f(icon->textColor.x, icon->textColor.y, icon->textColor.z, icon->textColor.w);
		glWindowPos2f(icon->position.x, icon->position.y);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)icon->text);
	}
}