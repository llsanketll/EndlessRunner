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

float width = 1080.0f, height = 800.0f;
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

	GLFWwindow* window = glfwCreateWindow((int)width, (int)height, "EndlessRunner", NULL, NULL);


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

	//Triangle Vertices
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.51f, 0.02f,0.03f,
		1.0f, -1.0f, 0.0f, 0.51f, 0.02f,0.03f,
		-1.0f, 1.0f, 0.0f, 0.51f, 0.02f,0.03f,
		1.0f, 1.0f, 0.0f, 0.51f, 0.02f,0.03f,
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
	float speedX = 0, speedY = 0;
	float gravity = -0.85f;
	float sizeX = 100.0f;
	float sizeY = 100.0f;
	float squareScaleX = sizeX / width;
	float squareScaleY = sizeY / height;
	float x = -width / 2 + sizeX;
	float y = 0;
	bool jumping = false;
	double mouseX = 0, mouseY = 0;
	TerrainGenerator terrain((float)width, (float)height);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		shaderProgram.Activate();

		TakeInput(window, x, y);
		glfwGetCursorPos(window, &mouseX, &mouseY);

		terrain.Generate(shaderProgram.ID, deltaTime);
		if (terrain.DetectCollision(x, y, sizeX, sizeY))
		{
			std::cout << "Collision" << std::endl;
		}


		y += speedY;
		speedY += gravity;
		//x = (float)mouseX - width / 2;
		//y = -1 * ((float)mouseY - height / 2);

		if (y - sizeY / 2 < -height / 2)
		{
			y = -height / 2 + sizeY / 2;
			speedY = 0;
			jumping = false;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !jumping)
		{
			speedY = 30.0f;
			jumping = true;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);


		model = glm::translate(model, glm::vec3(Normalize(x, 0, width / 2), Normalize(y, 0, height / 2), 0.0f));
		model = glm::scale(model, glm::vec3(squareScaleX, squareScaleY, 1.0f));



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
