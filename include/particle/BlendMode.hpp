#pragma once
namespace particle {
	enum class BlendMode
	{
		Normal,     // SRC_ALPHA, ONE_MINUS_SRC_ALPHA
		Additive,   // SRC_ALPHA, ONE
		Multiply,   // DST_COLOR, ZERO
		Screen      // ONE_MINUS_DST_COLOR, ONE
	};
}