#include "particle/shape/CircleShape.hpp"
#include <glm.hpp>
#include <gtc/constants.hpp>
#include <cmath>
#include <random>

namespace particle {

    glm::vec2 CircleShape::sample() {
        static thread_local std::mt19937 gen{ std::random_device{}() };
        std::uniform_real_distribution<float> u01(0.0f, 1.0f);

        float u = u01(gen);
        float r = radius_ * std::sqrt(u);
        float theta = u01(gen) * glm::two_pi<float>();

        return {
            r * std::cos(theta),
            r * std::sin(theta)
        };
    }

} // namespace particle