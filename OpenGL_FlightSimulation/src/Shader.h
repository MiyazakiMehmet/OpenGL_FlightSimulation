#include <string>

class Shader
{
private:

public:
    Shader();

    void CompileShader(std::string& vertexCodePath, std::string& fragmentCodePath);
    ~Shader();
};