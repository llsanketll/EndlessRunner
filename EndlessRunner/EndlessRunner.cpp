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

#define PIXEL_SIZE 2  

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

void BLDDisplayLine(float x1, float y1, float x2, float y2)
{
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	incx = PIXEL_SIZE;
	if (x2 < x1) incx = -PIXEL_SIZE;
	incy = PIXEL_SIZE;
	if (y2 < y1) incy = -PIXEL_SIZE;
	x = x1; y = y1;
	if (dx > dy)
	{
		DrawPixel(x, y);
		e = 2 * dy - dx;
		inc1 = 2 * (dy - dx);
		inc2 = 2 * dy;
		for (i = 0; i < dx / PIXEL_SIZE; i++)
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
		for (i = 0; i < dy / PIXEL_SIZE; i++)
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
		step = abs(dx) / PIXEL_SIZE;
	else
		step = abs(dy) / PIXEL_SIZE;
	Xin = (dx) / step;
	Yin = dy / step;
	x = x1;
	y = y1;

	DrawPixel(x, y);


	for (k = 1; k <= step; k++)
	{
		x += Xin;
		y += Yin;
		DrawPixel(x, y);
	}
}

void MidpointDrawCircle(int r)
{
	int x = 0;
	int y = r;
	float decision = 5 / 4 - r;
	DrawPixel(x, y);

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
		DrawPixel(x, y);
		DrawPixel(x, -y);
		DrawPixel(-x, y);
		DrawPixel(-x, -y);
		DrawPixel(y, x);
		DrawPixel(-y, x);
		DrawPixel(y, -x);
		DrawPixel(-y, -x);
	}

}

void MidPointDrawEllipse(int xCenter, int yCenter, int rx, int ry)
{
	float x = 0;
	float y = ry;//(0,ry) ---
	float p1 = ry * ry - (rx * rx) * ry + (rx * rx) * (0.25);
	//slope
	float dx = 2 * (ry * ry) * x;
	float dy = 2 * (rx * rx) * y;
	while (dx < dy)
	{
		//plot (x,y)
		DrawPixel(xCenter + x, yCenter + y);
		DrawPixel(xCenter - x, yCenter + y);
		DrawPixel(xCenter + x, yCenter - y);
		DrawPixel(xCenter - x, yCenter - y);
		if (p1 < 0)
		{
			x = x + 1;
			dx = 2 * (ry * ry) * x;
			p1 = p1 + dx + (ry * ry);
		}
		else
		{
			x = x + 1;
			y = y - 1;
			dx = 2 * (ry * ry) * x;
			dy = 2 * (rx * rx) * y;
			p1 = p1 + dx - dy + (ry * ry);
		}
	}
	//ploting for 2nd region of 1st quardant and the slope will be > -1
	 //----------------------Region-2------------------------//
	float p2 = (ry * ry) * (x + 0.5) * (x + 0.5) + (rx * rx) * (y - 1) * (y - 1) - (rx * rx) * (ry * ry);


	while (y > 0)
	{
		//plot (x,y)
		DrawPixel(xCenter + x, yCenter + y);
		DrawPixel(xCenter - x, yCenter + y);
		DrawPixel(xCenter + x, yCenter - y);
		DrawPixel(xCenter - x, yCenter - y);     //glEnd();
		if (p2 > 0)
		{
			x = x;
			y = y - 1;
			dy = 2 * (rx * rx) * y;
			//dy = 2 * rx * rx *y;
			p2 = p2 - dy + (rx * rx);
		}
		else
		{
			x = x + 1;
			y = y - 1;
			dy = dy - 2 * (rx * rx);
			dx = dx + 2 * (ry * ry);
			p2 = p2 + dx -
				dy + (rx * rx);
		}
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

	double mouseX = 0, mouseY = 0;

	int radiusX = 10, radiusY = 20;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();

		glfwGetCursorPos(window, &mouseX, &mouseY);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			radiusX += 1;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			radiusX -= 1;

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			radiusY += 1;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			radiusY -= 1;
		//MidpointDrawCircle(radius);
		MidPointDrawEllipse(0, 0, radiusX, radiusY);

		glfwSwapBuffers(window);
		//Take Care of all glfw Events
		glfwPollEvents();
	}

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}
