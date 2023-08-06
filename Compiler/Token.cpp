#include "Token.h"

Token::Token(const string& lexeme, TokenType type) : m_lexeme{ lexeme }, m_type{ type }
{
}

Token::Token(const string& lexeme, TokenType type, const string& error) : m_lexeme{ lexeme }, m_type{ type }, m_error{ error }
{
}

std::ostream& operator<<(std::ostream& out, const Token& token) {
	out << left << "\t" << setw(15) << token.m_lexeme << setw(20);

	switch (token.m_type) {
	case TokenType::keyword:
		out << "keyword" << "\n";
		break;

	case TokenType::id:
		out << "id" << "\n";
		break;

	case TokenType::integer:
		out << "integer" << "\n";
		break;

	case TokenType::real:
		out << "real" << "\n";
		break;

	case TokenType::character:
		out << "character" << "\n";
		break;

	case TokenType::string:
		out << "string" << "\n";
		break;

	case TokenType::op_arithmetic:
		out << "arithmetic operator" << "\n";
		break;

	case TokenType::op_assig:
		out << "assignment operator" << "\n";
		break;

	case TokenType::op_rel:
		out << "relational operator" << "\n";
		break;

	case TokenType::special_char:
		out << "special character" << "\n";
		break;

	case TokenType::invalid_token:
		out << "Not a valid token!" << "\n";
		break;

	default:
		break;

	}

	return out;
}

std::ostream& operator<<(std::ostream& out, const vector<Token>& tokenList) {
	if (tokenList.empty())
	{
		out << "\nAll tokens are valid!\n";
	}
	else
	{
		out << "\n" << left << setw(38) << "Lexeme" << "Token" << "\n";
		for (size_t i = 0; i < tokenList.size(); i++)
		{
			out << setw(2) << "" << "[" << i << "]: "
				<< setw(25) << tokenList.at(i).m_lexeme << "\t";

			switch (tokenList.at(i).m_type) {
			case TokenType::keyword:
				out << "keyword" << "\n";
				break;

			case TokenType::id:
				out << "id" << "\n";
				break;

			case TokenType::integer:
				out << "integer" << "\n";
				break;

			case TokenType::real:
				out << "real" << "\n";
				break;

			case TokenType::character:
				out << "character" << "\n";
				break;

			case TokenType::string:
				out << "string" << "\n";
				break;

			case TokenType::op_arithmetic:
				out << "arithmetic operator" << "\n";
				break;

			case TokenType::op_assig:
				out << "assignment operator" << "\n";
				break;

			case TokenType::op_rel:
				out << "relational operator" << "\n";
				break;

			case TokenType::special_char:
				out << "special character" << "\n";
				break;

			case TokenType::invalid_token:
				out << tokenList.at(i).m_error << "\n";
				break;

			default:
				break;

			}
		}
	}


	return out;
}