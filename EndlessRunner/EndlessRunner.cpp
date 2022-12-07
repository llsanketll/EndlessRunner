#include <iostream>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>
#include "ShaderClass.h"
#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ElementBufferObject.h"

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
	glPointSize(2);
	glDrawArrays(GL_POINTS, 0, 1);

	VAO.Unbind();
	VAO.Delete();
	VBO.Delete();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void BLDDisplayLine(float x1, float y1, float x2, float y2)
{
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = 1;
	if (x2 < x1) incx = -1;
	incy = 1;
	if (y2 < y1) incy = -1;
	x = x1; y = y1;
	if (dx > dy)
	{
		DrawPixel(x, y);
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx; i++)
		{
			if (e >= 0)
			{
				y += incy;
				e += inc1;
			}
			else
				e += inc2;
			x += incx;
			DrawPixel(x, y);
		}
	}
	else {
		DrawPixel(x, y);
		e = 2 * dx - dy;
		inc1 = 2 * (dx - dy);
		inc2 = 2 * dx;
		for (i = 0; i < dy; i++)
		{
			if (e >= 0)
			{
				x += incx;
				e += inc1;
			}
			else
				e += inc2;
			y += incy;
			DrawPixel(x, y);
		}
	}

}

void DDADisplayLine(float x1, float y1, float x2, float y2)
{
	float dy, dx, step, x, y, k, Xin, Yin;
	dx = x2 - x1;
	dy = y2 - y1;
	if (abs(dx) > abs(dy))
		step = abs(dx);
	else
		step = abs(dy);
	Xin = dx / step;
	Yin = dy / step;
	x = x1;
	y = y1;

	DrawPixel(x, y);


	for (k = 1; k <= step; k++)
	{
		x = x + Xin;
		y = y + Yin;
		DrawPixel(x, y);
	}
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

	GLFWwindow* window = glfwCreateWindow(800, 800, "EndlessRunner", NULL, NULL);
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

	Shader shaderProgram("./Shaders/Vertex.GLSL", "./Shaders/Fragment.GLSL");

	glPointSize(50);

	double mouseX = 0, mouseY = 0;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();

		glfwGetCursorPos(window, &mouseX, &mouseY);
		//DDADisplayLine(0, 0, mouseX - 400, -1 * (mouseY - 400));
		BLDDisplayLine(0, 0, mouseX - 400, -1 * (mouseY - 400));
		glfwSwapBuffers(window);
		//Take Care of all glfw Events
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
