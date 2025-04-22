#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <particle/Emitter.hpp>
#include <particle/spawn/RateSpawnStrategy.hpp>
#include <particle/spawn/BurstSpawnStrategy.hpp>
#include <particle/shape/CircleShape.hpp>
#include <particle/shape/BoxShape.hpp>
#include <particle/GravityModule.hpp>
#include <particle/ColorFadeModule.hpp>
#include <particle/SizeModule.hpp>
#include <particle/GLRenderer.hpp>
#include <particle/ValueOverLifeModule.hpp>
#include <particle/SpriteRenderer.hpp>
#include <particle/NoiseModule.hpp>
#include <particle/BlendMode.hpp>
#include <particle/AngularVelocityModule.hpp>

#include <gtc/matrix_transform.hpp>

#include <memory>
#include <filesystem>

int main() {
    // — Инициализация GLFW и GLAD —
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(800, 600, "Particle Test", nullptr, nullptr);
    if (!win) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;
    glViewport(0, 0, 800, 600);

    // — Включаем программный PointSize и блендинг —
    glEnable(GL_PROGRAM_POINT_SIZE);

    glm::mat4 proj = glm::ortho(-1.0f, +1.0f, -1.0f, +1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 viewProj = proj * view;

    // — Настройка эмиттера —
    auto spawnStrat = std::make_shared<particle::RateSpawnStrategy>(20.0f);
    auto shape = std::make_shared<particle::CircleShape>(0.2f);
    particle::Emitter em(spawnStrat, shape, glm::vec2(0.0f, +0.2f), 2.0f);

    em.addModule(std::make_shared<particle::GravityModule>(glm::vec2{0.0f, -0.1f}));

    using ColKF = particle::Keyframe<glm::vec4>;
    std::vector<ColKF> colorKeys = {
      {0.0f, {1,1,1,0.3f}, nullptr},
      {1.0f, {1,1,1,0.0f}, nullptr}
    };
    em.addModule(std::make_shared<
        particle::ValueOverLifeModule<glm::vec4>
    >(colorKeys, 2.0f));

    using SizeKF = particle::Keyframe<float>;
    std::vector<SizeKF> sizeKeys = {
      {0.0f, 0.05f, nullptr},
      {1.0f, 0.25f, nullptr}
    };
    em.addModule(std::make_shared<
        particle::ValueOverLifeModule<float>
    >(sizeKeys, 2.0f));

    em.addModule(std::make_shared<particle::NoiseModule>( /*magnitude=*/0.1f));
    em.addModule(std::make_shared<particle::AngularVelocityModule>(3.14f));
    particle::SpriteRenderer renderer("textures/particle.png");
    int locViewProj = glGetUniformLocation(renderer.getShader(), "uViewProj");
    renderer.setBlendMode(particle::BlendMode::Normal);
    // — Цикл рендеринга —
    float lastTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(win)) {
        float now = (float)glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;

        em.update(dt);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwGetKey(win, GLFW_KEY_1) == GLFW_PRESS) {
            renderer.setBlendMode(particle::BlendMode::Normal);
        }
        if (glfwGetKey(win, GLFW_KEY_2) == GLFW_PRESS) {
            renderer.setBlendMode(particle::BlendMode::Additive);
        }
        if (glfwGetKey(win, GLFW_KEY_3) == GLFW_PRESS) {
            renderer.setBlendMode(particle::BlendMode::Multiply);
        }
        if (glfwGetKey(win, GLFW_KEY_4) == GLFW_PRESS) {
            renderer.setBlendMode(particle::BlendMode::Screen);
        }

        // Передаём матрицу в шейдер
        glUseProgram(renderer.getShader());
        glUniformMatrix4fv(locViewProj, 1, GL_FALSE, &viewProj[0][0]);

        em.render(renderer);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
