#if defined(_MSC_VER) && (_MSC_VER >= 1310)
#	pragma warning(disable: 4996) // Disable the fopen, strcpy, sprintf being unsafe warning
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#define GLT_IMPLEMENTATION
#include"gltext.h"

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

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


void DisplayTextGameOver(int score) {

	char str[100];
	GLTtext* text = gltCreateText();
	//GLTtext* textReload = gltCreateText();
	//gltSetText(textReload, "Press R to reload");

	gltBeginDraw();
	gltColor(1.0f, 0.0f, 0.0f, 1.0f);
	sprintf(str, "Game Over! %d \n\nPress R to reset", score);
	gltSetText(text, str);
	gltDrawText2DAligned(text, (GLfloat)(width / 2), (GLfloat)(height / 2), 3.0f, GLT_CENTER, GLT_CENTER);

	//gltDrawText2DAligned(textReload, (GLfloat)(width / 2 + 10.0f), (GLfloat)(height / 2 + 10.0f), 3.0f, GLT_CENTER, GLT_CENTER);

	//gltDeleteText(text);
	//gltDeleteText(textReload);
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
		std::cout << "failed  to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OPENGL
	gladLoadGL();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);

	//Triangle Vertices
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,0.85f,0.0f,0.39f,
		1.0f, -1.0f, 0.0f,0.85f,0.0f,0.39f,
		-1.0f, 1.0f, 0.0f,0.85f,0.0f,0.39f,
		1.0f, 1.0f, 0.0f,0.85f,0.0f,0.39f,
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
	float gravity = -8000.f;
	float sizeX = 80.0f;
	float sizeY = 80.0f;
	float squareScaleX = sizeX / width;
	float squareScaleY = sizeY / height;
	float x = -width / 2 + sizeX;
	float y = 0;
	bool jumping = false;
	double mouseX = 0, mouseY = 0;
	int score = 0;
	bool gameOver = false;
	TerrainGenerator terrain((float)width, (float)height);

	// GLT initialistion for TEXT RENDERING
	if (!gltInit())
	{
		fprintf(stderr, "Failed to initialize glText\n");
		glfwTerminate();
		return EXIT_FAILURE;
	}

	char str[20];
	GLTtext* text1 = gltCreateText();
	double time = glfwGetTime();

	glEnable(GL_BLEND);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// GLT shader begin
		gltBeginDraw();
		gltColor(abs(cosf((float)glfwGetTime() * 0.5f)), abs(sinf((float)glfwGetTime())), 0.8f, 1.0f);
		sprintf(str, "Score %d", score);
		gltSetText(text1, str);
		gltDrawText2DAligned(text1, (GLfloat)width, 0, 2.0f, GLT_RIGHT, GLT_TOP);



		currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		shaderProgram.Activate();


		TakeInput(window, x, y);
		glfwGetCursorPos(window, &mouseX, &mouseY);

		terrain.Generate(shaderProgram.ID, deltaTime, &score);

		y += speedY * deltaTime;
		speedY += gravity * deltaTime;


		if (y - sizeY / 2 < -height / 2)
		{
			y = -height / 2 + sizeY / 2;
			speedY = 0;
			jumping = false;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !jumping)
		{
			speedY = 2800.0f;
			jumping = true;
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && jumping)
		{
			gravity = -50000.0f;;
		}
		else
		{
			gravity = -8000.f;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && gameOver)
		{
			speedY = 30.0f;
			jumping = true;
			terrain.ResetGame();
			score = 0;
			gameOver = false;
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

		if (terrain.DetectCollision(x, y, sizeX, sizeY))
		{
			DisplayTextGameOver(score);
			terrain.SetSpeed(0.0f);
			speedY = 0.0f;
			gameOver = true;
		}

		//Take Care of all glfw Events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Window Closing Events
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();
	gltDeleteText(text1);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}