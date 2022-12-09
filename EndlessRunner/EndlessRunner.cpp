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
		Normalize(posX, 0, width / 2), Normalize(posY, 0, height / 2), 0
	};

	//Drawing The initial coordinate
	VertexArrayObject VAO;
	VAO.Bind();
	VertexBufferObject VBO(vertices, sizeof(vertices));
	VAO.LinkAttirb(VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
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
	trans = glm::rotate(trans, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void DisplayScale(Shader& shaderProgram, float scaleX, float scaleY, VertexArrayObject& VAO)
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::scale(trans, glm::vec3(scaleX, scaleY, scaleX));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void DisplayShear(Shader& shaderProgram, float x, VertexArrayObject& VAO)
{
	glm::mat4 trans = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		x, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "model");
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
	trans = glm::translate(trans, glm::vec3(Normalize(x, 0, width / 2), Normalize(y, 0, height / 2), 0));
	GLuint transformLoc = glGetUniformLocation(shaderProgram.ID, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	VAO.Bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	VAO.Unbind();
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
		// front
	   -1.0, -1.0,  1.0, 1.0f, 0.0f, 0.0f,
		1.0, -1.0,  1.0,  1.0f, 0.0f, 0.0f,
		1.0,  1.0,  1.0, 1.0f, 0.0f, 0.0f,
	   -1.0,  1.0,  1.0, 1.0f, 0.0f, 0.0f,
	   // back
	   -1.0, -1.0, -1.0, 0.0f, 0.0f, 1.0f ,
		1.0, -1.0, -1.0,  0.0f, 0.0f, 1.0f,
		1.0,  1.0, -1.0,  0.0f, 0.0f, 1.0f,
	   -1.0,  1.0, -1.0,  0.0f, 0.0f, 1.0f
	};

	GLuint indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	Shader shaderProgram("./Shaders/Vertex.GLSL", "./Shaders/Fragment.GLSL");

	VertexArrayObject VAO;
	VAO.Bind();

	VertexBufferObject VBO(vertices, sizeof(vertices));
	ElementBufferObject EBO(indices, sizeof(indices));

	VAO.LinkAttirb(VBO, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO.LinkAttirb(VBO, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();


	double mouseX = 0, mouseY = 0;
	float lastTime = 0, currentTime, deltaTime;
	float x = 1, y = 1;
	float speedX = 100, speedY = 50;

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;




		TakeInput(window, x, y);



		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -10.0f));
		projection = glm::perspective(glm::radians(45.0f), (float)(height / width), 0.1f, 100.0f);
		model = glm::translate(model, glm::vec3(Normalize(x * 10, 0, 400), Normalize(y * 10, 0, 400), 0.0f));
		float scaleFactor = Normalize(abs(sin(glfwGetTime()) * 200) + 100, 0, 400);
		model = glm::scale(model, glm::vec3(scaleFactor));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLuint projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//int size;
		VAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

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
