#include <regex>
#include "Lexicon.h"


bool
Lexicon::isSpecialChar() const {
	return (
		m_code[index] == '{' ||
		m_code[index] == '}' ||
		m_code[index] == '(' ||
		m_code[index] == ')' ||
		m_code[index] == '[' ||
		m_code[index] == ']' ||
		m_code[index] == ',' ||
		m_code[index] == ';'
		);
}

bool
Lexicon::isOpArith() const {
	return (
		m_code[index] == '+' ||
		m_code[index] == '-' ||
		m_code[index] == '*' ||
		m_code[index] == '/'

		);
}

bool
Lexicon::isOpRel()const {
	return (
		m_code[index] == '<' ||
		m_code[index] == '>'
		);
}

bool
Lexicon::isKeyword() const {
	return !std::all_of(std::begin(keywords), std::end(keywords), [this](const string& item) {return item != lexeme; });
}

Lexicon::Lexicon(string& code) : m_code{ &code[0] }
{
}

void
Lexicon::firstCharCase() {

	if (isalpha(m_code[index]) || m_code[index] == '_') {
		lexeme.append(1, m_code[index]);
		machineState = State::idState;
		index++;
	}
	else if (isdigit(m_code[index])) {
		lexeme.append(1, m_code[index]);
		machineState = State::numberState;
		index++;
	}
	else if (isSpecialChar()) {
		lexeme.append(1, m_code[index]);
		tokenList.emplace_back(lexeme, TokenType::special_char);
		lexeme.clear();
		index++;
	}
	else if (isspace(m_code[index])) {
		lexeme.clear();
		index++;
	}
	else if (m_code[index] == '\'') {
		lexeme.append(1, m_code[index]);
		machineState = State::charState;
		index++;
	}
	else if (isOpArith()) {
		lexeme.append(1, m_code[index]);
		tokenList.emplace_back(lexeme, TokenType::op_arithmetic);
		lexeme.clear();
		index++;
	}
	else if (m_code[index] == '\"') {
		lexeme.append(1, m_code[index]);
		machineState = State::stringState;
		index++;

	}
	else if (isOpRel()) {
		lexeme.append(1, m_code[index]);
		index++;
		machineState = State::OpRelState;

	}
	else if (m_code[index] == '=') {
		lexeme.append(1, m_code[index]);
		index++;
		machineState = State::OpAssign;
	}
	else if (m_code[index] == '!') {
		lexeme.append(1, m_code[index]);
		index++;
		machineState = State::OpExcFst;
	}
	else {// Something is wrong!
		std::cout << "invalid first char case?: " << m_code[index];
		index++;
	}
}

void
Lexicon::idCase() {

	if (isalnum(m_code[index]) || m_code[index] == '_') {
		lexeme.append(1, m_code[index]);
		index++;
	}
	else if (isspace(m_code[index]) || isSpecialChar() || std::regex_match(std::string(1, m_code[index]), std::regex("[\\)=><!\\+\\*-/]"))) {
		machineState = State::keywordState;
	}
	else {
		machineState = State::invalidState;
		error = "Invalid id type";
	}
}

void
Lexicon::keywordCase()
{
	if (isKeyword()) {
		tokenList.emplace_back(lexeme, TokenType::keyword);
	}
	else {
		tokenList.emplace_back(lexeme, TokenType::id);
	}

	lexeme.clear();
	machineState = State::firstCharState;
}

void
Lexicon::numberCase() {

	if (isdigit(m_code[index])) {
		lexeme.append(1, m_code[index]);
		index++;
	}
	else if (m_code[index] == '.') {
		lexeme.append(1, m_code[index]);
		machineState = State::flValState;
		index++;
	}
	else if (isspace(m_code[index])) {
		tokenList.emplace_back(lexeme, TokenType::integer);
		lexeme.clear();
		machineState = State::firstCharState;
	}
	else if (m_code[index] == ';' || std::regex_match(std::string(1, m_code[index]), std::regex("[\\)=><!\\+\\*-/]")))
	{
		tokenList.emplace_back(lexeme, TokenType::integer);
		lexeme.clear();
		machineState = State::firstCharState;
	}
	else {
		machineState = State::invalidState;
		error = "Invalid integer type!";
	}
}

void
Lexicon::floatValidation() {
	if (isdigit(m_code[index])) {
		lexeme.append(1, m_code[index]);
		index++;
		machineState = State::floatState;
	}
	else {
		machineState = State::invalidState;
		error = "Invalid float type!";
	}
}

