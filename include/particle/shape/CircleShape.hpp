#pragma once
#include "IEmitterShape.hpp"

namespace particle {
    class CircleShape : public IEmitterShape {
        float radius_;
    public:
        explicit CircleShape(float r) : radius_(r) {}
        glm::vec2 sample() override;
    };
}