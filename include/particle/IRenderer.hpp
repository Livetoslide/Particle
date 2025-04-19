#pragma once
#include "particle/IParticle.hpp"
#include <vector>

namespace particle {
	
	class IRenderer {
	public:
		virtual ~IRenderer() = default;
		virtual void draw(const std::vector<IParticle*>& particles) = 0;
	};
}