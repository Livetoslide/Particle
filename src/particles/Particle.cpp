#include "particle/Particle.hpp"

namespace particle {

	Particle::Particle(const glm::vec2& pos,
		const glm::vec2& vel,
		float life,
		const glm::vec4& col,
		float sz)
		: pos_(pos), vel_(vel), life_(life), color_(col), size_(sz)
	{
	}

	glm::vec2& Particle::position() { return pos_; }
	glm::vec2& Particle::velocity() { return vel_; }
	float& Particle::life() { return life_; }
	glm::vec4& Particle::color() { return color_; }
	float& Particle::size() { return size_; }
}