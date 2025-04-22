#pragma once
#include "IModule.hpp"
#include <random>

namespace particle {

	class AngularVelocityModule : public IModule {
		float maxAngVel_;
		std::mt19937 gen_;
		std::uniform_real_distribution<float> dist_;
	public:
		explicit AngularVelocityModule(float maxAngVel)
			: maxAngVel_(maxAngVel)
			, gen_(std::random_device{}())
			, dist_(-maxAngVel, maxAngVel)
		{ }

		void apply(IParticle& p, float /*dt*/) override {
		
			if (p.angularVelocity() == 0.0f) {
				p.angularVelocity() = dist_(gen_);
			}
		}
	};
}