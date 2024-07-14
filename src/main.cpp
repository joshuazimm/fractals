#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Camera.h" // Include your Camera class header

#define MENGER_DEPTH 4
#define SHADER_PROGRAM_ERROR -1

// Initialize camera
Camera camera;

// Movement settings
bool keys[1024] = { 0 };
const float deltaTime = 0.01f; // wasd velocity
const float sensitivity = 0.1; // mouse velocity

std::vector<glm::vec3> cubeVertices;
std::vector<glm::vec3> cubeNormals;

void addCubeVertices() {
    glm::vec3 vertices[] = {
        glm::vec3(-1.0f, -1.0f, -1.0f), // Front face
        glm::vec3(1.0f, -1.0f, -1.0f),
        glm::vec3(1.0f, 1.0f, -1.0f),
        glm::vec3(-1.0f, 1.0f, -1.0f),
        glm::vec3(-1.0f, -1.0f, 1.0f),  // Back face
        glm::vec3(1.0f, -1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(-1.0f, 1.0f, 1.0f)
    };

    GLuint indices[] = {
        0, 1, 2, 0, 2, 3, // Front face
        4, 5, 6, 4, 6, 7, // Back face
        0, 3, 7, 0, 7, 4, // Left face
        1, 5, 6, 1, 6, 2, // Right face
        3, 2, 6, 3, 6, 7, // Top face
        0, 1, 5, 0, 5, 4  // Bottom face
    };

    glm::vec3 normals[] = {
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, -1.0f, 0.0f)
    };

    for (int i = 0; i < 36; ++i) {
        cubeVertices.push_back(vertices[indices[i]]);
        cubeNormals.push_back(normals[i / 6]);
    }
}

void generateMengerSponge(std::vector<glm::mat4>& instanceTransforms, const glm::vec3 &min, const glm::vec3 &max, const int &depth) {
    if (depth == 0) {
        glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), (min + max) * 0.5f), (max - min) * 0.5f);
        instanceTransforms.push_back(transform);
        return;
    }

    glm::vec3 size = (max - min) / 3.0f;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                if ((i == 1 && j == 1) || (i == 1 && k == 1) || (j == 1 && k == 1) || (i == 1 && j == 1 && k == 1)) {
                    continue;
                }

                glm::vec3 newMin = min + glm::vec3(i, j, k) * size;
                glm::vec3 newMax = newMin + size;
                generateMengerSponge(instanceTransforms, newMin, newMax, depth - 1);
            }
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static double lastX, lastY;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;

    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.rotate(xoffset, yoffset, 0.0f);
}

std::string loadShaderSource(const char* filePath) {
    std::ifstream shaderFile;
    std::stringstream shaderStream;
    shaderFile.open(filePath);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return shaderStream.str();
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint linkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "LINKING_FAILED\n" << infoLog << std::endl;
    }
    return program;
}

// Key Callbacks
void processInput() {
    if (keys[GLFW_KEY_W])
        camera.moveForward(deltaTime);
    if (keys[GLFW_KEY_S])
        camera.moveBackward(deltaTime);
    if (keys[GLFW_KEY_A])
        camera.moveLeft(deltaTime);
    if (keys[GLFW_KEY_D])
        camera.moveRight(deltaTime);
}

int init(GLFWwindow*& window) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(1920, 1080, "GLFW Camera Rotation Example", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable the cursor
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    camera.setPosition(glm::vec3(0.0f, 0.0f, 5.0f));

    std::string vertexSource = loadShaderSource("bin/vertex.glsl");
    std::string fragmentSource = loadShaderSource("bin/fragment.glsl");

    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());
    GLuint shaderProgram = linkProgram(vertexShader, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    GLFWwindow* window;
    GLuint shaderProgram = init(window);
    if (shaderProgram == SHADER_PROGRAM_ERROR)
        return -1;

    addCubeVertices();
    std::vector<glm::mat4> instanceTransforms;
    generateMengerSponge(instanceTransforms, glm::vec3(-1.0f), glm::vec3(1.0f), MENGER_DEPTH);

    GLuint VAO, VBO, NBO, instanceVBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &NBO);
    glGenBuffers(1, &instanceVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(glm::vec3), cubeVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, NBO);
    glBufferData(GL_ARRAY_BUFFER, cubeNormals.size() * sizeof(glm::vec3), cubeNormals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, instanceTransforms.size() * sizeof(glm::mat4), instanceTransforms.data(), GL_STATIC_DRAW);

    for (int i = 0; i < 4; i++) {
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Projection matrix is constant until fov changing is implemented
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f); // Identity 4x4

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera.getViewMatrix();

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.getPosition()));

        // Set material properties
        glUniform3f(glGetUniformLocation(shaderProgram, "material.ambient"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.diffuse"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), 32.0f);

        // Set light properties
        glUniform3f(glGetUniformLocation(shaderProgram, "light.position"), 3.0f, 3.0f, 3.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "light.ambient"), 0.2f, 0.2f, 0.2f);
        glUniform3f(glGetUniformLocation(shaderProgram, "light.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(shaderProgram, "light.specular"), 1.0f, 1.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, cubeVertices.size(), instanceTransforms.size());
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &NBO);
    glDeleteBuffers(1, &instanceVBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}