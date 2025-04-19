#pragma once
#include "particle/IParticle.hpp"

namespace particle {
	class Particle : public IParticle {
		glm::vec2 pos_;
		glm::vec2 vel_;
		float		life_;
		glm::vec4 color_;
		float size_;
	public:
		Particle(const glm::vec2& pos,
				 const glm::vec2& vel, 
				 float life,
			     const glm::vec4& col = glm::vec4{ 1,1,1,1 },
				 float sz = 5.0f);

		glm::vec2& position() override;
		glm::vec2& velocity() override;
		float&	   life()     override;
		glm::vec4& color()    override;
		float&     size()     override;
	};
}
