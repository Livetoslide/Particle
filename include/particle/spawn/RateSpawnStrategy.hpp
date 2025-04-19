#pragma once
#include "ISpawnStrategy.hpp"

namespace patricle {

	class RateSpawnStrategy : public ISpawnStrategy {
		float rate_; //частиц в сек
		float accumulator = 0.0f;
	public:
		explicit RateSpawnStrategy(float rate) : rate_(rate) {}
		int spawnCount(float dt) override;
	};
}