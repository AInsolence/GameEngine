#pragma once

#include <vector>
#include <GL/glew.h>

class Mesh
{
public:
	Mesh(const std::vector<GLfloat>& Vertices,
			const std::vector<unsigned int>& Indices);

	~Mesh();

	void Create(const std::vector<GLfloat>& Vertices,
				const std::vector<unsigned int>& Indices);

	void Render() const;
	void Clear();

private:
	GLuint VAO, VBO, IBO;
	int IndexCount;
};

