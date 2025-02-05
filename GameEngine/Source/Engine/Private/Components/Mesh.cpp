#include "Components/Mesh.h"


Mesh::Mesh(const std::vector<GLfloat>& Vertices,
			const std::vector<unsigned int>& Indices)
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	IndexCount = 0;

	Create(Vertices, Indices);
}

Mesh::~Mesh()
{
	Clear();
}


void Mesh::Create(const std::vector<GLfloat>& Vertices,
					const std::vector<unsigned int>& Indices)
{
	IndexCount = Indices.size();

	glGenVertexArrays(1, &VAO); // first param here number of VAOs we are creating
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), Indices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices[0]) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

	/// With attributes explaining how to use data
	// vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 8, nullptr);
	glEnableVertexAttribArray(0);
	// texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 8, reinterpret_cast<void*>(sizeof(Vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	// normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertices[0]) * 8, reinterpret_cast<void*>(sizeof(Vertices[0]) * 5));
	glEnableVertexAttribArray(2);
	///
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::Render() const
{
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::Clear()
{
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	VAO = 0;
	IBO = 0;
	VBO = 0;

	IndexCount = 0;
}
