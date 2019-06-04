#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"
#include "Camera.h"
#include "Spectator.h"

const int WIDTH = 1600;
const int HEIGHT = 900;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    unsigned int err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Spectator spectator;
    spectator.setPosition(glm::vec3(0.0f, 0.0f, -1.0f));

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f,    0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,    0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,    0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,   -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,   -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f,    0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,    0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,    0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f,    0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f,    0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,    0.0f,  1.0f,  0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    rigel::VertexArray va;
    rigel::VertexBuffer vb(vertices, sizeof(vertices));

    rigel::VertexBufferLayout layout;
    layout.push(GL_FLOAT, 3, false);
    layout.push(GL_FLOAT, 2, false);
    layout.push(GL_FLOAT, 3, false);
    va.addBuffer(vb, layout);

    rigel::IndexBuffer ib(indices, 36);

    // Box shader
    rigel::Shader shader("res/shaders/standart.shader");
    shader.bind();

    rigel::Texture pixelTexture("res/textures/pixel.png");
    rigel::Texture boxTexture("res/textures/box.png");
    pixelTexture.bind(0);
    boxTexture.bind(1);

    // Material
    shader.setUniform3f("u_material.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform3f("u_material.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform3f("u_material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform1f("u_material.shininess", 32.0f);

    // Directional light
    shader.setUniform1i("u_numDirectionalLights", 1);

    shader.setUniform3f("u_directionalLights[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
    shader.setUniform3f("u_directionalLights[0].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.setUniform3f("u_directionalLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform3f("u_directionalLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));

    // Point light
    shader.setUniform1i("u_numPointLights", 3);

    for (int i = 0; i < 3; i++)
    {
        shader.setUniform3f("u_pointLights[" + std::to_string(i) + "].position", glm::vec3(-20.0f + i * 20, 2.0f, 0.0f));

        shader.setUniform1f("u_pointLights[" + std::to_string(i) + "].constant", 1.0f);
        shader.setUniform1f("u_pointLights[" + std::to_string(i) + "].linear", 0.022f);
        shader.setUniform1f("u_pointLights[" + std::to_string(i) + "].quadratic", 0.0019f);

        shader.setUniform3f("u_pointLights[" + std::to_string(i) + "].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
        shader.setUniform3f("u_pointLights[" + std::to_string(i) + "].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setUniform3f("u_pointLights[" + std::to_string(i) + "].specular", glm::vec3(1.0f, 1.0f, 1.0f));
    }

    // Spot light
    shader.setUniform1i("u_numSpotLights", 1);

    shader.setUniform3f("u_spotLights[0].position", glm::vec3(0.0f, 2.0f, 10.0f));
    shader.setUniform3f("u_spotLights[0].direction", glm::vec3(0.0f, -1.0f, -2.0f));

    shader.setUniform1f("u_spotLights[0].cutOff", glm::cos(glm::radians(16.5f)));
    shader.setUniform1f("u_spotLights[0].outerCutOff", glm::cos(glm::radians(20.0f)));

    shader.setUniform1f("u_spotLights[0].constant", 1.0f);
    shader.setUniform1f("u_spotLights[0].linear", 0.022f);
    shader.setUniform1f("u_spotLights[0].quadratic", 0.0019f);

    shader.setUniform3f("u_spotLights[0].ambient", glm::vec3(0.1f, 0.1f, 0.1f));
    shader.setUniform3f("u_spotLights[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
    shader.setUniform3f("u_spotLights[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));


    shader.unbind();

    va.unbind();
    vb.unbind();
    ib.unbind();

    rigel::Renderer renderer;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Exit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        spectator.update(window, deltaTime);
        renderer.clear();

        shader.bind();
        glm::vec3 cameraPosition = spectator.getCamera().getPosition();
        shader.setUniform3f("u_viewPos", cameraPosition);

        glm::mat4 view = spectator.getCamera().getViewMatrix();
        shader.setUniformMat4f("u_view", view);

        glm::mat4 proj = spectator.getCamera().getProjectionMatrix();
        shader.setUniformMat4f("u_proj", proj);

        shader.setUniform1i("u_texture", 1);
        shader.setUniform3f("u_color", glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
        shader.setUniformMat4f("u_model", model);
        renderer.drawElements(va, ib, shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 3.0f));
        shader.setUniformMat4f("u_model", model);
        renderer.drawElements(va, ib, shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 3.0f));
        shader.setUniformMat4f("u_model", model);
        renderer.drawElements(va, ib, shader);

        shader.setUniform1i("u_texture", 0);
        shader.setUniform3f("u_color", glm::vec3(0.5f, 0.5f, 0.5f));

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 3.0f));
        model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
        shader.setUniformMat4f("u_model", model);
        renderer.drawElements(va, ib, shader);

        shader.unbind();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}