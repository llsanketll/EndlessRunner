#pragma once
#include "Obstacle.h"
class TerrainGenerator
{
private:
	float m_width;
	float m_height;

	float xPos;
	float xSpeed;
	Obstacle* o1;
public:
	TerrainGenerator(float width, float height);
	void Generate(GLuint shaderID, float deltaTime);
};

