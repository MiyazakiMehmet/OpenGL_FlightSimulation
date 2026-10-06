#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform float shininess;  

void main()
{
	// normalization
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// ambient
	vec3 ambient = ambientColor * ambientStrength;
	
	// diffuse (Lambertian reflection)
	float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;


    vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}