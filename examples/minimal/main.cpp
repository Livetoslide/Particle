#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <particle/Emitter.hpp>
#include <particle/spawn/RateSpawnStrategy.hpp>
#include <particle/spawn/BurstSpawnStrategy.hpp>
#include <particle/shape/CircleShape.hpp>
#include <particle/shape/BoxShape.hpp>
#include <particle/GravityModule.hpp>
#include <particle/ColorFadeModule.hpp>
#include <particle/SizeModule.hpp>
#include <particle/GLRenderer.hpp>

#include <memory>

int main() {

    glfwInit();
    GLFWwindow* win = glfwCreateWindow(800, 600, "Particles", nullptr, nullptr);
    glfwMakeContextCurrent(win);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 800, 600);

    auto rateStrat = std::make_shared<particle::RateSpawnStrategy>(300.0f);
    auto circle = std::make_shared<particle::CircleShape>(0.1f);
    std::vector<particle::Burst> bursts = { {0.5f, 100}, {1.5f, 200} };
    auto burstStrat = std::make_shared<particle::BurstSpawnStrategy>(bursts);
    particle::Emitter em(rateStrat, circle);

    // добавляем модули поведения
    em.addModule(std::make_shared<particle::GravityModule>());
    em.addModule(std::make_shared<particle::ColorFadeModule>(
        glm::vec4{ 1,1,0,1 }, glm::vec4{ 1,0,0,0 }, 3.0f
    ));
    em.addModule(std::make_shared<particle::SizeModule>(
        8.0f, 0.0f, 3.0f
    ));

    particle::GLRenderer rnd;
    float last = (float)glfwGetTime();
    while (!glfwWindowShouldClose(win)) {
        float now = (float)glfwGetTime(), dt = now - last; last = now;

        em.update(dt);

        glClear(GL_COLOR_BUFFER_BIT);
        em.render(rnd);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
