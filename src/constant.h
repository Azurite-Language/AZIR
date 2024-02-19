#pragma once
#include <string>

namespace OPCODE {
	enum OPCODE { EXIT = 0, PUSH, POP, DEFUN, SET, GOTO, RELATIVE, RTS, JSR, IF, IFn, ENDIF, ELSE, THEN, EQUAL, NEQUAL, PLUS, MINUS, DIV, MULT, INF, PUSH_CALLSTACK, POP_CALLSTACK, INPUT, PUSH_S,POP_S,ALLOCATE,DC,PUSHA,GET,LAST, INVALID };
	const std::string mnemoniques[] = { "EXIT", "PUSH", "POP", "defun", "set", "GOTO", "/", "rts", "jsr","IF","IFn","ENDIF", "ELSE", "THEN","EQ","NEQ","ADD","SUB","DIV","MULT","INF", "PUSH_CALLSTACK","POP_CALLSTACK","INPUT","PUSH_S", "POP_S","ALLOCATE","DC","PUSHA", "GET" };
}

namespace Operator {

	enum Operator { EQUAL = 0, NEQUAL, PLUS, MINUS, DIV, MULT, INF,GET,LAST,INVALID };
	const std::string OP_code[] = { "EQ","NEQ" ,"ADD", "SUB","DIV","MULT", "INF", "GET" };
	const std::string OP_trans[] = { "EQ","NEQ","ADD","SUB","DIV","MULT","INF", "GET" };

}
