#pragma once
#include <vector>
#include <cstdio>
#include <cstddef>
#include <cctype>


class PoolManager {

public:
	struct Entry {
		bool valid;
		size_t index;
		size_t size;
		bool used;
	};

	static Entry EntryNull;
	static Entry Allocate(size_t size);
	static void Free(size_t index);
	static Entry GetEntry(size_t index);
	static std::vector<Entry> Allocated;
};

// std::vector<PoolManager::Entry> PoolManager::Allocated;/* = std::vector<PoolManager::Entry>();*/