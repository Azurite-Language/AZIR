#include "OPManager.h"

std::string OPManager::OP_GOTO(std::vector<std::string> lines, IndexManager *manager)
{
    std::string ret = "";
	

	int pos = manager->GetIndex(lines[1]);
	

	if (pos == -1)
		return ret;
	ret = "GOTO " + std::to_string(manager->GetByIndex(pos).line);
	return ret;
}

OPCODE::OPCODE OPManager::GetCode(std::string name)
{
	
	for (size_t i = 0; i < OPCODE::LAST; i++)
	{
		if (name == OPCODE::mnemoniques[i])
			return (OPCODE::OPCODE)i;
	}
	return OPCODE::INVALID;


}
