#include <iostream>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "ShaderClass.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

#define PIXEL_SIZE 2  
int width = 800, height = 800;

float Normalize(float input, float min, float max)
{
	return (input - min) / (max - min);
}

void DrawPixel(int posX, int posY)
{
	GLfloat vertices[] = {
		Normalize(posX, 0, 400), Normalize(posY, 0, 400), 0
	};

	//Drawing The initial coordinate
	VertexArrayObject VAO;
	VAO.Bind();
	VertexBufferObject VBO(vertices, sizeof(vertices));
	VAO.LinkVBO(VBO, 0);
	VAO.Unbind();
	VBO.Unbind();

	VAO.Bind();
	glPointSize(PIXEL_SIZE);
	glDrawArrays(GL_POINTS, 0, 1);

	VAO.Unbind();
	VAO.Delete();
	VBO.Delete();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MidpointDrawCircle(int centerX, int centerY, int r)
{
	int x = 0;
	int y = r;
	float decision = 5 / 4 - r;
	DrawPixel(x + centerX, y + centerY);

	while (y > x)
	{
		if (decision < 0)
		{
			x++;
			decision += 2 * x + 1;
		}
		else
		{
			y--;
			x++;
			decision += 2 * (x - y) + 1;
		}
		DrawPixel(x + centerX, y + centerY);
		DrawPixel(x + centerX, -y + centerY);
		DrawPixel(-x + centerX, y + centerY);
		DrawPixel(-x + centerX, -y + centerY);
		DrawPixel(y + centerX, x + centerY);
		DrawPixel(-y + centerX, x + centerY);
		DrawPixel(y + centerX, -x + centerY);
		DrawPixel(-y + centerX, -x + centerY);
	}
}

void DisplayRotation(Shader& shaderProgram, VertexArrayObject& VAO)
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DisplayScale(Shader& shaderProgram, float scaleX, float scaleY, VertexArrayObject& VAO)
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(scaleX, scaleY, 1));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DisplayTranslation(Shader& shaderProgram, float& x, float& y, float& speedX, float& speedY, float deltaTime)
{
	x += speedX * deltaTime;
	y += speedY * deltaTime;
	if (x + 25 > 200 || x - 25 < -200) speedX *= -1;
	if (y + 25 > 200 || y - 25 < -200) speedY *= -1;

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(Normalize(x, 0, 400), Normalize(y, 0, 400), 0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	MidpointDrawCircle(x, y, 50);
}

void DisplayShear(Shader& shaderProgram, float x, VertexArrayObject& VAO)
{
	glm::mat4 trans = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		x, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	VAO.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void DisplayReflection(GLFWwindow* window, Shader& shaderProgram, VertexArrayObject VAO, float x, float y)
{
	glm::mat4 trans(1.0f);
	glm::mat4 ReflectX(
		1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	glm::mat4 ReflectY(
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		trans = ReflectX * trans;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		trans = ReflectY * trans;
	trans = glm::translate(trans, glm::vec3(Normalize(x, 0, 400), Normalize(y, 0, 400), 0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	VAO.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void TakeInput(GLFWwindow* window, float& x, float& y)
{
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		x += 1;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		x -= 1;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		y += 1;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		y -= 1;
}

int main()
{
	//Initialized the window
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Get Display Resolutions
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* screen = glfwGetVideoMode(primaryMonitor);
	std::cout << screen->width << ", " << screen->height << std::endl;

	GLFWwindow* window = glfwCreateWindow(width, height, "EndlessRunner", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "failed  to create" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OPENGL
	gladLoadGL();
	glViewport(0, 0, width, height);

	GLfloat vertices[] = {
		-0.25f, -0.25f, 0.0f,
		0.25f, -0.25f, 0.0f,
		-0.25f, 0.25f, 0.0f,
		0.25f, 0.25f, 0.0f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 1, 3
	};

	Shader shaderProgram("./Shaders/Vertex.GLSL", "./Shaders/Fragment.GLSL");

	VertexArrayObject VAO;
	VAO.Bind();

	VertexBufferObject VBO(vertices, sizeof(vertices));
	ElementBufferObject EBO(indices, sizeof(indices));

	VAO.LinkVBO(VBO, 0);
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();



	double mouseX = 0, mouseY = 0;
	float lastTime = 0, currentTime, deltaTime;
	float x = 150, y = 150;
	float speedX = 100, speedY = 50;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();

		currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;


		//DisplayTranslation(shaderProgram, x, y, speedX, speedY, deltaTime);

		//DisplayRotation(shaderProgram, VAO);

		TakeInput(window, x, y);
		//DisplayScale(shaderProgram, Normalize(x, 1, 400), Normalize(y, 1, 400), VAO);
		//DisplayShear(shaderProgram, Normalize(x, 0, 400), VAO);


		DisplayReflection(window, shaderProgram, VAO, x, y);


		//Take Care of all glfw Events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	VAO.Delete();
	VBO.Delete();
	EBO.Delete();

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
