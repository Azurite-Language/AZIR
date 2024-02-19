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
#include "OPManager.h"
#include "VariablesManager.h"
#include "main.h"
#include "StringManager.h"
using VMtype = int32_t;

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		[](unsigned char ch) { return !std::isspace(ch); })
		.base(),
		s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
	trim(s);
	return s;
}

struct FLAGS {
	VMtype param1;
	VMtype param2;
	VMtype modifier;
};

std::string Add_space(std::string entry) {
	std::string ret = "";

	for (char const c : entry) {

		if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c=='\\')) {

			ret += ' ';
			ret += c;
			ret += ' ';
		}
		else
		{
			ret += c;

		}

	}

	return ret;
}


std::vector<std::string> SplitByChar(std::string entry, char separator) {

	std::vector<std::string> ret = {};
	std::string word = "";
	uint32_t pos = 0;

	for (char const& c : Add_space(entry)) {
		if (c == separator) {
			if (word != "")
				ret.push_back(word);
			word = "";
		}
		else {

			word += c;
		}
	}
	if (word != "")
		ret.push_back(word);
	return ret;
}

std::string GetString(std::vector<std::string> entry, int off) {
	std::string ret = "";
	for (; off < entry.size() - 3; off++)
		ret += entry[off] + " ";
	ret += entry[off];

	return ret;
}

Operator::Operator GetOperator(std::string name) {
	for (int fooInt = 0; fooInt != Operator::LAST; fooInt++)
	{
		if (name == Operator::OP_code[fooInt])
			return (Operator::Operator)fooInt;
	}

	return Operator::INVALID;
}



