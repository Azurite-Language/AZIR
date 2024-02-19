#include "StackManager.h"

static std::stack<VariablesManager::Variable> Stack;
void StackManager::PushStack(std::string name, uint32_t offset, VariablesManager::VARIABLE_TYPE type, int pool_ptr)
{
	Stack.push({true, name, offset, type, pool_ptr });
}


VariablesManager::Variable StackManager::PopStack()
{
	VariablesManager::Variable var = Stack.top();
	Stack.pop();
	return  var;
}

//VariablesManager::Variable StackManager::GetRelative(size_t pos) {
//
//	
//
//}