void
Lexicon::floatCase() {
	if (isdigit(m_code[index])) {
		lexeme.append(1, m_code[index]);
		index++;
	}
	else {
		tokenList.emplace_back(lexeme, TokenType::real);
		lexeme.clear();
		machineState = State::firstCharState;
	}
}

void
Lexicon::invalidCase() {

	if (isspace(m_code[index])) {

		invalidList.emplace_back(lexeme, TokenType::invalid_token, error);
		lexeme.clear();
		machineState = State::firstCharState;
	}
	else {
		lexeme.append(1, m_code[index]);
		index++;
	}
}

void
Lexicon::charCase() {
	machineState = State::endCharState;
	if (isalnum(m_code[index])) {
		lexeme.append(1, m_code[index]);
		index++;
	}
	else if (m_code[index] == '\\') {
		lexeme.append(2, m_code[index]);
		index += 2;
	}
	else if (m_code[index] == '\'') {
		lexeme.append(1, m_code[index]);
		tokenList.emplace_back(lexeme, TokenType::character);
		lexeme.clear();
		machineState = State::firstCharState;
		index++;
	}
	else {
		machineState = State::invalidState;
		error = "Invalid char type!";
	}

}

void
Lexicon::endCharCase()
{
	if (m_code[index] == '\'') {
		machineState = State::firstCharState;
		lexeme.append(1, m_code[index]);
		tokenList.emplace_back(lexeme, TokenType::character);
		lexeme.clear();
		index++;
	}
	else {
		machineState = State::invalidState;
		error = "Invalid char type!";
	}
}

void
Lexicon::stringCase() {

	if (m_code[index] == '\\') {
		lexeme.append(2, m_code[index]);
		index += 2;
	}
	else if (m_code[index] == '\"') {
		lexeme.append(1, m_code[index]);
		tokenList.emplace_back(lexeme, TokenType::string);
		lexeme.clear();
		index++;
		machineState = State::firstCharState;
	}
	else if (index < length - 1) {
		lexeme.append(1, m_code[index]);
		index++;
	}
	else {
		lexeme.append(1, m_code[index]);
		error = "Invalid string type!";
		invalidList.emplace_back(lexeme, TokenType::invalid_token, error);
		lexeme.clear();
		index++;
	}

}

void
Lexicon::opRelCase() {

	if (m_code[index] == '=') {
		lexeme.append(1, m_code[index]);
		index++;
	}

	tokenList.emplace_back(lexeme, TokenType::op_rel);
	lexeme.clear();
	machineState = State::firstCharState;
}

void
Lexicon::opAssignOrRelCase() {
	if (m_code[index] == '=') {
		lexeme.append(1, m_code[index]);
		tokenList.emplace_back(lexeme, TokenType::op_rel);
		index++;
	}
	else {
		tokenList.emplace_back(lexeme, TokenType::op_assig);
	}
	lexeme.clear();
	machineState = State::firstCharState;

}

void
Lexicon::opExcFstCase() {
	machineState = State::firstCharState;
	if (m_code[index] == '=') {
		lexeme.append(1, m_code[index]);
		index++;
		tokenList.emplace_back(lexeme, TokenType::op_rel);
		lexeme.clear();
	}
	else {
		machineState = State::invalidState;
		error = "Invalid relational operator!";
	}

}


vector<Token>
Lexicon::LexiconAnalysis() {

	while (m_code[length] != '\0') {
		length++;
	}

	while (index < length) {
		switch (machineState) {

		case State::firstCharState: //first char case
			firstCharCase();
			break;

		case State::idState: // id case
			idCase();
			break;

		case State::numberState: // number case
			numberCase();
			break;

		case State::flValState: // float validation case
			floatValidation();
			break;

		case State::floatState: // float case
			floatCase();
			break;

		case State::keywordState: // keyword case
			keywordCase();
			break;

		case State::charState: // char case
			charCase();
			break;

		case State::endCharState: // ending char case
			endCharCase();
			break;

		case State::stringState: // string case
			stringCase();
			break;

		case State::OpRelState: // < || > case
			opRelCase();
			break;

		case State::OpAssign: // = case
			opAssignOrRelCase();
			break;

		case State::OpExcFst: // ! case
			opExcFstCase();

			break;

		case State::invalidState:
			invalidCase();
			break;

		default:
			break;
		}


	}
	return tokenList;
}