void GetOperation(Operator::Operator op, std::vector<std::string> entry, VariablesManager* vmanager, int pos,
	std::vector<std::string>* result) {


	VMtype varindex1 = 0;
	VMtype parameters = 0;
	size_t offset = 0;
	if (entry[pos + 1] == OPCODE::mnemoniques[OPCODE::POP]) {
		offset += 2;
	}
	else if (entry[pos + 1] == OPCODE::mnemoniques[OPCODE::POP_S]) {
		StringManager::AzuString entry = StringManager::PopValue();
		if (entry.variable.valid) {
			parameters += 0x1;
			varindex1 = entry.variable.offset;
		}
		else {
			varindex1 = entry.allocated.index;
		}
		offset += 2;
		// parameters += 0x1 << 2;
	}
	else {
		varindex1 = vmanager->GetVarIndex(entry[pos + 1]);
		if (varindex1 == -1) {
			(*result).push_back("PUSH " + trim_copy(entry[pos + 1]));
			varindex1 = 0;
		}
		else {

		parameters = 0x2;
		}
	}

	VMtype varindex2 = 0;
	if (entry[pos + offset + 2] == OPCODE::mnemoniques[OPCODE::POP]) {
		offset += 2;
	}
	else if (entry[pos + offset + 2] == OPCODE::mnemoniques[OPCODE::POP_S]) {


		StringManager::AzuString entry = StringManager::PopValue();
		if (entry.variable.valid) {
			parameters += 0x2;
			varindex2 = entry.variable.offset;
		}
		else {
			varindex2 = entry.allocated.index;
		}

		// parameters += 0x2 << 2;
	}
	else {
		varindex2 = vmanager->GetVarIndex(entry[pos + offset + 2]);
		if (varindex2 == -1) {
			(*result).push_back("PUSH " + trim_copy(entry[pos + offset + 2]));
			varindex2 = 0;
		}
		else {

		parameters += 0x1;
		}
	}

	VMtype flags[3] = { varindex1, varindex2, parameters };

	if (op == Operator::INVALID) {
		op = GetOperator(entry[pos]);
	}
	if (op == Operator::INVALID)
		throw;
	(*result).push_back(Operator::OP_trans[op] + " " + std::to_string(flags[0]) + " " + std::to_string(flags[1]) + " " + std::to_string(flags[2]));

}
static std::vector<std::string> data = std::vector<std::string>();
bool ParseLine(std::string entry, IndexManager* manager, VariablesManager* vmanager, std::vector<std::string>* ret) {

	std::vector<std::string> lines = SplitByChar(entry, ' ');

	bool IsParsed = true;
	std::string result;
	std::vector<VMtype> flags;
	OPCODE::OPCODE entry_code = OPManager::GetCode(lines[0]);
	StringManager::AzuString address;
	PoolManager::Entry PoolEntry;
	switch (entry_code)
	{
	case OPCODE::DEFUN:
		manager->SetIndex(lines[1], ret->size() + 1);
		manager->PushIndex();
		break;

	case OPCODE::SET:
		vmanager->AddVar(lines[1]);
		break;

	case OPCODE::PUSH:
		if (lines.size() > 4)
			GetOperation(Operator::INVALID, lines, vmanager, 2, ret);
		else {
			ret->push_back("PUSH " + lines[2]);
		}
		break;

	case OPCODE::POP:
		ret->push_back(OPCODE::mnemoniques[OPCODE::POP]);
		break;

	case OPCODE::GOTO:
		result = OPManager::OP_GOTO(lines, manager);
		if (result.size() == 0) {
			result = "GOTO ";
			if (lines.size() > 2)
				result += manager->RelativeToAbsolute(lines[2]);
			else
				result += lines[1];
			ret->push_back(result);
			IsParsed = false;
		}
		else
		{
			ret->push_back(result);

		}

		break;

	case OPCODE::RELATIVE:
		manager->SetIndex(lines[1], ret->size() + 1);
		break;
	case OPCODE::RTS:
		ret->push_back("POP_CALLSTACK " + std::to_string(vmanager->nbvars()));
		vmanager->FlushVars();
		manager->PopIndex();
		break;
	case OPCODE::JSR:
		ret->push_back("PUSH_CALLSTACK " + lines[2]);
		result = OPManager::OP_GOTO(lines, manager);

		if (result == "") {
			ret->push_back("GOTO " + lines[1]);
			IsParsed = false;
		}
		else
			ret->push_back(result);
		break;
	case OPCODE::IF:
		manager->PushIndex();
		ret->push_back("IFn");
		ret->push_back("GOTO " + manager->RelativeToAbsolute("ELSE"));
		IsParsed = false;
		break;


	case OPCODE::ELSE:
		ret->push_back("GOTO " + manager->RelativeToAbsolute("ENDIF"));
		manager->SetIndex(OPCODE::mnemoniques[OPCODE::ELSE], ret->size() + 1);
		IsParsed = false;
		break;


	case OPCODE::THEN:
		manager->SetIndex(OPCODE::mnemoniques[OPCODE::THEN], ret->size() + 1);
		break;

	case OPCODE::ENDIF:
		manager->SetIndex(OPCODE::mnemoniques[OPCODE::ENDIF], ret->size() + 1);
		manager->PopIndex();
		break;
	case OPCODE::PUSH_S:
		if (vmanager->GetVarIndex(lines[2]) != -1) {
			address = StringManager::PushVariable(vmanager->GetVarByIndex(vmanager->GetVarIndex(lines[2])));
		}
		else {
			address = StringManager::PushValue(GetString(lines, 3));
			// data.push_back("ALLOCATE " + std::to_string(address.allocated.size) + " " + std::to_string(address.allocated.index));
			data.push_back("DC " + std::to_string(address.allocated.index) + " " + GetString(lines, 3));
			ret->push_back("PUSHA " + std::to_string(address.allocated.index));
		}
		break;
	case OPCODE::INPUT: {
		address = StringManager::PopValue();
		result = "INPUT ";
		VMtype flags = 0;
		if (address.allocated.valid) {
			flags + 0x2;
			result += std::to_string(address.variable.offset);
		}
		else {
			result += std::to_string(address.allocated.index);
		}

		// result = "INPUT " + std::to_string(StringManager::PopValue());
	//	address = StringManager::PushFantom(200);
	//	data.push_back("ALLOCATE 200 " + std::to_string(address.allocated.index));
		ret->push_back(result + " " + std::to_string(flags));
	}break;
	case OPCODE::GET:

		GetOperation(Operator::GET, lines, vmanager, 0, ret);
		
		// address = StringManager::PushFantom(200);
		// data.push_back("ALLOCATE 200 " + std::to_string(address.allocated.index));
		// ret->push_back("GET " +lines[0] +std::to_string(flags) + " " );

		//ret->push_back(OPCODE::mnemoniques[OPCODE::GET] + " " + std::to_string(StringManager::PopValue().variable->offset) + lines[lines.size()-1]);
		break;
	case OPCODE::INVALID:
	case OPCODE::LAST:
		manager->SetIndex(lines[0], ret->size() + 1);
		break;
	default:
		ret->push_back(entry);
		break;
	}

	return IsParsed;
}


std::vector<std::string> UnSaltEntrys(std::vector<std::string> file_contents, IndexManager* manager, VariablesManager* vmanager, std::vector<int>* to_decrypt) {

	std::vector<std::string> ret = {};
	std::vector<std::string> lines;
	for (size_t i = 0; i < file_contents.size(); i++) {
		if (file_contents[i] == "")
			continue;
		if (!ParseLine(file_contents[i], manager, vmanager, &ret)) {
			(*to_decrypt).push_back(ret.size() - 1);
		}
	}
	return ret;
}

