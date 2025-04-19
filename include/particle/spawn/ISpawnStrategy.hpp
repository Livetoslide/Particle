#pragma once

namespace particle {

	class ISpawnStrategy {
	public: 
		virtual ~ISpawnStrategy() = default;
		// Возвращает число частиц, которые нужно заспавнить за dt
		virtual int spawnCount(float dt) = 0;
	};
}