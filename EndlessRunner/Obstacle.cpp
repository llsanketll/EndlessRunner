#include "Obstacle.h"


Obstacle::Obstacle(float maxX, float maxY, float windowWidth, float windowHeight)
{
	m_maxX = maxX;
	m_maxY = maxY;
	sizeX = maxX;
	sizeY = maxY;
	sizeX = RandFloat(maxX, 10.0f);
	sizeY = RandFloat(maxY, 10.0f);
	window.x = windowWidth;
	window.y = windowHeight;

	VAO.Bind();

	VertexBufferObject VBO1(vertices, sizeof(vertices));
	ElementBufferObject EBO1(indices, sizeof(indices));

	VAO.LinkAttirb(VBO1, 0, 3, GL_FLOAT, sizeof(GLfloat) * 6, (void*)0);
	VAO.LinkAttirb(VBO1, 1, 3, GL_FLOAT, sizeof(GLfloat) * 6, (void*)(3 * sizeof(GLfloat)));
	VAO.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	VBO1.Delete();
	EBO1.Delete();

}

void Obstacle::Draw(float posX, float posY, GLuint shaderID)
{

	this->posX = posX;
	this->posY = posY;

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);

	float squareScaleX = sizeX / (window.x);
	float squareScaleY = sizeY / (window.y);

	model = glm::translate(model, glm::vec3(Normalize(posX, 0, window.x / 2), Normalize(posY, 0, window.y / 2), 0.0f));
	model = glm::scale(model, glm::vec3(squareScaleX, squareScaleY, 1.0f));

	glm::mat4 MVP = projection * view * model;
	GLuint MVPLoc = glGetUniformLocation(shaderID, "MVP");
	glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
	VAO.Unbind();
}

void Obstacle::Regenerate()
{
	this->sizeX = RandFloat(m_maxX, m_minY);
	this->sizeY = RandFloat(m_maxY, m_minY);
}

void Obstacle::Delete()
{
}
