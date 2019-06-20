#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
//uniform vec4 ourColor;

void main()
{
	//FragColor = vertexColor;
	FragColor = vec4(vertexColor, 1.0);
}