#version 330

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D MyTexture;

void main()
{
	FragColor = texture(MyTexture, TexCoord);
	//FragColor = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0);
}