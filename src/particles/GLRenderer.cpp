#include "particle/GLRenderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// простой шейдер
static const char* VSH = R"glsl(
#version 330 core
layout(location=0) in vec2 aPos;
uniform float uSize;
void main(){
    gl_PointSize = uSize;
    gl_Position = vec4(aPos,0,1);
}
)glsl";
static const char* FSH = R"glsl(
#version 330 core
out vec4 C;
void main(){ C = vec4(1,0.5,0.2,1); }
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
        glAttachShader(shader_, vs); glAttachShader(shader_, fs);
        glLinkProgram(shader_);
        glDeleteShader(vs); glDeleteShader(fs);

        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 1000, nullptr, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glBindVertexArray(0);
    }

    GLRenderer::~GLRenderer() {
        glDeleteProgram(shader_);
        glDeleteBuffers(1, &vbo_);
        glDeleteVertexArrays(1, &vao_);
    }

    void GLRenderer::draw(const std::vector<IParticle*>& pts) {
        std::vector<float> buf;
        buf.reserve(pts.size() * 2);
        for (auto* p : pts) {
            buf.push_back(p->position().x);
            buf.push_back(p->position().y);
        }
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, buf.size() * sizeof(float), buf.data());
        glUseProgram(shader_);
        glBindVertexArray(vao_);
        glUniform1f(glGetUniformLocation(shader_, "uSize"), 5.0f);
        glDrawArrays(GL_POINTS, 0, (GLsizei)pts.size());
        glBindVertexArray(0);
    }

} // namespace particle
