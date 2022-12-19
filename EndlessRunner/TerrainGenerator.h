#pragma once
#include "Obstacle.h"
#include <iostream>
class TerrainGenerator
{
private:
	float m_width;
	float m_height;

	float xPos1;
	float xSpeed;
	Obstacle* ob1;
public:
	TerrainGenerator(float width, float height);
	void Generate(GLuint shaderID, float deltaTime);
	bool DetectCollision(float objX, float objY, float sizeX, float sizeY);
	void setSpeed(float speed);
};

