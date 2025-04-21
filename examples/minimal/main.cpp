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

#include <memory>

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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // — Настройка эмиттера —
    auto spawnStrat = std::make_shared<particle::RateSpawnStrategy>(50.0f);
    auto shape = std::make_shared<particle::CircleShape>(0.3f);
    particle::Emitter em(spawnStrat, shape);

    em.addModule(std::make_shared<particle::GravityModule>());

    // цвет over life
    using ColKF = particle::Keyframe<glm::vec4>;
    std::vector<ColKF> colorKeys = {
      {0.0f, {1,1,0,1}, nullptr},
      {0.5f, {0,1,1,1}, nullptr},
      {1.0f, {1,0,0,0}, nullptr}
    };
    em.addModule(std::make_shared<
        particle::ValueOverLifeModule<glm::vec4>
    >(colorKeys, 1.0f));

    // size over life
    using SizeKF = particle::Keyframe<float>;
    std::vector<SizeKF> sizeKeys = {
      {0.0f, 14.0f, nullptr},
      {1.0f,  2.0f, nullptr}
    };
    em.addModule(std::make_shared<
        particle::ValueOverLifeModule<float>
    >(sizeKeys, 1.0f));

    particle::GLRenderer renderer;

    // — Цикл рендеринга —
    float last = (float)glfwGetTime();
    while (!glfwWindowShouldClose(win)) {
        float now = (float)glfwGetTime();
        float dt = now - last; last = now;

        em.update(dt);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        em.render(renderer);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
