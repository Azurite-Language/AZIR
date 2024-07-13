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
#include <filesystem>

#include <getopt.h>

using VMtype = int32_t;

// trim from start (in place)
static inline void ltrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
									{ return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
						 [](unsigned char ch)
						 { return !std::isspace(ch); })
				.base(),
			s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
	ltrim(s);
	rtrim(s);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s)
{
	trim(s);
	return s;
}

struct FLAGS
{
	VMtype param1;
	VMtype param2;
	VMtype modifier;
};

bool isSeparator(char c)
{
	return !isalnum(c) && c != '_';
}
// insert space before and after tokens
std::string insertSpace(std::string entry)
{
	std::string ret = "";

	for (char const c : entry)
	{

		if (isSeparator(c))
		{

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

std::vector<std::string> SplitByChar(std::string entry, char separator)
{

	std::vector<std::string> ret = {};
	std::string word = "";
	uint32_t pos = 0;

	for (char const &c : insertSpace(entry))
	{
		if (c == separator)
		{
			if (word != "")
				ret.push_back(word);
			word = "";
		}
		else
		{
			word += c;
		}
	}
	if (word != "")
		ret.push_back(word);
	return ret;
}

std::string GetString(std::vector<std::string> entry, size_t off)
{
	std::string ret = "";
	for (; off < entry.size() - 3; off++)
		ret += entry[off] + " ";
	ret += entry[off];

	return ret;
}

Operator::Operator GetOperator(std::string name)
{
	for (int fooInt = 0; fooInt != Operator::LAST; fooInt++)
	{
		if (name == Operator::OP_code[fooInt])
			return (Operator::Operator)fooInt;
	}

	return Operator::INVALID;
}

void GetOperation(Operator::Operator op, std::vector<std::string> entry, size_t &offset, VariablesManager *vmanager,
				  std::vector<std::string> *result)
{
	if (op == Operator::INVALID)
	{
		op = GetOperator(entry[offset]);
	}
	if (op == Operator::INVALID)
		throw;

	VMtype varindex1 = 0;
	VMtype parameters = 0;
	if (entry[1 + offset] == OPCODE::mnemoniques[OPCODE::POP])
	{
		offset += 2;
	}
	else if (entry[1 + offset] == OPCODE::mnemoniques[OPCODE::POP_S])
	{
		StringManager::AzuString entry = StringManager::PopValue();
		if (entry.variable.valid)
		{
			parameters |= 0b10;
			varindex1 = entry.variable.offset;
		}
		else
		{
			varindex1 = entry.allocated.index;
		}
		offset += 2;
		// parameters += 0x1 << 2;
	}
	else
	{
		varindex1 = vmanager->GetVarIndex(entry[1 + offset]);
		if (varindex1 == -1)
		{
			(*result).push_back("PUSH " + trim_copy(entry[1 + offset]));
			varindex1 = 0;
		}
		else
		{

			parameters |= 0b01;
		}
	}

	// Now parsing operand 2
	offset += 2;
	VMtype varindex2 = 0;
	if (entry[offset] == OPCODE::mnemoniques[OPCODE::POP])
	{
		offset += 2;
	}
	else if (entry[offset] == OPCODE::mnemoniques[OPCODE::POP_S])
	{

		StringManager::AzuString entry = StringManager::PopValue();
		if (entry.variable.valid)
		{
			parameters |= (0b10 << 2);
			varindex2 = entry.variable.offset;
		}
		else
		{
			varindex2 = entry.allocated.index;
		}
		offset += 2;
		// parameters += 0x2 << 2;
	}
	else
	{
		varindex2 = vmanager->GetVarIndex(entry[offset]);
		if (varindex2 == -1)
		{
			(*result).push_back("PUSH " + trim_copy(entry[offset]));
			varindex2 = 0;
		}
		else
		{

			parameters |= (0b01 << 2);
		}
		offset += 1;
	}

	VMtype flags[3] = {varindex1, varindex2, parameters};
	(*result).push_back(Operator::OP_trans[op] + " " + std::to_string(flags[0]) + " " + std::to_string(flags[1]) + " " + std::to_string(flags[2]));
}
static std::vector<std::string> data = std::vector<std::string>();

void parsePush(std::vector<std::string> tokens, size_t &i, std::vector<std::string> *ret, VariablesManager *vmanager)
{
	// Find the token to push, between parenthesis
	std::vector<std::string> tokensToPush(tokens.begin() + i + 2, std::find(tokens.begin() + i + 2, tokens.end(), ")"));
	i += 2;
	if (tokensToPush.size() == 1)
	{
		auto varindex = vmanager->GetVarIndex(tokensToPush[0]);
		if (varindex == -1)
		{
			ret->push_back("PUSH " + tokensToPush[0]);
		}
		else
		{
			ret->push_back("PUSH_RELATIVE " + std::to_string(varindex));
		}
		i += 1;
	}
	else
	{
		GetOperation(Operator::INVALID, tokens, i, vmanager, ret);
	}
}

void parsePushS(std::vector<std::string> tokens, size_t &i, std::vector<std::string> *ret, VariablesManager *vmanager)
{
	throw;
}

void parseInput(std::vector<std::string> tokens, size_t &i, std::vector<std::string> *ret, VariablesManager *vmanager)
{
	throw;
}

void parseGet(std::vector<std::string> tokens, size_t &i, std::vector<std::string> *ret, VariablesManager *vmanager)
{
	throw;
}

void parseGoto(std::vector<std::string> tokens, size_t &i, std::vector<std::string> *ret, IndexManager *manager)
{
	throw;
}

std::vector<std::string> parseTokens(std::vector<std::string> tokens, IndexManager &manager, VariablesManager &vmanager)
{
	std::vector<std::string> ret = {};
	for (size_t i = 0; i < tokens.size(); i++)
	{
		OPCODE::OPCODE op = OPManager::GetCode(tokens[i]);
		switch (op)
		{
		case OPCODE::DEFUN:
			manager.SetIndex(tokens[i + 1], ret.size() + 1);
			manager.PushIndex();
			i += 1;
			break;
		case OPCODE::SET:
			vmanager.AddVar(tokens[i + 1]);
			i += 1;
			break;
		case OPCODE::PUSH:
			parsePush(tokens, i, &ret, &vmanager);
			break;
		case OPCODE::POP:
			ret.push_back(OPCODE::mnemoniques[OPCODE::POP]);
			i += 2; // Skip parenthesis
			break;
		case OPCODE::GOTO:
			ret.push_back("GOTO " + tokens[i + 1]);
			i += 1;
			break;
		case OPCODE::GOTO_RELATIVE:
			ret.push_back("GOTO " + manager.RelativeToAbsolute(tokens[i + 1]));
			i += 1;
			break;
		case OPCODE::RELATIVE:
			manager.SetIndex(tokens[i + 1], ret.size() + 1);
			i += 1;
			break;
		case OPCODE::POP_CALLSTACK:
			ret.push_back("POP_CALLSTACK " + std::to_string(vmanager.nbvars()));
			vmanager.FlushVars();
			manager.PopIndex();
			i += 2;
			break;
		case OPCODE::JSR:
			ret.push_back("JSR " + tokens[i + 1]);
			i += 1;
			break;
		case OPCODE::RTS:
			ret.push_back("POP_CALLSTACK " + std::to_string(vmanager.nbvars()));
			i += 1;
			break;
		case OPCODE::PUSH_CALLSTACK:
			ret.push_back("PUSH_CALLSTACK");
			i += 2;
			// parseGoto(tokens, i, &ret, &manager);
			break;
		case OPCODE::IF:
			manager.PushIndex();
			ret.push_back("IFn");
			break;
		case OPCODE::ELSE:
			manager.SetIndex(OPCODE::mnemoniques[OPCODE::ELSE], ret.size() + 1);
			break;
		case OPCODE::THEN:
			ret.push_back("GOTO " + manager.RelativeToAbsolute("ENDIF"));
			manager.SetIndex(OPCODE::mnemoniques[OPCODE::THEN], ret.size() + 1);
			break;
		case OPCODE::ENDIF:
			manager.SetIndex(OPCODE::mnemoniques[OPCODE::ENDIF], ret.size() + 1);
			manager.PopIndex();
			break;
		case OPCODE::PUSH_S:
			parsePushS(tokens, i, &ret, &vmanager);
			break;
		case OPCODE::INPUT:
			parseInput(tokens, i, &ret, &vmanager);
			break;
		case OPCODE::GET:
			parseGet(tokens, i, &ret, &vmanager);
			break;
		default:
			break;
		}
	}

	return ret;
}

void UpdateEntrys(std::vector<std::string> *file_content, IndexManager *manager, std::vector<int> *to_decrypt)
{

	for (size_t i = 0; i < to_decrypt->size(); i++)
	{
		std::vector<std::string> lines = SplitByChar((*file_content)[(*to_decrypt)[i]], ' ');
		(*file_content)[(*to_decrypt)[i]] = OPManager::OP_GOTO(lines, manager);
	}
}

std::vector<std::vector<VMtype>> CompressLines(std::vector<std::string> entrys)
{
	std::vector<std::vector<VMtype>> result = {};
	for (size_t i = 0; i < entrys.size(); i++)
	{

		std::vector<std::string> lines = SplitByChar(entrys[i], ' ');
		if (lines.size() == 0)
			continue;

		result.push_back({(VMtype)OPManager::GetCode(lines[0])});
		if (result.back()[0] == OPCODE::INVALID)
			throw lines[0] + " not implemented";
		for (size_t j = 1; j < lines.size(); j++)
		{
			result.back().push_back(std::stol(lines[j]));
		}

		for (size_t j = result.back().size(); j < 4; j++)
		{
			result.back().push_back(0);
		}
	}

	return result;
}

void WriteString(std::string to_write, FILE *file)
{

	for (char x : to_write)
	{
		fwrite(&x, sizeof(char), 1, file);
	}
}

bool replace(std::string &str, const std::string &from, const std::string &to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

int main(int argc, char *argv[])
{

	const std::string usage = "Usage: " + std::string(argv[0]) + " [options] <input_file>\n"
							  "Options:\n"
							  "  -o, --output <output_file>  Output file\n"
							  "  --output-plaintext           Output plaintext file\n"
							  "  -h, --help                   Display this information\n";
	int output_plaintext = 0;

	static struct option long_options[] = {
		{"output", required_argument, 0, 'o'},
		{"help", no_argument, 0, 'h'},
		{"output-plaintext", no_argument, &output_plaintext, 1},
		{0, 0, 0, 0}};

	std::string output_file = "";
	
	int c;
	for (int i = 0; optind + i < argc; i++)
	{
		while ((c = getopt_long(argc, argv, "o:h", long_options, NULL)) != -1)
		{
			switch (c)
			{
			case 0:
			case 1:
				break;
			case 'o':
				output_file = optarg;
				break;
			case 'h':
				std::cout << usage << std::endl;
				return 1;
			case '?':
				std::cout << usage << std::endl;
				return 1;
			default:
				std::cout << usage << std::endl;
				return 1;
			}
		}	
	}

	if (optind >= argc)
	{
		std::cout << usage << std::endl;
		return 1;
	}
	std::string input_file = argv[optind];
	auto path = std::filesystem::path(input_file);
	if (output_file == "")
	{
		output_file = path.replace_extension(".byte").string();
	}

	std::string plain_text_file = path.replace_extension(".txt").string();


	std::ifstream input(input_file);
	if (!input.is_open())
	{
		std::cerr << "Unable to open file " << input_file << std::endl;
		return 1;
	}
	

	

	uint32_t linecount = 0;
	std::string line;
	std::ofstream myfile;
	std::vector<std::string> file_contents = {};
	std::vector<int> to_decrypt = {};
	std::vector<std::vector<VMtype>> mask = {};

	std::vector<std::string> inputTokens = {};

	std::string currentLine;
	while (std::getline(input, currentLine))
	{
		auto tokens = SplitByChar(currentLine, ' ');
		inputTokens.insert(inputTokens.end(), tokens.begin(), tokens.end());
	}

	IndexManager manager;
	VariablesManager vmanager;
	std::vector<std::string> plainTextIR = parseTokens(inputTokens, manager, vmanager);
	// Resolve relative adresses
	for (size_t i = 0; i < plainTextIR.size(); i++)
	{
		std::vector<std::string> lines = SplitByChar(plainTextIR[i], ' ');
		if (lines[0] == "GOTO" || lines[0] == "JSR")
		{
			plainTextIR[i] = lines[0] + " " + std::to_string(manager.SearchByName(lines[1]).line);
		}
	}

	
	if (output_plaintext)
	{
		myfile.open(plain_text_file);
		for (size_t i = 0; i < data.size(); i++)
		{
			replace(data[i], "\\n", "\n");
			myfile << data[i] + "\n";
		}

		for (size_t i = 0; i < plainTextIR.size(); i++)
			myfile << plainTextIR[i] + "\n";
		myfile.close();
	}

	plainTextIR.push_back("EXIT 0");
	std::vector<std::vector<VMtype>> kompressed = CompressLines(plainTextIR);

	FILE *compressed;
	if ((compressed = fopen(output_file.c_str(), "wb")) == NULL)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}
	else if (compressed != NULL)
	{
		for (size_t i = 0; i < data.size(); i++)
		{
			std::vector<std::string> lines = SplitByChar(data[i], ' ');
			OPCODE::OPCODE op = OPManager::GetCode(lines[0]);
			if (op == OPCODE::INVALID)
				throw lines[0] + " not implemented";
			else if (op == OPCODE::DC)
			{
				fwrite(&op, sizeof(VMtype), 1, compressed);
				VMtype size = std::stol(lines[1]);
				fwrite(&size, sizeof(VMtype), 1, compressed);
				for (size_t j = 2; j < lines.size() - 1; j++)
				{
					WriteString(lines[j] + " ", compressed);
				}
				WriteString(lines[lines.size() - 1], compressed);

				char empty = '\0';
				for (size_t j = (ftell(compressed) % (4 * sizeof(VMtype))); j < 4 * sizeof(VMtype); j++)
					fwrite(&empty, sizeof(char), 1, compressed);
			}
			else if (op == OPCODE::ALLOCATE)
			{
				fwrite(&op, sizeof(VMtype), 1, compressed);
				VMtype size = std::stol(lines[1]);
				fwrite(&size, sizeof(VMtype), 1, compressed);
				size = std::stol(lines[2]);
				fwrite(&size, sizeof(VMtype), 1, compressed);
				size = 0x0;
				fwrite(&size, sizeof(VMtype), 1, compressed);
			}
		}
		for (size_t i = 0; i < kompressed.size(); i++)
		{

			for (size_t j = 0; j < kompressed[i].size(); j++)
			{
				fwrite(&kompressed[i][j], sizeof(VMtype), 1, compressed);
			}
		}
	}

	return 0;
}