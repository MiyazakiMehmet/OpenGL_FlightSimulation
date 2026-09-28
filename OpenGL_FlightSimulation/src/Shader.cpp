#include "Shader.h"

Shader::Shader() {
	shaderID = 0;
}

void Shader::CompileShader(std::string& vertexCodePath, std::string& fragCodePath) {
	std::string vertexCodeStr = ReadFile(vertexCodePath);
	std::string fragCodeStr = ReadFile(fragCodePath);

	const char* vertexCode = vertexCodeStr.c_str();
	const char* fragCode = fragCodeStr.c_str();

	shaderID = glCreateProgram();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexCode, NULL);
	glCompileShader(vertexShader);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragCode, NULL);
	glCompileShader(fragmentShader);

	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::UseShader() {
	glUseProgram(shaderID);
}

std::string Shader::ReadFile(std::string& filePath)
{
	std::ifstream file(filePath);
	std::stringstream buffer;

	if (!file.is_open()) {
		std::cout << "Failed to open file:" << filePath << std::endl;

		return "";
	}
	buffer << file.rdbuf(); //Read and store entire file into string stream
	file.close();

	return buffer.str(); //Transform string and then c-style string

}

Shader::~Shader() {
	glDeleteProgram(shaderID);
}