#pragma once
#include "particle/IParticle.hpp"

namespace particle {
	class Particle : public IParticle {
		glm::vec2 pos_;
		glm::vec2 vel_;
		float		life_;
	public:
		Particle(const glm::vec2& pos, const glm::vec2& vel, float life);

		glm::vec2& position() override;
		glm::vec2& velocity() override;
		float&	   life()     override;
	};
}
