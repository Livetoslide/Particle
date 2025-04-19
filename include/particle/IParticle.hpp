#pragma once
#include <glm.hpp>

namespace particle {

	struct IParticle {
		virtual ~IParticle() = default;
		virtual glm::vec2& position() = 0;
		virtual glm::vec2& velocity() = 0;
		virtual float& life() = 0;

		virtual glm::vec4& color() = 0;
		virtual float& size() = 0;
	};
}