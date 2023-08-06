#pragma once
#include <vector>
#include "Lexicon.h"
#include "Semantic.h"

enum class terminaltoken
{
	tk_invalid = -1,
	tk_int,
	tk_float,
	tk_char
};

class Syntactic
{
public:
	explicit Syntactic(const std::vector<Token>& tokens);

private:
	void extractLexeme(const std::vector<Token>& tokens);
	bool acceptedStart();
	bool startBlock();
	void blockContent();
	bool endBlock();
	terminaltoken varType();
	bool ifStruct();
	bool endIfStruct();
	bool elseStruct();
	bool relationalOp();
	void rightValue();

	void syn_mainFunc();
	bool syn_block();
	bool syn_varDecl();
	bool syn_command();
	bool syn_basicCommand();
	bool syn_assignment();
	bool syn_iteration();
	bool syn_relationalExpr();
	bool syn_aritExpr();
	bool syn_aritExpr2();
	bool syn_term();
	bool syn_term2();
	bool syn_factor();

	size_t m_idx = 0;
	size_t m_list_size = 0;
	std::string m_nextToken;
	TokenType m_nextType;
	std::vector<Token> m_tokens;
	std::vector<std::string> m_lexemes;

	Semantic semantic;
};