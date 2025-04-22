#include "particle/Particle.hpp"

namespace particle {

	glm::vec2& Particle::position() { return pos_; }
	glm::vec2& Particle::velocity() { return vel_; }
	float& Particle::life() { return life_; }
	glm::vec4& Particle::color() { return color_; }
	float& Particle::size() { return size_; }
	float& Particle::angle()            { return angle_; }
	float& Particle::angularVelocity()  { return angularVelocity_; }
}