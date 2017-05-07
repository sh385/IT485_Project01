#include "user_interface.h"

UICanvas* createUI()
{
	UICanvas* canvas = (UICanvas*)malloc(sizeof(UICanvas));
	canvas->indices = (GLuint*)malloc(sizeof(GLuint) * 6);
	canvas->numElements = 0;
	canvas->numVertices = 0;
	canvas->numButtons = 0;
	canvas->indices[0] = 0;
	canvas->indices[1] = 1;
	canvas->indices[2] = 2;
	canvas->indices[3] = 2;
	canvas->indices[4] = 1;
	canvas->indices[5] = 3;

	glGenVertexArrays(1, &canvas->vao);
	glGenBuffers(1, &canvas->vbo);
	glGenBuffers(1, &canvas->ebo);
	return canvas;
}

UIElement* createUIElement(GLuint width, GLuint height, glm::vec2 position, bool isButton)
{
	UIElement* uiElement = (UIElement*)malloc(sizeof(UIElement));
	uiElement->isButton = isButton;
	uiElement->width = width;
	uiElement->height = height;
	uiElement->position = position;
	return uiElement;
}

void setText(UIElement* uiElement, char* text)
{
	uiElement->text = text;
}

void setTextColor(UIElement* uiElement, glm::vec4 color)
{
	uiElement->textColor = color;
}

void addUIElement(UICanvas* canvas, UIElement* uiElement)
{
	if (uiElement->isButton)
	{
		canvas->numButtons++;
	}
	canvas->numElements++;
	canvas->numVertices += 4;
	if (canvas->numElements == 1)
	{
		canvas->uiElements = (UIElement*)malloc(sizeof(UIElement)*canvas->numElements);
		canvas->vertices = (UIVertex*)malloc(sizeof(UIVertex) * canvas->numVertices);
	}
	else
	{
		canvas->uiElements = (UIElement*)realloc(canvas->uiElements, sizeof(UIElement)*canvas->numElements);
		canvas->vertices = (UIVertex*)realloc(canvas->vertices, sizeof(UIVertex) * canvas->numVertices);
	}
	canvas->uiElements[canvas->numElements - 1] = *uiElement;

	canvas->vertices[canvas->numVertices - 4].position.x = uiElement->position.x;
	canvas->vertices[canvas->numVertices - 4].position.y = uiElement->position.y;
	canvas->vertices[canvas->numVertices - 3].position.x = (GLfloat)uiElement->position.x + uiElement->width;
	canvas->vertices[canvas->numVertices - 3].position.y = (GLfloat)uiElement->position.y;
	canvas->vertices[canvas->numVertices - 2].position.x = (GLfloat)uiElement->position.x;
	canvas->vertices[canvas->numVertices - 2].position.y = (GLfloat)uiElement->position.y + uiElement->height;
	canvas->vertices[canvas->numVertices - 1].position.x = (GLfloat)uiElement->position.x + uiElement->width;
	canvas->vertices[canvas->numVertices - 1].position.y = (GLfloat)uiElement->position.y + uiElement->height;
}

void initUI(UICanvas* canvas)
{

}

void updateUI(UICanvas* canvas)
{
	Uint8 mousePositions = SDL_GetMouseState(&canvas->mouseX, &canvas->mouseY);

	if (canvas->numButtons > 0)
	{
		for (GLuint i = 0; i < canvas->numElements; i++)
		{
			if (canvas->uiElements[i].isButton && contains(canvas->uiElements[i], glm::vec2(canvas->mouseX, canvas->mouseY)))
			{

				setTextColor(&canvas->uiElements[i], glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
				canvas->uiElements[i].isHighlighted = true;
			}
			else
			{
				setTextColor(&canvas->uiElements[i], glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
				canvas->uiElements[i].isHighlighted = false;
			}
		}
	}
}

bool contains(UIElement uiElement, glm::vec2 position)
{
	if (position.x >= uiElement.position.x && position.x <= uiElement.position.x + uiElement.width
		&& (600 - position.y) >= uiElement.position.y && (600 - position.y) <= uiElement.position.y + uiElement.height)
	{
		return true;
	}
	return false;
}

void drawUI(UICanvas* canvas)
{
	glBindVertexArray(canvas->vao);
	glBindBuffer(GL_ARRAY_BUFFER, canvas->vbo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, canvas->ebo);
	glEnableVertexAttribArray(0);

	for (GLuint i = 0; i < canvas->numElements; i++)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(UIVertex) * 4, &canvas->vertices[i * 4], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 6, &canvas->indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(UIVertex), 0);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//if (canvas->uiElements[i].text)
		//{
		glColor4f(canvas->uiElements[i].textColor.x, canvas->uiElements[i].textColor.y, canvas->uiElements[i].textColor.z, canvas->uiElements[i].textColor.w);
		glWindowPos2f(canvas->uiElements[i].position.x, canvas->uiElements[i].position.y);
		glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char*)canvas->uiElements[i].text);
		//}
	}
	glBindVertexArray(0);
}