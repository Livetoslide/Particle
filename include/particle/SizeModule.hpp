#pragma once
#include "IModule.hpp"

namespace particle {

    class SizeModule : public IModule {
        float startSize_, endSize_, maxLife_;
    public:
        SizeModule(float startSize,
            float endSize,
            float maxLife)
            : startSize_(startSize), endSize_(endSize), maxLife_(maxLife) {
        }

        void apply(IParticle& p, float /*dt*/) override {
            float t = glm::clamp(p.life() / maxLife_, 0.0f, 1.0f);
            p.size() = endSize_ + (startSize_ - endSize_) * t;
        }
    };

} // namespace particle
