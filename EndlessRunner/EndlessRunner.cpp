#include <iostream>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderClass.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"
#include "TerrainGenerator.h"

int width = 800, height = 800;
float Normalize(float input, float min, float max)
{
	return (input - min) / (max - min);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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

	//Get Screen Resolution
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
	glViewport(0, 0, 800, 800);

	//Triangle Vertices
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f, 0.51f, 0.02f,0.03f,
		0.5f, -0.5f, 0.0f, 0.51f, 0.02f,0.03f,
		-0.5f, 0.5f, 0.0f, 0.51f, 0.02f,0.03f,
		0.5f, 0.5f, 0.0f, 0.51f, 0.02f,0.03f,
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 1, 3
	};


	Shader shaderProgram("./Shaders/Vertex.GLSL", "./Shaders/Fragment.GLSL");

	VertexArrayObject VAO1;
	VAO1.Bind();

	VertexBufferObject VBO1(vertices, sizeof(vertices));
	ElementBufferObject EBO1(indices, sizeof(indices));

	VAO1.LinkAttirb(VBO1, 0, 3, GL_FLOAT, sizeof(GLfloat) * 6, (void*)0);
	VAO1.LinkAttirb(VBO1, 1, 3, GL_FLOAT, sizeof(GLfloat) * 6, (void*)(3 * sizeof(GLfloat)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	float lastTime = 0, currentTime, deltaTime;
	float x = -200.0f, y = 0;
	float speedX = 0, speedY = 0;
	float gravity = -1.0f;
	float cubeScale = 0.25f;
	float squarePixelSize = cubeScale * 0.5f * height / 2;
	bool jumping = false;
	TerrainGenerator terrain(width, height);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		shaderProgram.Activate();

		TakeInput(window, x, y);

		terrain.Generate(shaderProgram.ID, deltaTime);

		y += speedY;
		speedY += gravity;

		if (y - squarePixelSize < -height / 2)
		{
			y = -height / 2 + squarePixelSize;
			speedY = 0;
			jumping = false;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !jumping)
		{
			speedY = 25.0f;
			jumping = true;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(Normalize(x, 0, (float)width / 2), Normalize(y, 0, (float)height / 2), 0.0f));
		model = glm::scale(model, glm::vec3(cubeScale, cubeScale, 1.0f));

		glm::mat4 MVP = projection * view * model;
		GLuint MVPLoc = glGetUniformLocation(shaderProgram.ID, "MVP");
		glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, glm::value_ptr(MVP));

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);


		//Take Care of all glfw Events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Window Closing Events
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
