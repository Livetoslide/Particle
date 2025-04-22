#pragma once
#include "particle/IEmitter.hpp"
#include "particle/IRenderer.hpp"
#include "particle/spawn/ISpawnStrategy.hpp"
#include "particle/shape/IEmitterShape.hpp"
#include <vector>
#include <memory>

namespace particle {

    class Emitter : public IEmitter {
        std::vector<std::unique_ptr<IParticle>> particles_;
        std::vector<std::shared_ptr<IModule>>   modules_;
        std::shared_ptr<ISpawnStrategy>         spawnStrategy_;
        std::shared_ptr<IEmitterShape>          shape_;

        glm::vec2 baseVelocity_;
        float     maxLife_;        
    public:
        Emitter(std::shared_ptr<ISpawnStrategy> spawn,
            std::shared_ptr<IEmitterShape>  shape)
            : spawnStrategy_(std::move(spawn)), shape_(std::move(shape)) {
        }
        Emitter(std::shared_ptr<ISpawnStrategy> spawn,
            std::shared_ptr<IEmitterShape>  shape,
            const glm::vec2& baseVel,
            float                           maxLife = 3.0f)
            : spawnStrategy_(std::move(spawn))
            , shape_(std::move(shape))
            , baseVelocity_(baseVel)
            , maxLife_(maxLife)
        {
        }
        void spawn() override;
        void update(float dt) override;
        void render(IRenderer& renderer) override;
        void addModule(std::shared_ptr<IModule> module) override;
        size_t count() const { return particles_.size(); }

        const std::vector<std::unique_ptr<IParticle>>& getParticles() const { return particles_; }
    };

}
