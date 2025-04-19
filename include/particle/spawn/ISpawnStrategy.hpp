#pragma once

namespace particle {

	class ISpawnStrategy {
	public: 
		virtual ~ISpawnStrategy() = default;
		// ���������� ����� ������, ������� ����� ���������� �� dt
		virtual int spawnCount(float dt) = 0;
	};
}