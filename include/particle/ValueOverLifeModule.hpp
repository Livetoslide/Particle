#pragma once
#include "IModule.hpp"
#include "IParticle.hpp"
#include <glm.hpp>
#include <vector>
#include <functional>

namespace particle {

	using EasingFunc = std::function<float(float)>;

	template<typename T>
	struct Keyframe
	{
		float time;
		T	  value;
		EasingFunc ease;
	};

	template<typename T>
	class ValueOverLifeModule : public IModule {
		std::vector<Keyframe<T>> keys_;
		float maxLife_;
	public:
		ValueOverLifeModule(std::vector<Keyframe<T>> keys, float maxLife)
			: keys_(std::move(keys)), maxLife_(maxLife) {
		}

		void apply(IParticle& p, float /*dt*/) override {
			float tNorm = 1.0f - glm::clamp(p.life(), 0.0f, 1.0f);

			if (keys_.empty()) return;
			if (tNorm <= keys_.front().time) {
				setValue(p, keys_.front().value);
				return;
			}
			if (tNorm >= keys_.back().time) {
				setValue(p, keys_.back().value);
				return;
			}
			size_t i = 0;
			while (i + 1 < keys_.size() && keys_[i + 1].time < tNorm) ++i;
			auto& K0 = keys_[i], & K1 = keys_[i + 1];
			float segmentT = (tNorm - K0.time) / (K1.time - K0.time);
			float e = K1.ease ? K1.ease(segmentT) : segmentT;
			setValue(p, interpolate(K0.value, K1.value, e));
		}
	private:
		void setValue(IParticle& p, const T& v);
		T interpolate(const T& a, const T& b, float t);
	};

	// Специализация для float (размер)
	template<>
	inline void ValueOverLifeModule<float>::setValue(IParticle& p, const float& v) {
		p.size() = v;
	}
	template<>
	inline float ValueOverLifeModule<float>::interpolate(
		const float& a, const float& b, float t
	) {
		return a + (b - a) * t;
	}

	// Специализация для glm::vec4 (цвет)
	template<>
	inline void ValueOverLifeModule<glm::vec4>::setValue(
		IParticle& p, const glm::vec4& v
	) {
		p.color() = v;
	}
	template<>
	inline glm::vec4 ValueOverLifeModule<glm::vec4>::interpolate(
		const glm::vec4& a, const glm::vec4& b, float t
	) {
		return glm::mix(a, b, t);
	}
}