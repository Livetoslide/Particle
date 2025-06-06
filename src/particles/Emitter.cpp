#include "particle/Emitter.hpp"
#include "particle/Particle.hpp"
#include <algorithm>
#include <cstdlib>
#include <glm.hpp>

namespace particle {

    void Emitter::spawn() {
        float vx = (std::rand() % 100 - 50) / 50.0f;
        float vy = (std::rand() % 100)      / 100.0f;
        particles_.push_back(std::make_unique<Particle>(glm::vec2(0.0f), glm::vec2(vx, vy), 3.0f));
    }

    void Emitter::update(float dt) {
        // 1) �������
        int toSpawn = spawnStrategy_->spawnCount(dt);
        for (int i = 0; i < toSpawn; ++i) {
            glm::vec2 pos = shape_->sample();
            // �������� = ������� + (������ ����� � ������)
            particles_.push_back(std::make_unique<Particle>(
                pos,
                baseVelocity_,   // ������ ��������� ������� ��������
                maxLife_
            ));
        }

        // 2) ��� ������ ������� ��������� ������, ��������� life, ������
        for (auto& p : particles_) {
            for (auto& m : modules_)
                m->apply(*p, dt);
            p->life() -= dt;
            p->position() += p->velocity() * dt;
            p->angle() += p->angularVelocity() * dt;
        }

        // 3) ������� ��������
        particles_.erase(
            std::remove_if(particles_.begin(), particles_.end(),
                [](auto& p) { return p->life() <= 0.0f; }),
            particles_.end()
        );
    }

    void Emitter::render(IRenderer& r) {
        std::vector<IParticle*> batch;
        for (auto& u : particles_)
            batch.push_back(u.get());
        r.draw(batch);
    }

    void Emitter::addModule(std::shared_ptr<IModule> module) {
        modules_.push_back(module);
    }

}