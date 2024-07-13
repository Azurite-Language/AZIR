#pragma once

#include <string>
#include <cstdio>
#include <unordered_map>
#include <string>
#include <stack>
#include "PoolManager.h"
#include "VariablesManager.h"
namespace StringManager
{

	struct AzuString {
		PoolManager::Entry allocated;
		VariablesManager::Variable variable;

	};
	static std::unordered_map<std::string, int> EntryList = std::unordered_map<std::string,int>();
	static std::stack<AzuString> entryStack = std::stack<AzuString>();


	static size_t size = 0;
	AzuString PushValue(std::string value);
	AzuString PushAddress(size_t address);
	AzuString PushVariable(VariablesManager::Variable variable);
	int GetCode(std::string entry);
	AzuString PopValue();
	AzuString PushFantom(size_t nbchar);
	const PoolManager::Entry EntryNull = { false, 0, 0, false };
}