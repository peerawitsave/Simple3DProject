
// [top includes unchanged]
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Mesh.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool is3DMode = false;

std::string LoadShaderSource(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Failed to load shader: " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compile error:\n" << infoLog << std::endl;
    }
    return shader;
}

GLuint CreateShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = LoadShaderSource(vertexPath);
    std::string fragmentCode = LoadShaderSource(fragmentPath);

    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentCode);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple 3D Object", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
    ImGui::StyleColorsDark();

    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);

    GLuint shaderProgram = CreateShaderProgram("shaders/basic.vert", "shaders/basic.frag");

    enum ShapeType { TRIANGLE, RECTANGLE, CIRCLE, PYRAMID };
    ShapeType currentShape = TRIANGLE;

    Mesh* triangle = Mesh::CreateTriangle();
    Mesh* rectangle = Mesh::CreateQuad();
    Mesh* circle = Mesh::CreateCircle();
    Mesh* pyramid = Mesh::CreatePyramid();
    Mesh* backdrop = Mesh::CreateBackdropPlane();

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    float lightIntensity = 1.0f;
    bool animateLight = false;

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Shape & Lighting", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos(ImVec2(10, 10));
        ImGui::Checkbox("Enable 3D Mode", &is3DMode);
        const char* lightOptions[] = { "Top-Left", "Bottom-Right", "Front" };
        static int selectedLight = 0;
        ImGui::Combo("Light Direction", &selectedLight, lightOptions, IM_ARRAYSIZE(lightOptions));
        ImGui::Checkbox("Animate Light", &animateLight);
        if (ImGui::Button("Show Triangle")) currentShape = TRIANGLE;
        if (ImGui::Button("Show Rectangle")) currentShape = RECTANGLE;
        if (ImGui::Button("Show Circle")) currentShape = CIRCLE;
        if (ImGui::Button("Show Pyramid")) currentShape = PYRAMID;
        ImGui::ColorEdit3("Light Color", glm::value_ptr(lightColor));
        ImGui::SliderFloat("Intensity", &lightIntensity, 0.0f, 5.0f);
        ImGui::End();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        float time = glfwGetTime();
        float offsetY = sin(time) * 0.2f;
        float scale = 0.8f + 0.2f * sin(time * 2.0f);

        glm::vec3 lightPresets[] = {
            glm::normalize(glm::vec3(-0.5f, 1.0f, 0.3f)),
            glm::normalize(glm::vec3(1.0f, -1.0f, -0.2f)),
            glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f))
        };
        glm::vec3 lightDir = animateLight ? glm::normalize(glm::vec3(cos(time), 1.0f, sin(time))) : lightPresets[selectedLight];
        glm::vec3 effectiveLight = lightColor * lightIntensity;
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);

        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        if (is3DMode) {
            view = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        }

        // --- BACKDROP ---
        glm::mat4 identityModel = glm::mat4(1.0f);
        glUniform1i(glGetUniformLocation(shaderProgram, "isShadow"), 0);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(identityModel));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightDir"), 1, glm::value_ptr(lightDir));
        glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(effectiveLight));
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(cameraPos));
        glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
        backdrop->Draw();

        // --- SHADOW PASS ---
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, offsetY, 0.0f));
        model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
        model = glm::rotate(model, time, is3DMode ? glm::vec3(0.3f, 1.0f, 0.0f) : glm::vec3(0.0f, 0.0f, 1.0f));

        glUniform1i(glGetUniformLocation(shaderProgram, "isShadow"), 0);
        glm::mat4 identity = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(identity));
        backdrop->Draw();

        switch (currentShape) {
        case TRIANGLE: triangle->Draw(); break;
        case RECTANGLE: rectangle->Draw(); break;
        case CIRCLE: circle->Draw(); break;
        case PYRAMID: pyramid->Draw(); break;
        }

        glm::mat4 shadowMat = glm::mat4(1.0f);
        shadowMat = glm::translate(shadowMat, glm::vec3(0, 0.01f, 0)); // slight lift
        shadowMat *= glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 0.0f, 1.0f)); // flatten Y

        // --- MAIN OBJECT PASS ---
        glUniform1i(glGetUniformLocation(shaderProgram, "isShadow"), 0);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        switch (currentShape) {
        case TRIANGLE: triangle->Draw(); break;
        case RECTANGLE: rectangle->Draw(); break;
        case CIRCLE: circle->Draw(); break;
        case PYRAMID: pyramid->Draw(); break;
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete triangle;
    delete rectangle;
    delete circle;
    delete pyramid;
    delete backdrop;
    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
