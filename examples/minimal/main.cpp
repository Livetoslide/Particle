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
    // --- Инициализация GLFW и GLAD ---
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Particle Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD\n";
        return -1;
    }
    glViewport(0, 0, 800, 600);

    // --- Создание эмиттера ---
    // Спавн: 50 частиц в секунду
    auto spawnStrat = std::make_shared<particle::RateSpawnStrategy>(50.0f);
    // Форма: круг радиуса 0.5
    auto shape = std::make_shared<particle::CircleShape>(0.07f);
    particle::Emitter em(spawnStrat, shape);

    // --- Добавление модулей поведения ---
    // 1) Гравитация
    //em.addModule(std::make_shared<particle::GravityModule>());

    // 2) Изменение цвета по жизни (ValueOverLifeModule<glm::vec4>)
    using ColKF = particle::Keyframe<glm::vec4>;
    std::vector<ColKF> colorKeys = {
        { 0.0f, glm::vec4{1,1,0,1},       nullptr                  }, // жёлтый в начале
        { 0.5f, glm::vec4{0,1,1,1},       nullptr                  }, // циан в середине
        { 1.0f, glm::vec4{1,0,0,0},       [](float t) { return t; } }  // краснеем и таем
    };
    em.addModule(std::make_shared<
        particle::ValueOverLifeModule<glm::vec4>
    >(colorKeys, /*maxLife=*/1.0f));

    // 3) Изменение размера по жизни (ValueOverLifeModule<float>)
    using SizeKF = particle::Keyframe<float>;
    std::vector<SizeKF> sizeKeys = {
        { 0.0f, 10.0f, nullptr },                          // стартовый размер 10
        { 1.0f,  2.0f, [](float t) { return 1 - (1 - t) * (1 - t); } } // плавно уменьшаем
    };
    em.addModule(std::make_shared<
        particle::ValueOverLifeModule<float>
    >(sizeKeys, /*maxLife=*/3.0f));

    // --- Рендерер ---
    particle::GLRenderer renderer;

    // --- Основной цикл ---
    float lastTime = (float)glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // вычисляем dt
        float now = (float)glfwGetTime();
        float dt = now - lastTime;
        lastTime = now;

        // обновляем систему (спавн + apply модулей + обновление позиции/жизни)
        em.update(dt);

        // отрисовка
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        em.render(renderer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
