#pragma once
#include "particle/IParticle.hpp"
#include "particle/IModule.hpp"
#include "particle/IRenderer.hpp"
#include <vector>
#include <memory>

namespace particle {

	class IEmitter {
	public:
		virtual ~IEmitter() = default;
		virtual void spawn() = 0;
		virtual void update(float dt) = 0;
		virtual void render(class IRenderer& r) = 0;
		virtual void addModule(std::shared_ptr<IModule> m) = 0;
	};
}