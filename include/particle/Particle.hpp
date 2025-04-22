#pragma once
#include "particle/IParticle.hpp"

namespace particle {
	class Particle : public IParticle {
		glm::vec2 pos_;
		glm::vec2 vel_;
		float		life_;
		glm::vec4 color_;
		float size_;
		float angle_ = 0.0f;
		float angularVelocity_ = 0.0f;
	public:
		Particle(const glm::vec2& pos,
			const glm::vec2& vel,
			float life,
			const glm::vec4& col = { 1,1,1,1 },
			float sz = 5.0f,
			float ang = 0.0f,
			float angVel = 0.0f)
			: pos_(pos), vel_(vel), life_(life),
			color_(col), size_(sz),
			angle_(ang), angularVelocity_(angVel)
		{
		}

		glm::vec2& position() override;
		glm::vec2& velocity() override;
		float&	   life()     override;
		glm::vec4& color()    override;
		float&     size()     override;
		float& angle()           override;
		float& angularVelocity() override;
	};
}
