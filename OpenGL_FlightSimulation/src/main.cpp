#include <glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"

Shader shader;
Mesh mesh;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Eski fonksiyonlarý kapat

    GLFWwindow* window = glfwCreateWindow(800, 600, "Flight Simulation", NULL, NULL);
    if (window == NULL) {
        std::cout << "GLFW window could not be created!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW not initialized!" << std::endl;
        return -1;
    }

    std::vector<float> vertices = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
    };
    std::vector<unsigned int> indices = {
        0, 1, 2
    };

    //Shader Handling
    std::string vertexShaderPath = "src/shaders/VertexShader.vert";
    std::string fragmentShaderPath = "src/shaders/FragmentShader.frag";
    shader.CompileShader(vertexShaderPath, fragmentShaderPath);

    mesh.CompileMesh(vertices, indices);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Koyu turkuaz arka plan
        glClear(GL_COLOR_BUFFER_BIT);

        shader.UseShader();
        mesh.RenderMesh();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}