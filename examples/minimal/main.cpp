#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <particle/Emitter.hpp>
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

    particle::Emitter em;
    em.addModule(std::make_shared<particle::GravityModule>());
    em.addModule(std::make_shared<particle::ColorFadeModule>(
        glm::vec4{ 1,1,0,1 }, glm::vec4{ 1,0,0,0 }, 3.0f
    ));
    em.addModule(std::make_shared<particle::SizeModule>(
        10.0f, 2.0f, 3.0f
    ));

    particle::GLRenderer rnd;
    float last = (float)glfwGetTime();
    while (!glfwWindowShouldClose(win)) {
        float now = (float)glfwGetTime(), dt = now - last; last = now;
        em.spawn();
        em.update(dt);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        em.render(rnd);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
