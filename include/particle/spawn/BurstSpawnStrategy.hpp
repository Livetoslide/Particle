#pragma once
#include "ISpawnStrategy.hpp"
#include <vector>

namespace particle {

	struct Burst { float time; int count; };

	class BurstSpawnStrategy : public ISpawnStrategy {
		std::vector<Burst> bursts_;
		float elapsed_ = 0.0f;
	public:
		explicit BurstSpawnStrategy(std::vector<Burst> bursts)
			: bursts_(std::move(bursts)) { }
		int spawnCount(float dt) override;
	};
}