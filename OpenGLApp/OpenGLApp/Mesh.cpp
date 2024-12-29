#include "Mesh.h"


Mesh::Mesh(const GLfloat* Vertices,
		const unsigned* Indices,
		const unsigned NumOfVertices,
		const unsigned NumOfIndices)
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	IndexCount = 0;

	Create(Vertices, Indices, NumOfVertices, NumOfIndices);
}

Mesh::~Mesh()
{
	Clear();
}


void Mesh::Create(const GLfloat* Vertices,
					const unsigned* Indices,
					const unsigned NumOfVertices,
					const unsigned NumOfIndices)
{
	IndexCount = NumOfIndices;

	glGenVertexArrays(1, &VAO); // first param here number of VAOs we are creating
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * NumOfIndices, Indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * NumOfVertices, Vertices, GL_STATIC_DRAW);

	// With attributes explaining how to use data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Render()
{
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

		glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Clear()
{
	if (IBO != 0)
	{
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	IndexCount = 0;
}
