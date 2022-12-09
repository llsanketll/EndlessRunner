#ifndef VERTEX_ARRAY_OBJECT
#define VERTEX_ARRAY_OBJECT
#include <glad/glad.h>
#include "VertexBufferObject.h"
class VertexArrayObject
{
public:
	GLuint ID;
	VertexArrayObject();

	void LinkAttirb(VertexBufferObject& VBO, GLuint layout, GLint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};
#endif // !VERTEX_ARRAY_OBJECT

