#pragma once
#include "particle/IParticle.hpp"

namespace particle {

	class IModule
	{
	public:
		virtual ~IModule() = default;
		virtual void apply(IParticle& p, float dt) = 0;
	};
}