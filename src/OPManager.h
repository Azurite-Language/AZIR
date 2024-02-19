#pragma once


#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <locale>
#include <stdio.h>
#include <vector>
#include <string>
#include "IndexManager.h"
#include "constant.h"
class OPManager
{


public:
	std::string static OP_GOTO(std::vector<std::string> lines, IndexManager *manager);
	std::vector<std::string> OP_PUSH(std::string line);
	OPCODE::OPCODE static GetCode(std::string name);




};

