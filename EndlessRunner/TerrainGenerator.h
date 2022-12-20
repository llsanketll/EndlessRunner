#pragma once
#include "Obstacle.h"
#include <iostream>
class TerrainGenerator
{
private:
	float m_width;
	float m_height;
	int _score;
	float xPos1;
	float xSpeed;
	Obstacle* ob1;
public:
	TerrainGenerator(float width, float height);
	void Generate(GLuint shaderID, float deltaTime, int* score);
	bool DetectCollision(float objX, float objY, float sizeX, float sizeY);
	void SetSpeed(float speed);
	void ResetGame();
};
