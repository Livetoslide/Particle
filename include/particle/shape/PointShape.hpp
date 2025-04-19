#pragma once
#include "IEmitterShape.hpp"

namespace particle {
    class PointShape : public IEmitterShape {
    public:
        glm::vec2 sample() override { return glm::vec2(0.0f); }
    };
}