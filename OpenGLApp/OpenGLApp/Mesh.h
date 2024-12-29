#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh(const GLfloat* Vertices,
		const unsigned int* Indices,
		const unsigned int NumOfVertices,
		const unsigned int NumOfIndices);

	~Mesh();

	void Create(const GLfloat* Vertices,
					const unsigned int* Indices,
					const unsigned int NumOfVertices,
					const unsigned int NumOfIndices);

	void Render();
	void Clear();

private:
	GLuint VAO, VBO, IBO;
	GLsizei IndexCount;
};

