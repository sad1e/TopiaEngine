#pragma once

#include <vector>
#include <mutex>

namespace topia
{
	class BitSetAllocator
	{
	public:
		explicit BitSetAllocator(size_t InCapacity, bool InbMultipleThreaded);

		BitSetAllocator(const BitSetAllocator&) = delete;
		BitSetAllocator(BitSetAllocator&&) = delete;

		int Allocate();
		void Release(int InIndex);

	private:
		std::vector<bool> Allocated;
		std::mutex Mutex;

		int NextAvailable = 0;
		bool bMultipleThreaded;
	};
} // namespace topia
