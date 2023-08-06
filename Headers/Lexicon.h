#pragma once
#include "IOFile.h"
#include "Token.h"

#include <iostream>
#include <vector>
#include <algorithm>

using std::string;
using std::vector;


enum class State {
	invalidState = -1,
	firstCharState,
	idState,
	numberState,
	flValState,
	floatState,
	charState,
	stringState,
	endCharState,
	keywordState,
	OpRelState,
	OpAssign,
	OpExcFst

};

class Lexicon {
private:
	char* m_code{};
	size_t index{};
	size_t length{};
	string lexeme{};
	string error{};
	vector <Token> tokenList;
	vector <Token> invalidList;
	vector<string> keywords{ "main", "if", "else", "while", "do", "for", "int" , "float", "char" };
	bool isKeyword() const;
	bool isSpecialChar() const;
	bool isOpArith() const;
	bool isOpRel() const;
	State machineState{ State::firstCharState };
	void firstCharCase();
	void idCase();
	void keywordCase();
	void numberCase();
	void floatValidation();
	void floatCase();
	void invalidCase();
	void charCase();
	void endCharCase();
	void stringCase();
	void opRelCase();
	void opAssignOrRelCase();
	void opExcFstCase();


public:
	explicit Lexicon(string& file);
	vector<Token> LexiconAnalysis();
	vector<Token> getTokenList()const { return tokenList; }
	vector<Token> getInvalidList() const { return invalidList; }
};