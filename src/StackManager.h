#pragma once 
#include <stack>
#include "VariablesManager.h"
#include <string>

class StackManager
{

public:
	static void PushStack(std::string name, uint32_t offset, VariablesManager::VARIABLE_TYPE type, int pool_ptr);
	static VariablesManager::Variable PopStack();
	//static VariablesManager::Variable GetRelative(size_t off);



private:
	static std::stack<VariablesManager::Variable> stack;

};

