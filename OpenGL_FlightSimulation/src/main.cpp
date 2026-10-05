#include <glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"

Shader shader;
Mesh mesh;
Transform transform;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), -90.0f, 0.0f);

// mouse state for callbacks
static bool firstMouse = true;
static double lastX = 400.0;
static double lastY = 300.0;
float currentFrame, deltaTime, lastFrame;


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//Calls when the mouse is moved
static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam) return;

    static bool firstMouse = true;
    static double lastX = 400.0, lastY = 300.0;
    if (firstMouse) { lastX = xpos; lastY = ypos; firstMouse = false; }

    float xoffset = static_cast<float>(xpos - lastX);
    float yoffset = static_cast<float>(lastY - ypos);
    lastX = xpos; lastY = ypos;

    //Updates the yaw and pitch
    cam->ProcessMouseMovement(xoffset, yoffset);
}

//WASD movement
void KeyboardMovement(GLFWwindow* window) {
    currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, CursorPosCallback);

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

    transform.position = glm::vec3(0.0f, 0.0f, -1.0f);
	transform.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
	transform.scale = glm::vec3(0.5f);
    
    glm::mat4 modelMatrix = transform.GetModelMatrix();


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 getProjectionMatrix = camera.GetProjectionMatrix(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

		KeyboardMovement(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glEnable(GL_DEPTH_TEST);

        shader.UseShader();

		//Uniform Initialization
        shader.SetMat4("model", modelMatrix);
        shader.SetMat4("view", viewMatrix);
		shader.SetMat4("projection", getProjectionMatrix);
        //First Object
        shader.SetVec3("ambientColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.SetVec3("objectColor", glm::vec3(0.4f, 0.5f, 0.31f));
        shader.SetFloat("ambientStrength", 0.2f);
        mesh.RenderMesh();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}