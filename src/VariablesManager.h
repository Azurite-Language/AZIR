#pragma once
#include <string>
#include <vector>
class VariablesManager
{
public:
	enum class VARIABLE_TYPE
	{
		STR,NUM
	};

	static struct Variable {
		bool valid;
		std::string name;
		uint32_t offset;
		VARIABLE_TYPE type;
		int pool_ptr;
	};

	int GetVarIndex(std::string varname);
	Variable GetVarByIndex(int index);
	void FlushVars();
	void AddVar(std::string varname);
	int nbvars();
	const Variable VarNull = { false, "", 0, VARIABLE_TYPE::NUM, 0 };
private:
	std::vector<Variable> varlist = {};

};

