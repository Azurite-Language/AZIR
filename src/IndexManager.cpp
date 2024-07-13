#include "IndexManager.h"
#include <string>
INDEX IndexManager::SearchByName(std::string name)
{
	size_t i = 0;
	for (; i < index_list.size() && index_list[i].name != name; i++);
	if (i == index_list.size())
		return{ 0,"" };

	return index_list[i];
}

void IndexManager::SetIndex(std::string name, uint32_t line)
{
	
	IndexStack[Index_ptr] = name;
	std::string s;
	for (size_t i = 0; i <= Index_ptr; i++) s += IndexStack[i];
	index_list.push_back({ line, s});
}

int IndexManager::GetIndex(std::string name)
{
	size_t i = 0;
	for (; i < index_list.size() && index_list[i].name != name; i++);

	if (i < index_list.size())
		return i;
	return -1;
}

INDEX IndexManager::GetByIndex(size_t index)
{
	if (index < index_list.size())
		return index_list[index];
	return{ 0,"" };
}

bool IndexManager::IsRelative(std::string name)
{
	return name[0] == '/';
}

std::string IndexManager::RelativeToAbsolute(std::string relative)
{
	std::string s;
	for (size_t i = 0; i < Index_ptr;i++) s += IndexStack[i];
	return s + relative;
		//IndexStack[Index_ptr] + relative;
}

void IndexManager::PushIndex()
{
	if (Index_ptr == IndexStack.size() - 1)
		IndexStack.push_back("");
	Index_ptr++;
}

void IndexManager::PopIndex()
{
	Index_ptr -= (Index_ptr > 0);
}
