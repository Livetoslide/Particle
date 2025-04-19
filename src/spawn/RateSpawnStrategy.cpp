#include "particle/spawn/RateSpawnStrategy.hpp"
#include <cmath>

namespace particle {

	int RateSpawnStrategy::spawnCount(float dt)
	{
		accumulator_ += rate_ * dt;
		int toSpawn = static_cast<int>(std::floor(accumulator_));
		accumulator_ -= toSpawn;
		return toSpawn;
	}
}