#pragma once
#include "IModule.hpp"
#include <glm.hpp>

namespace particle {
	class GravityModule : public IModule {
		glm::vec2 g_;
	public:
		explicit GravityModule(const glm::vec2& gravity = glm::vec2{0.0f, -9.8f}) 
			: g_(gravity) { }

		void apply(IParticle& p, float dt) override {
			p.velocity() += g_ * dt;
		}
	};
}