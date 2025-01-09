#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertColor;

uniform sampler2D MyTexture;

void main()
{
	FragColor = texture(MyTexture, TexCoord) * VertColor;
	//FragColor = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0);
}