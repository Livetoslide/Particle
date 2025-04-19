#include "particle/shape/BoxShape.hpp"
#include <glm.hpp>
#include <random>

namespace particle {

    glm::vec2 BoxShape::sample() {
        static thread_local std::mt19937 gen{ std::random_device{}() };
        std::uniform_real_distribution<float> ux(-width_ * 0.5f, width_ * 0.5f);
        std::uniform_real_distribution<float> uy(-height_ * 0.5f, height_ * 0.5f);
        return glm::vec2(ux(gen), uy(gen));
    }

}  // namespace particle