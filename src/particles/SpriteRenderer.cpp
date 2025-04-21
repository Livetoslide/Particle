#include "particle/SpriteRenderer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <gtc/matrix_transform.hpp>
#include <iostream>

// ¬ершинный шейдер дл€ инстанс-рендеринга спрайтов
static const char* VSH = R"glsl(
#version 330 core
layout(location=0) in vec2 aQuadPos;
layout(location=1) in vec2 aOffset;
layout(location=2) in float aSize;
layout(location=3) in float aAngle;
layout(location=4) in vec4 aColor;

out vec2 vUV;
out vec4 vColor;
uniform mat4 uViewProj;

void main() {
    float s = aSize;
    mat2 rot = mat2(cos(aAngle), -sin(aAngle), sin(aAngle), cos(aAngle));
    vec2 pos = rot * (aQuadPos * s) + aOffset;
    gl_Position = uViewProj * vec4(pos, 0.0, 1.0);
    vUV = aQuadPos * 0.5 + 0.5;
    vColor = aColor;
}
)glsl";

// ‘рагментный шейдер
static const char* FSH = R"glsl(
#version 330 core
in vec2 vUV;
in vec4 vColor;
out vec4 FragColor;
uniform sampler2D uTex;
void main() {
    vec4 tex = texture(uTex, vUV);
    FragColor = tex * vColor;
}
)glsl";


namespace particle {

    SpriteRenderer::SpriteRenderer(const std::string& texturePath) {
        initShaders();
        initBuffers();
        texture_ = loadTexture(texturePath);
    }

    SpriteRenderer::~SpriteRenderer() {
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ebo_);
        glDeleteVertexArrays(1, &vao_);
        glDeleteProgram(shader_);
        glDeleteTextures(1, &texture_);
    }
    void SpriteRenderer::initShaders() {
        auto compile = [&](GLenum type, const char* src) {
            unsigned sh = glCreateShader(type);
            glShaderSource(sh, 1, &src, nullptr);
            glCompileShader(sh);
            int ok; char log[512];
            glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
            if (!ok) glGetShaderInfoLog(sh, 512, nullptr, log), std::cerr << "Shader error: " << log << "\n";
            return sh;
            };
        unsigned vs = compile(GL_VERTEX_SHADER, VSH);
        unsigned fs = compile(GL_FRAGMENT_SHADER, FSH);
        shader_ = glCreateProgram();
        glAttachShader(shader_, vs);
        glAttachShader(shader_, fs);
        glLinkProgram(shader_);
        glDeleteShader(vs);
        glDeleteShader(fs);
    }

    void SpriteRenderer::initBuffers() {
        // единичный квадрик: два треугольника
        float quad[] = {
            -1.0f, -1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f,
            -1.0f,  1.0f
        };
        unsigned idx[] = { 0,1,2, 2,3,0 };

        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        // VBO дл€ квадрика + индексы
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quad) + sizeof(idx), nullptr, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad), quad);
        glBufferSubData(GL_ARRAY_BUFFER, sizeof(quad), sizeof(idx), idx);

        // EBO
        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

        // атрибуты квадрика (location=0)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

        glBindVertexArray(0);
    }

    unsigned SpriteRenderer::loadTexture(const std::string& path) {
        int w, h, ch;
        unsigned tex;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &ch, 4);
        if (!data) { std::cerr << "Failed to load texture: " << path << "\n"; return 0; }
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        return tex;
    }

    void SpriteRenderer::draw(const std::vector<IParticle*>& pts) {
        std::vector<float> inst;
        inst.reserve(pts.size() * 8);
        for (auto* p : pts) {
            auto& pos = p->position(); inst.push_back(pos.x); inst.push_back(pos.y);
            inst.push_back(p->size()); inst.push_back(0.0f);
            auto& c = p->color(); inst.push_back(c.r); inst.push_back(c.g); inst.push_back(c.b); inst.push_back(c.a);
        }

        glUseProgram(shader_);
        glBindVertexArray(vao_);
        glBindTexture(GL_TEXTURE_2D, texture_);

        unsigned instVBO;
        glGenBuffers(1, &instVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instVBO);
        glBufferData(GL_ARRAY_BUFFER, inst.size() * sizeof(float), inst.data(), GL_DYNAMIC_DRAW);

        // атрибуты инстанс-данных
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0));
        glVertexAttribDivisor(1, 1);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
        glVertexAttribDivisor(2, 1);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glVertexAttribDivisor(3, 1);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
        glVertexAttribDivisor(4, 1);

        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, (GLsizei)pts.size());
        glBindVertexArray(0);
        glDeleteBuffers(1, &instVBO);
    }
}