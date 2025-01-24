#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 LightMatrices[6];
out vec4 FragPosition;

void main()
{
	for (int Face = 0; Face < 6; ++Face)
	{
		gl_Layer = Face;
		
		for (int i = 0; i < 3; ++i)
		{
			FragPosition = gl_in[i].gl_Position;
			gl_Position = LightMatrices[Face] * FragPosition;

			EmitVertex();
		}

		EndPrimitive();
	}
}