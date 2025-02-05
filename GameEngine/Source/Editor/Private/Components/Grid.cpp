#include "Components/Grid.h"

#include <gtc/type_ptr.hpp>

#include "Render/Shader.h"

Grid::Grid(float InitSize, int InitDivisions)
{
	Size = InitSize;
	Divisions = InitDivisions;

	SetupGrid();
}

void Grid::Draw(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix)
{
	GridShader->Use();

	glUniform4f(UniformGridColorLocation, 0.5f, 1.0f, 0.5f, 0.15f);
	glUniformMatrix4fv(UniformViewMatrix, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(UniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

	glBindVertexArray(VAO);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(-1.0f, -1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GridShader->Validate();

	glDrawArrays(GL_LINES, 0, VertexCount);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_BLEND);
}

void Grid::SetupGrid()
{
	GridShader = std::make_unique<Shader>("Source/Engine/Public/Render/Shaders/grid_shader.vert",
											"Source/Engine/Public/Render/Shaders/grid_shader.frag");

	UniformViewMatrix = GridShader->GetViewLocation();
	UniformGridColorLocation = GridShader->GetGridColorLocation();
	UniformProjectionMatrix = GridShader->GetProjectionLocation();

	std::vector<float> Vertices;
	const float Step = Size / Divisions;

	for (int Index = -Divisions; Index <= Divisions; ++Index) {
		float position = Index * Step;

		Vertices.push_back(position);
		Vertices.push_back(0.0f);
		Vertices.push_back(-Size);

		Vertices.push_back(position);
		Vertices.push_back(0.0f);
		Vertices.push_back(Size);

		Vertices.push_back(-Size);
		Vertices.push_back(0.0f);
		Vertices.push_back(position);

		Vertices.push_back(Size);
		Vertices.push_back(0.0f);
		Vertices.push_back(position);
	}

	for(auto& vertex : Vertices)
	{
		vertex -= 1.1f;
	}

	VertexCount = Vertices.size() / 3;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), Vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
