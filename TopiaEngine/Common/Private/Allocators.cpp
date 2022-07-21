#include "Allocators.h"

namespace topia
{
	BitSetAllocator::BitSetAllocator(size_t InCapacity, bool InbMultipleThreaded)
	    : bMultipleThreaded(InbMultipleThreaded)
	{
		Allocated.resize(InCapacity);
	}

	int BitSetAllocator::Allocate()
	{
		if (bMultipleThreaded)
			Mutex.lock();

		int Result = -1;
		int Capacity = static_cast<int>(Allocated.size());
		for (int i = 0; i < Capacity; ++i)
		{
			int ii = (NextAvailable + i) % Capacity;
			if (!Allocated[i])
			{
				Result = ii;
				NextAvailable = (ii + 1) % Capacity;
				Allocated[ii] = true;
				break;
			}
		}

		if (bMultipleThreaded)
			Mutex.unlock();

		return Result;
	}

	void BitSetAllocator::Release(int InIndex)
	{
		if (InIndex >= 0 && InIndex < static_cast<int>(Allocated.size()))
		{
			if (bMultipleThreaded)
				Mutex.lock();

			Allocated[InIndex] = false;
			NextAvailable = std::min(NextAvailable, InIndex);

			if (bMultipleThreaded)
				Mutex.unlock();
		}
	}
} // namespace topia
