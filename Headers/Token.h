#pragma once
#include <iostream>
#include <iomanip>
#include <vector>

using std::string;
using std::setw;
using std::left;
using std::vector;

enum class TokenType {
	invalid_token = -1,
	keyword,
	id,
	integer,
	real,
	character,
	string,
	op_arithmetic,
	op_rel,
	op_assig,
	special_char
};

class Token
{
private:
	string m_lexeme{};
	TokenType m_type{};
	string m_error{};
public:
	Token(const string& lexeme, TokenType type);
	Token(const string& lexeme, TokenType type, const string& error);

	string getLexeme() const { return m_lexeme; }
	TokenType getType() const { return m_type; }
	friend std::ostream& operator<<(std::ostream& out, const Token& token);
	friend std::ostream& operator<<(std::ostream& out, const vector<Token>& tokenList);

};



