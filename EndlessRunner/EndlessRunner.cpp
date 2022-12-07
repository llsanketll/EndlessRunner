#include <iostream>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "ShaderClass.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	//Initialized the window
	glfwInit();
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* screen = glfwGetVideoMode(primaryMonitor);
	std::cout << screen->width << ", " << screen->height << std::endl;
	GLFWwindow* window = glfwCreateWindow(screen->width / 2, screen->height, "EndlessRunner", NULL, NULL);


	if (window == NULL)
	{
		std::cout << "failed  to create" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window)
		;

	//Load GLAD so it configures OPENGL
	gladLoadGL();
	glViewport(0, 0, 800, 800);

	//Triangle Vertices
	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f,
		 0.5f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f,
		 0.0f, 0.45f, 0.0f,
		 0.45f,  0.45f, 0.0f,
		 0.0f, 1.0f, 0.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		3, 4, 5
	};


	Shader shaderProgram("./Shaders/Vertex.GLSL", "./Shaders/Fragment.GLSL");

	VertexArrayObject VAO1;
	VAO1.Bind();

	VertexBufferObject VBO1(vertices, sizeof(vertices));
	ElementBufferObject EBO1(indices, sizeof(indices));

	VAO1.LinkVBO(VBO1, 0);
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 7, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		//Take Care of all glfw Events
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
