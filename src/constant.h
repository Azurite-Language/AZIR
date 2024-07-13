#pragma once
#include <string>

namespace OPCODE {
	enum OPCODE { EXIT = 0, PUSH, POP, DEFUN, SET, GOTO,GOTO_RELATIVE, RELATIVE, RTS, JSR, IF, IFn, ENDIF, ELSE, THEN, EQUAL, NEQUAL, PLUS, MINUS, DIV, MULT, LT, PUSH_CALLSTACK, POP_CALLSTACK, INPUT, PUSH_S,POP_S,ALLOCATE,DC,PUSHA,GET,LAST, INVALID };
	const std::string mnemoniques[] = { "EXIT", "PUSH", "POP", "defun", "set", "GOTO", "GOTO_RELATIVE", "/", "rts", "JSR","IF","IFn","ENDIF", "ELSE", "THEN","EQ","NEQ","ADD","SUB","DIV","MULT","LT", "PUSH_CALLSTACK","POP_CALLSTACK","INPUT","PUSH_S", "POP_S","ALLOCATE","DC","PUSHA", "GET" };
}

namespace Operator {

	enum Operator { EQUAL = 0, NEQUAL, PLUS, MINUS, DIV, MULT, INF,GET,LT, LAST, INVALID };
	const std::string OP_code[] = { "EQ","NEQ" ,"ADD", "SUB","DIV","MULT", "INF", "GET", "LT" };
	const std::string OP_trans[] = { "EQ","NEQ","ADD","SUB","DIV","MULT","INF", "GET", "LT" };

}
