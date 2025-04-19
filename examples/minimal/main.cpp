#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <particle/Emitter.hpp>
#include <particle/GLRenderer.hpp>
#include <particle/GravityModule.hpp>

int main() {
    glfwInit();
    GLFWwindow* w = glfwCreateWindow(800, 600, "Particles", nullptr, nullptr);
    glfwMakeContextCurrent(w);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glViewport(0, 0, 800, 600);

    particle::Emitter em;
    em.addModule(std::make_shared<particle::GravityModule>());
    particle::GLRenderer rnd;

    float last = (float)glfwGetTime();
    while (!glfwWindowShouldClose(w)) {
        float now = (float)glfwGetTime();
        float dt = now - last; last = now;

        em.spawn();
        em.update(dt);

        glClearColor(0.1f, 0.1f, 0.1f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        em.render(rnd);

        glfwSwapBuffers(w);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
