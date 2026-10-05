#include <string>
#include <glew.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

class Shader
{
private:
    unsigned int shaderID;
    unsigned int vertexShader, fragmentShader;
public:
    Shader();

    void CompileShader(std::string& vertexCodePath, std::string& fragmentCodePath);
    void UseShader();

    //Upload Uniforms
    void SetMat4(const std::string& name, const glm::mat4& matrix);
    void SetVec3(const std::string& name, const glm::vec3& vector);
    void SetFloat(const std::string& name, float value);

    std::string ReadFile(std::string& filePath);
    ~Shader();
};