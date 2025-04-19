#pragma once
#include "ISpawnStrategy.hpp"

namespace particle {

	class RateSpawnStrategy : public ISpawnStrategy {
		float rate_; //частиц в сек
		float accumulator_ = 0.0f;
	public:
		explicit RateSpawnStrategy(float rate) : rate_(rate) {}
		int spawnCount(float dt) override;
	};
}