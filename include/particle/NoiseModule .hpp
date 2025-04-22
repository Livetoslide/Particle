#pragma once
#include "IModule.hpp"
#include <random>
#include <glm.hpp>

namespace particle {


	class NoiseModule : public IModule {
		float magnitude_;
		std::mt19937 gen_;
		std::uniform_real_distribution<float> dist_;

	public:
		explicit NoiseModule(float magnitude)
			: magnitude_(magnitude)
			, gen_(std::random_device{}())
			, dist_(-1.0f, 1.0f)
		{ }

		void apply(IParticle& p, float dt) override {
			float rx = dist_(gen_);
			float ry = dist_(gen_);

			glm::vec2 offs{ rx,ry };
			if (glm::length(offs) > 0.0f) {
				offs = glm::normalize(offs);
			}
			p.position() += offs * (magnitude_ * dt);
		}
	};
}