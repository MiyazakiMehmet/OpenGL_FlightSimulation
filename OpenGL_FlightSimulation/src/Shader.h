#include <string>
#include <glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader
{
private:
    unsigned int shaderID;
    unsigned int vertexShader, fragmentShader;
public:
    Shader();

    void CompileShader(std::string& vertexCodePath, std::string& fragmentCodePath);
    void UseShader();
    std::string ReadFile(std::string& filePath);
    ~Shader();
};