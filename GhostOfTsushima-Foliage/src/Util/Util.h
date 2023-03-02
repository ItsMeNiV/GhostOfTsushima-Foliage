#pragma once
#include "pch.h"

namespace Util {

	class HashVec2
	{
	public:
		size_t operator()(const glm::ivec2& coord) const noexcept
		{
			return std::hash<int32_t>{}(coord.x) | std::hash<int32_t>{}(coord.y) << sizeof(int32_t) * 8;
		}
	};

}