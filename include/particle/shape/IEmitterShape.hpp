#pragma once
#include <glm.hpp>

namespace particle {

	class IEmitterShape {
	public:
		virtual	~IEmitterShape() = default;
		virtual glm::vec2 sample() = 0;
	};
}