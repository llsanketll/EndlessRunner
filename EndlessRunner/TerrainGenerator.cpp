#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(float width, float height)
{
	m_width = width;
	m_height = height;
	xPos1 = width + width * 0.5f;
	xSpeed = -5.f;
	ob1 = new Obstacle(200.0f, 250.f, m_width, m_height);
}

void TerrainGenerator::Generate(GLuint shaderID, float deltaTime)
{
	if (xPos1 < -m_width / 2 - ob1->sizeX / 2)
	{
		xPos1 = m_width / 2 + ob1->sizeX / 2;
		ob1->Regenerate();
	}

	if (ob1 != NULL)
	{
		ob1->Draw(xPos1, -m_height / 2 + ob1->sizeY / 2, shaderID);
	}

	xPos1 += xSpeed;

	if (xSpeed > -20.0f)
		xSpeed -= deltaTime * 0.5f;
}

bool TerrainGenerator::DetectCollision(float objX, float objY, float sizeX, float sizeY)
{
	float tolerance = 0.01f;

	float playerTop = objY + sizeY / 2;
	float playerBottom = objY - sizeY / 2;
	float playerLeft = objX - sizeX / 2;
	float playerRight = objX + sizeX / 2;

	float obstacleTop = ob1->posY + ob1->sizeY / 2;
	float obstacleRight = ob1->posX + ob1->sizeX / 2;
	float obstacleLeft = ob1->posX - ob1->sizeX / 2;
	float obstacleBottom = ob1->posY - ob1->sizeY / 2;

	if (playerLeft < obstacleRight &&
		playerRight > obstacleLeft &&
		playerTop > obstacleBottom &&
		playerBottom < obstacleTop)
		return true;
	return false;
}

void TerrainGenerator::setSpeed(float speed)
{
	xSpeed = speed;
}
