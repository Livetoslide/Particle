#include "particle/GLRenderer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm.hpp>

// Вершинный шейдер: получает позицию, размер и цвет
static const char* VSH = R"glsl(
#version 330 core
layout(location=0) in vec2 aPos;
layout(location=1) in float aSize;
layout(location=2) in vec4 aColor;
out vec4 vColor;

void main(){
    gl_Position   = vec4(aPos, 0.0, 1.0);
    gl_PointSize  = aSize;
    vColor        = aColor;
}
)glsl";

// Фрагментный шейдер: просто выводим цвет
static const char* FSH = R"glsl(
#version 330 core
in  vec4 vColor;
out vec4 FragColor;
void main(){
    FragColor = vColor;
}
)glsl";

namespace particle {

    static unsigned compileShader(unsigned type, const char* src) {
        unsigned sh = glCreateShader(type);
        glShaderSource(sh, 1, &src, nullptr);
        glCompileShader(sh);
        int ok; char log[512];
        glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
        if (!ok) {
            glGetShaderInfoLog(sh, 512, nullptr, log);
            std::cerr << "Shader compile error: " << log << "\n";
        }
        return sh;
    }

    GLRenderer::GLRenderer() {
        // Компилируем и линкуем шейдеры
        unsigned vs = compileShader(GL_VERTEX_SHADER, VSH);
        unsigned fs = compileShader(GL_FRAGMENT_SHADER, FSH);
        shader_ = glCreateProgram();
        glAttachShader(shader_, vs);
        glAttachShader(shader_, fs);
        glLinkProgram(shader_);
        glDeleteShader(vs);
        glDeleteShader(fs);

        // Генерируем буферы
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        // Места под 1000 частиц, каждая даёт 2+1+4 = 7 float
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 7 * 1000, nullptr, GL_DYNAMIC_DRAW);

        // Позиция (vec2)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0));

        // Размер (float)
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));

        // Цвет (vec4)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    GLRenderer::~GLRenderer() {
        glDeleteBuffers(1, &vbo_);
        glDeleteVertexArrays(1, &vao_);
        glDeleteProgram(shader_);
    }

    void GLRenderer::draw(const std::vector<IParticle*>& pts) {
        // Собираем плоский массив [x,y,size,r,g,b,a,...]
        std::vector<float> data;
        data.reserve(pts.size() * 7);
        for (auto* p : pts) {
            auto& pos = p->position();
            auto  sz = p->size();
            auto& col = p->color();
            data.push_back(pos.x);
            data.push_back(pos.y);
            data.push_back(sz);
            data.push_back(col.r);
            data.push_back(col.g);
            data.push_back(col.b);
            data.push_back(col.a);
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), data.data());

        glUseProgram(shader_);
        glBindVertexArray(vao_);
        glDrawArrays(GL_POINTS, 0, (GLsizei)pts.size());
        glBindVertexArray(0);
    }

} // namespace particle
