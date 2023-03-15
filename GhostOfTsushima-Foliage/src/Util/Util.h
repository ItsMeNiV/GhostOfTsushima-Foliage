#pragma once
#include "pch.h"
#include "World/Chunk.h"

namespace Util {

	class HashVec2
	{
	public:
		size_t operator()(const glm::ivec2& coord) const noexcept
		{
			return std::hash<int32_t>{}(coord.x) | std::hash<int32_t>{}(coord.y) << sizeof(int32_t) * 8;
		}
	};

	class HashChunk
	{
	public:
		size_t operator()(const Ref<Chunk> chunk) const noexcept
		{
			return std::hash<int32_t>{}(chunk->GetPosition().x) | std::hash<int32_t>{}(chunk->GetPosition().y) << sizeof(int32_t) * 8;
		}
	};

	class ChunkComparator
	{
	public:
		inline bool operator()(const Ref<Chunk> chunkA, const Ref<Chunk> chunkB) const noexcept
		{
			return chunkA->GetPosition().x == chunkB->GetPosition().x && chunkA->GetPosition().y == chunkB->GetPosition().y;
		}
	};

	namespace GlobalConfig
	{
		static constexpr uint32_t ChunkSize = 30;
		static constexpr uint32_t RenderTileSize = 10;
		static constexpr uint32_t RenderTilesPerChunkSide = ChunkSize / RenderTileSize;
	}
}