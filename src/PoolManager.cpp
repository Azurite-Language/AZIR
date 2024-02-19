#include "PoolManager.h"



static PoolManager::Entry EntryNull = { false, 0, 0, false };
PoolManager::Entry PoolManager::Allocate(size_t size)
{
    Allocated.push_back({true ,Allocated.size(), size, true });
    return Allocated[Allocated.size()-1];
}

void PoolManager::Free(size_t index)
{
    Allocated[index].used = false;
}

PoolManager::Entry PoolManager::GetEntry(size_t index)
{
    return Allocated[index];
}
