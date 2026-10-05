#version 330 core

out vec4 FragColor;

uniform vec3 ambientColor;
uniform vec3 objectColor;
uniform float ambientStrength;

void main()
{
	vec3 ambient = ambientColor * ambientStrength;
	vec3 result = ambient * objectColor;
	FragColor = vec4(result, 1.0);
}