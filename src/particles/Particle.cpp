#include "particle/Particle.hpp"

namespace particle {
	Particle::Particle(const glm::vec2& pos, const glm::vec2& vel, float life)
		: pos_(pos), vel_(vel), life_(life) { }

	glm::vec2& Particle::position() { return pos_; }
	glm::vec2& Particle::velocity() { return vel_; }
	float&     Particle::life()     { return life_; }
}