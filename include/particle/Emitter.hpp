#pragma once
#include "particle/IEmitter.hpp"
#include <vector>
#include <memory>

namespace particle {

    class Emitter : public IEmitter {
        std::vector<std::unique_ptr<IParticle>> particles_;
        std::vector<std::shared_ptr<IModule>>   modules_;
    public:
        Emitter();
        void spawn() override;
        void update(float dt) override;
        void render(IRenderer& renderer) override;
        void addModule(std::shared_ptr<IModule> module) override;
        size_t count() const { return particles_.size(); }

        const std::vector<std::unique_ptr<IParticle>>& getParticles() const { return particles_; }
    };

}
