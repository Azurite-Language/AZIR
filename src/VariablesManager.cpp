#include "VariablesManager.h"

int VariablesManager::GetVarIndex(std::string varname)
{

    size_t i = 0;
    for (; i < variableContext[currentFunction].size() && variableContext[currentFunction][i].name != varname; i++);

    if (i < variableContext[currentFunction].size())
        return i;
        
    return -1;
}

VariablesManager::Variable VariablesManager::GetVarByIndex(size_t index)
{
    if (index < variableContext[currentFunction].size())
        return variableContext[currentFunction][index];
    return {true, "", 0, VARIABLE_TYPE::NUM, 0};
}

void VariablesManager::FlushVars()
{
    variableContext[currentFunction] = {};
    currentFunction = "";
}

void VariablesManager::AddVar(std::string varname)
{
    variableContext[currentFunction].push_back({true, varname, 0, VARIABLE_TYPE::NUM, 0});
}

int VariablesManager::nbvars()
{
    return variableContext[currentFunction].size();
}

void VariablesManager::SetCurrentFunction(std::string name)
{
    currentFunction = name;
    variableContext[currentFunction] = {};
}
