#pragma once
#include <stdio.h>
#include <cctype>
#include <cstdint>
#include <string>
#include <vector>


struct INDEX {
	uint32_t line;
	std::string name;
};

class IndexManager
{

public:
	std::vector<INDEX> index_list = {};

	INDEX SearchByName(std::string name);
	void SetIndex(std::string name, uint32_t line);
	int GetIndex(std::string name);
	INDEX GetByIndex(size_t index);
	bool IsRelative(std::string name);
	std::string RelativeToAbsolute(std::string relative);
	void PushIndex();
	void PopIndex();

private:
	std::vector<std::string> IndexStack = { "" };
	size_t Index_ptr = 0;
	
};

