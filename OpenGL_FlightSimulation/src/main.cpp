#include <glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

#include "Shader.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "PerlinNoise.h"
#include "Terrain.h"

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

    GLFWwindow* window = glfwCreateWindow(1600, 800, "Flight Simulation", NULL, NULL);
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

    std::vector<float> cubeVertices = {
        // --- Ön Yüz (+Z) | Normal: (0, 0, 1) ---
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f, // 0
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f, // 1
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f, // 2
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f, // 3

        // --- Arka Yüz (-Z) | Normal: (0, 0, -1) ---
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, // 4
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, // 5
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, // 6
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, // 7

         // --- Üst Yüz (+Y) | Normal: (0, 1, 0) ---
         -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f, // 8
          0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f, // 9
          0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f, // 10
         -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f, // 11

         // --- Alt Yüz (-Y) | Normal: (0, -1, 0) ---
         -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f, // 12
          0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f, // 13
          0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f, // 14
         -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f, // 15

         // --- Sað Yüz (+X) | Normal: (1, 0, 0) ---
          0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f, // 16
          0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f, // 17
          0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f, // 18
          0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f, // 19

          // --- Sol Yüz (-X) | Normal: (-1, 0, 0) ---
          -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f, // 20
          -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, // 21
          -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f, // 22
          -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f  // 23
    };
    std::vector<unsigned int> cubeIndices = {
        // Ön yüz
        0,  1,  2,      2,  3,  0,
        // Arka yüz
        4,  5,  6,      6,  7,  4,
        // Üst yüz
        8,  9,  10,     10, 11, 8,
        // Alt yüz
        12, 13, 14,     14, 15, 12,
        // Sað yüz
        16, 17, 18,     18, 19, 16,
        // Sol yüz
        20, 21, 22,     22, 23, 20
    };
    //Shader Handling
    std::string vertexShaderPath = "src/shaders/VertexShader.vert";
    std::string fragmentShaderPath = "src/shaders/FragmentShader.frag";
    shader.CompileShader(vertexShaderPath, fragmentShaderPath);

    // Generate Perlin terrain and upload to mesh
    PerlinNoise pn(1337u);
	float scale = 10.0f; //Çok açarsan detay azalýr
	int width = 520;
	int depth = 520;
	float terrainAmplitude = 300.0f;
	int terrainOctaves = 6;
    CreatePerlinTerrain(mesh, pn, width, depth, scale, terrainAmplitude, terrainOctaves);

    // move camera back to view terrain
    camera.position = glm::vec3(0.0f, 5.0f, 20.0f);

    transform.position = glm::vec3(0.0f, -35.0f, -1.0f);
	transform.rotation = glm::vec3(0.0f, 45.0f, 0.0f);
	transform.scale = glm::vec3(0.5f);
    
   


    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glm::mat4 viewMatrix = camera.GetViewMatrix();
        glm::mat4 getProjectionMatrix = camera.GetProjectionMatrix(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);

		KeyboardMovement(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE); // Disable face culling to render both sides of the terrain

        glm::mat4 modelMatrix = transform.GetModelMatrix();

        shader.UseShader();

        transform.position = glm::vec3(45.0f, -50.0f, 45.0f); // X, Y, Z

		//Uniform Initialization
        shader.SetMat4("model", modelMatrix);
        shader.SetMat4("view", viewMatrix);
		shader.SetMat4("projection", getProjectionMatrix);
        //First Object
        shader.SetVec3("ambientColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.SetVec3("objectColor", glm::vec3(0.4f, 0.5f, 0.31f));
        shader.SetFloat("ambientStrength", 0.2f);
		shader.SetVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
		shader.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.SetFloat("specularStrength", 0.15f);
        shader.SetFloat("shininess", 32.0f);
		shader.SetVec3("viewPos", camera.position);

        mesh.RenderMesh();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}