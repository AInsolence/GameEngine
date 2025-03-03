#pragma once
#include <fwd.hpp>
#include <memory>
#include <GL/glew.h>

class Shader;

class Grid
{
public:
	Grid(float InitSize, int InitDivisions);

	void Draw(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix);

	float GetSize() const { return Size; }
	int GetDivisions() const { return Divisions; }

	void SetSize(float InSize) { Size = InSize; }
	void SetDivision(int InDivisions) { Divisions = InDivisions; }

private:
	float Size;
	int Divisions;

	unsigned int VAO, VBO;
	int VertexCount;

	GLint UniformGridColorLocation;
	GLint UniformViewMatrix;
	GLint UniformProjectionMatrix;

	std::unique_ptr<Shader> GridShader;

	void SetupGrid();
};

