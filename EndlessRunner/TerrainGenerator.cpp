#include "TerrainGenerator.h"

TerrainGenerator::TerrainGenerator(float width, float height)
{
	m_width = width;
	m_height = height;
	xPos = width + 100.0f;
	xSpeed = -10.f;
	o1 = new Obstacle(200.0f, 250.f, m_width, m_height);
}

void TerrainGenerator::Generate(GLuint shaderID, float deltaTime)
{
	xPos += xSpeed;
	if (xSpeed < 100.0f)
		xSpeed += -deltaTime * 0.05f;
	if (xPos + o1->sizeY < -m_height)
	{
		xPos = m_width + o1->sizeX;
		o1->Regenerate();
	}
	if (o1 != NULL)
		o1->Draw(xPos, -m_height + o1->sizeY / 2, shaderID);
}
