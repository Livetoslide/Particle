#include "particle/GLRenderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm.hpp>

// Шейдеры теперь принимают атрибуты: vec2 aPos; float aSize; vec4 aColor;
static const char* VSH = R"glsl(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in float aSize;
layout(location=2) in vec4 aColor;
out vec4 vColor;
void main(){
    gl_PointSize = aSize;
    gl_Position  = vec4(aPos, 0.0, 1.0);
    vColor       = aColor;
}
)glsl";
static const char* FSH = R"glsl(
#version 330 core
in vec4 vColor;
out vec4 FragColor;
void main(){
    FragColor = vColor;
}
)glsl";

namespace particle {

    static unsigned compile(GLenum t, const char* src) {
        unsigned s = glCreateShader(t);
        glShaderSource(s, 1, &src, nullptr);
        glCompileShader(s);
        int ok; char log[512];
        glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            glGetShaderInfoLog(s, 512, nullptr, log);
            std::cerr << "Shader error: " << log << "\n";
        }
        return s;
    }

    GLRenderer::GLRenderer() {
        unsigned vs = compile(GL_VERTEX_SHADER, VSH),
            fs = compile(GL_FRAGMENT_SHADER, FSH);
        shader_ = glCreateProgram();
        glAttachShader(shader_, vs);
        glAttachShader(shader_, fs);
        glLinkProgram(shader_);
        glDeleteShader(vs);
        glDeleteShader(fs);

        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        // макс. частиц 1000, по 7 float на каждую: (2+1+4)=7
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 7 * 1000, nullptr, GL_DYNAMIC_DRAW);
        // позиция
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        // размер
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
        // цвет
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    GLRenderer::~GLRenderer() {
        glDeleteProgram(shader_);
        glDeleteBuffers(1, &vbo_);
        glDeleteVertexArrays(1, &vao_);
    }

    void GLRenderer::draw(const std::vector<IParticle*>& pts) {
        std::vector<float> buf;
        buf.reserve(pts.size() * 7);
        for (auto* p : pts) {
            buf.push_back(p->position().x);
            buf.push_back(p->position().y);
            buf.push_back(p->size());
            buf.push_back(p->color().r);
            buf.push_back(p->color().g);
            buf.push_back(p->color().b);
            buf.push_back(p->color().a);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, buf.size() * sizeof(float), buf.data());
        glUseProgram(shader_);
        glBindVertexArray(vao_);
        glDrawArrays(GL_POINTS, 0, (GLsizei)pts.size());
        glBindVertexArray(0);
    }

} // namespace particle
