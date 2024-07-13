#include "StringManager.h"
std::vector<PoolManager::Entry> PoolManager::Allocated;/* = std::vector<PoolManager::Entry>();*/

StringManager::AzuString StringManager::PushValue(std::string value)
{
	PoolManager::Entry index;
	if (EntryList.find(value) == EntryList.end()) {
		index = PoolManager::Allocate(sizeof(std::string) * value.length());
		size++;
		EntryList[value] = index.index;
	}
	entryStack.push({ PoolManager::GetEntry(EntryList[value]), { false, "", 0, VariablesManager::VARIABLE_TYPE::STR, 0}});
	// StackManager::PushStack("", 0, VariablesManager::VARIABLE_TYPE::STR, EntryList[value]);
	return entryStack.top();
}

StringManager::AzuString StringManager::PushAddress(size_t address)
{
	return { PoolManager::GetEntry(address), { false, "", 0, VariablesManager::VARIABLE_TYPE::NUM, 0 }};
	// return PoolManager::Entry();
}

StringManager::AzuString StringManager::PushVariable(VariablesManager::Variable variable)
{
	StringManager::AzuString  val = {EntryNull, variable};
	entryStack.push(val);
	size++;
	return val;
}

int StringManager::GetCode(std::string entry)
{
	if (EntryList.find(entry) == EntryList.end())
		return -1;
	return EntryList[entry];
}

StringManager::AzuString StringManager::PopValue()
{
	StringManager::AzuString val = entryStack.top();
	entryStack.pop();
	return val;
}



StringManager::AzuString StringManager::PushFantom(size_t nbchar)
{
	/*PoolManager::Allocate(nbchar * sizeof(char));*/
	PoolManager::Entry entry= PoolManager::Allocate(nbchar * sizeof(char));
	size++;
	entryStack.push({ entry, { false, "", 0, VariablesManager::VARIABLE_TYPE::NUM, 0 } });
	return entryStack.top();
}