void UpdateEntrys(std::vector<std::string>* file_content, IndexManager* manager, std::vector<int>* to_decrypt) {

	for (size_t i = 0; i < to_decrypt->size(); i++) {
		std::vector<std::string> lines = SplitByChar((*file_content)[(*to_decrypt)[i]], ' ');
		(*file_content)[(*to_decrypt)[i]] = OPManager::OP_GOTO(lines, manager);
	}
}


std::vector<std::vector<VMtype>> CompressLines(std::vector<std::string> entrys) {
	std::vector<std::vector<VMtype>> result = {};
	for (size_t i = 0; i < entrys.size(); i++) {

		std::vector<std::string> lines = SplitByChar(entrys[i], ' ');


		result.push_back({ (VMtype)OPManager::GetCode(lines[0]) });
		if (result[i][0] == OPCODE::INVALID)
			throw lines[0] + " not implemented";
		for (size_t j = 1; j < lines.size(); j++) {
			result[i].push_back(std::stol(lines[j]));
		}

		for (size_t j = result[i].size(); j < 4; j++) {
			result[i].push_back(0);
		}

	}

	return result;
}

void WriteString(std::string to_write, FILE* file) {

	for (char x : to_write)
	{
		fwrite(&x, sizeof(char), 1, file);
	}
}


bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}


int main() {
	uint32_t linecount = 0;
	std::string line;
	std::ifstream input("test.txt");
	std::ofstream myfile;
	std::vector<std::string> file_contents = {};
	std::vector<int> to_decrypt = {};
	std::vector<std::vector<VMtype>> mask = {};


	if (input.is_open()) {
		while (std::getline(input, line)) {
			line = trim_copy(line);
			file_contents.push_back(line);
		}
		input.close();
	}


	IndexManager manager;
	VariablesManager vmanager;
	std::vector<std::string> unsalted = UnSaltEntrys(file_contents, &manager, &vmanager, &to_decrypt);
	myfile.open("example.byte");
	UpdateEntrys(&unsalted, &manager, &to_decrypt);
	for (size_t i = 0; i < data.size(); i++) {
		replace(data[i], "\\n", "\n");
		myfile << data[i] + "\n";
	}

	for (size_t i = 0; i < unsalted.size(); i++)
		myfile << unsalted[i] + "\n";
	unsalted.push_back("EXIT 0");
	std::vector<std::vector<VMtype>> kompressed = CompressLines(unsalted);

	errno_t err;
	FILE* compressed;


	if ((err = fopen_s(&compressed, "output.byte", "wb")) != 0) {
		//fprintf(stderr, "cannot open file '%s': %s\n",
		//	compressed, strerror_s(err));
	}
	else if (compressed != NULL) {
		//for(size_t i = 0)
		for (size_t i = 0; i < data.size(); i++) {
			std::vector<std::string> lines = SplitByChar(data[i], ' ');
			OPCODE::OPCODE op = OPManager::GetCode(lines[0]);
			if (op == OPCODE::INVALID)
				throw lines[0] + " not implemented";
			else if (op == OPCODE::DC) {
				fwrite(&op, sizeof(VMtype), 1, compressed);
				VMtype size = std::stol(lines[1]);
				fwrite(&size, sizeof(VMtype), 1, compressed);
				for (size_t j = 2; j < lines.size() - 1; j++) {
					WriteString(lines[j] + " ", compressed);
				}
				WriteString(lines[lines.size()-1], compressed);

				char empty = '\0';
				for (size_t j = (ftell(compressed) % (4 * sizeof(VMtype))); j < 4 * sizeof(VMtype); j++)
					fwrite(&empty, sizeof(char), 1, compressed);
			}
			else if (op == OPCODE::ALLOCATE) {
				fwrite(&op, sizeof(VMtype), 1, compressed);
				VMtype size = std::stol(lines[1]);
				fwrite(&size, sizeof(VMtype), 1, compressed);
				size = std::stol(lines[2]);
				fwrite(&size, sizeof(VMtype), 1, compressed);
				size = 0x0;
				fwrite(&size, sizeof(VMtype), 1, compressed);
			}

		}
		for (size_t i = 0; i < kompressed.size(); i++) {

			for (size_t j = 0; j < kompressed[i].size(); j++) {
				fwrite(&kompressed[i][j], sizeof(VMtype), 1, compressed);
			}

		}


	};

	//for (size_t i = 0; i < unsalted.size(); i++) {
	//	for ()


	//}


	myfile.close();

	return 0;
}