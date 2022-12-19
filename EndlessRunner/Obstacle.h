#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "VertexBufferObject.h"
#include "VertexArrayObject.h"
#include "ElementBufferObject.h"
class Obstacle
{
	float m_maxX;
	float m_maxY;
	float m_minY = 50.f;
	glm::vec2 window;
	VertexArrayObject VAO;
	GLfloat vertices[24] = {
		-1.0f, -1.0f, 0.0f, 1.0f , 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f , 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f,1.0f , 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,1.0f , 1.0f, 1.0f,
	};
	GLuint indices[6] = {
	0, 1, 2,
	2, 1, 3
	};

public:
	float sizeX;
	float sizeY;

	float posX;
	float posY;
private:
	float Normalize(float input, float min, float max)
	{
		return (input - min) / (max - min);
	}

	float RandFloat(float max, float min)
	{
		return ((float)rand() / (RAND_MAX + 1)) * (max - min + 1) + min;
	}

public:
	Obstacle(float maxX, float maxY, float windowWidth, float windowHeight);
	void Draw(float posX, float posY, GLuint ShaderID);
	void Regenerate();
	void Delete();
};

