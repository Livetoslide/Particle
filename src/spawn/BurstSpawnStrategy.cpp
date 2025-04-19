#include "particle/spawn/BurstSpawnStrategy.hpp"

namespace particle {

    int BurstSpawnStrategy::spawnCount(float dt) {
        float old = elapsed_;
        elapsed_ += dt;
        int total = 0;
        for (auto& b : bursts_) {
            if (b.time > old && b.time <= elapsed_) {
                total += b.count;
            }
        }
        return total;
    }

} // namespace particle
