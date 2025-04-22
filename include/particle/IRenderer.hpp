#pragma once
#include "particle/IParticle.hpp"
#include "particle/BlendMode.hpp"
#include <vector>

namespace particle {
	
	class IRenderer {
	public:
		virtual ~IRenderer() = default;
		virtual void draw(const std::vector<IParticle*>& particles) = 0;
		virtual void setBlendMode(BlendMode mode) = 0;
	};
}