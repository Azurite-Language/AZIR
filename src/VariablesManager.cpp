#include "VariablesManager.h"

int VariablesManager::GetVarIndex(std::string varname)
{

    int i = 0;
    for (; i < varlist.size() && varlist[i].name != varname; i++);

    if (i < varlist.size())
        return i;
        
    return -1;
}

VariablesManager::Variable VariablesManager::GetVarByIndex(int index)
{
    if (index < varlist.size())
        return varlist[index];
    return {true, "", 0 };
}

void VariablesManager::FlushVars()
{
    varlist = {};
}

void VariablesManager::AddVar(std::string varname)
{
    varlist.push_back({true ,varname,varlist.size() });

}

int VariablesManager::nbvars()
{
    return varlist.size();
}