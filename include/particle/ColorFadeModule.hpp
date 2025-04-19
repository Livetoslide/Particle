#pragma once
#include "IModule.hpp"
#include <glm.hpp>

namespace particle {

	class ColorFadeModule : public IModule {
		glm::vec4 start_, end_;
		float maxLife_;
	public:
		ColorFadeModule(const glm::vec4& startColor,
			const glm::vec4& endColor,
			float maxLife)
			: start_(startColor), end_(endColor), maxLife_(maxLife) {
		}
		void apply(IParticle& p, float /*dt*/) override {
			float t = glm::clamp(p.life() / maxLife_, 0.0f, 1.0f);
			p.color() = glm::mix(end_, start_, t);
		}
	};
}