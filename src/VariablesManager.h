#pragma once
#include <string>
#include <vector>
#include <map>
class VariablesManager
{
public:
	enum class VARIABLE_TYPE
	{
		STR,NUM
	};

	struct Variable {
		bool valid;
		std::string name;
		uint32_t offset;
		VARIABLE_TYPE type;
		int pool_ptr;
	};

	int GetVarIndex(std::string varname);
	Variable GetVarByIndex(size_t index);
	void FlushVars();
	void AddVar(std::string varname);
	int nbvars();
	const Variable VarNull = { false, "", 0, VARIABLE_TYPE::NUM, 0 };
	void SetCurrentFunction(std::string name);
private:
	std::string currentFunction = "";
	std::map<std::string, std::vector<Variable>> variableContext = {};
};

