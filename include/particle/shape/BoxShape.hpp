#pragma once
#include "IEmitterShape.hpp"

namespace particle {
    class BoxShape : public IEmitterShape {
        float width_, height_;
    public:
        BoxShape(float w, float h) : width_(w), height_(h) {}
        glm::vec2 sample() override;
    };
}