#include <string>

#include "Syntactic.h"

#include "SyntacticException.h"
#include "SemanticException.h"

Syntactic::Syntactic(const std::vector<Token>& tokens) :m_tokens{ tokens }
{
	extractLexeme(tokens);
	m_list_size = m_tokens.size();
	syn_mainFunc();
}

void
Syntactic::extractLexeme(const std::vector<Token>& tokens)
{
	for (Token tk : tokens)
		m_lexemes.emplace_back(tk.getLexeme());

}


void Syntactic::syn_mainFunc()
{
	try
	{
		if (!acceptedStart())
			throw SyntacticException{ "main should have \"int main()\" syntax." };

		syn_block();

		std::cout << "The end.\n";
	}
	catch (const SyntacticException& syn_exc)
	{
		std::cout << "Syntactic error: " << syn_exc.what() << "\n";
	}
	catch (const SemanticException& sem_exc)
	{
		std::cout << "Semantic error: " << sem_exc.what() << "\n";
	}
}

bool
Syntactic::acceptedStart()
{
	std::string main_start;
	if (m_list_size < 4)
		throw SyntacticException{ "Token list size less than 4." };

	for (; m_idx < 4; m_idx++) //4 = number of tokens needed
		main_start.append(m_lexemes.at(m_idx));


	return (main_start == "intmain()");

}

bool
Syntactic::syn_block()
{
	if (startBlock())
	{
		semantic.scopeIncr();
		blockContent();
	}
	else
		throw SyntacticException{ "Block initialization failed. No \"{\" special char." };

	if (!endBlock())
		throw SyntacticException{ "Block missing \"}\" special char." };

	semantic.scopeDecr();
	std::cout << "Successful block!\n";
	return true;

}

bool
Syntactic::startBlock()
{
	return (m_lexemes.at(m_idx) == "{");
}

void
Syntactic::blockContent()
{
	while (syn_command())
		;
}

bool
Syntactic::endBlock()
{
	return (m_lexemes.at(m_idx) == "}");
}



bool
Syntactic::syn_varDecl()
{
	bool varDecl = true;
	m_idx++;
	std::string vd_str = m_lexemes.at(m_idx);

	if (vd_str == "int" || vd_str == "float" || vd_str == "char")
	{
		m_idx++;
		TokenType is_id = m_tokens.at(m_idx).getType();

		if (is_id != TokenType::id)
			throw SyntacticException{ "Variable declaration isn't syntactically correct." };

		std::string vd_id = m_lexemes.at(m_idx);
		semantic.varDecl(vd_str, vd_id);
		m_idx++;

		if (m_lexemes.at(m_idx) != ";")
			throw SyntacticException{ "Variable declaration isn't syntactically correct." };

		std::cout << "Successful declaration!\n";
	}
	else// what if it is a command function?
		varDecl = false;

	return varDecl;
}



bool
Syntactic::syn_command()
{
	bool commandVal = true;
	if (m_idx + 1 >= m_list_size) //check if it is the end of the code
		throw SyntacticException{ "Missing tokens!" };

	std::string nextToken = m_lexemes.at(m_idx + 1);

	if (nextToken == "int"
		|| nextToken == "float"
		|| nextToken == "char"
		|| nextToken == "id"
		)
	{
		commandVal = syn_varDecl();
	}
	else if (nextToken == "while")
	{
		commandVal = syn_iteration();
	}
	else if (nextToken == "if")
	{
		commandVal = ifStruct();
	}
	else
		commandVal = syn_basicCommand();

	return commandVal;
}

bool
Syntactic::ifStruct()
{
	std::string id_if;
	m_idx++;
	size_t end = m_idx + 2;

	if (end > m_list_size)
		return false;


	for (; m_idx < end; m_idx++)
		id_if.append(m_lexemes.at(m_idx));
	m_idx--; // arit_expr has m_idx++. for ends next token position.
	return ((id_if == "if(") && syn_relationalExpr() && endIfStruct()); // invalid command, but is it valid block?

}

bool
Syntactic::endIfStruct()
{
	m_idx++;

	if (m_lexemes.at(m_idx) == ")")
	{
		m_idx++;
		syn_block();

	}
	else
		throw SyntacticException{ "if body missing \")\"." };

	if (elseStruct())
		std::cout << "Successful if else structure!\n";
	else
		std::cout << "Successful if structure!\n";

	return true;
}

bool
Syntactic::elseStruct()
{
	m_idx++;
	if (m_lexemes.at(m_idx) == "else")
	{
		m_idx++;
		syn_block();
	}
	else
	{
		m_idx--; // if there is no "else" we need to go back one token.
		return false;
	}

	return true;
}


bool
Syntactic::syn_basicCommand()
{

	return (syn_assignment()); // removed block()!!
}

bool
Syntactic::syn_assignment()
{
	bool assign = true;
	semantic.isAssignment(true);
	m_idx++;
	TokenType assign_id = m_tokens.at(m_idx).getType();
	if (assign_id == TokenType::id)
	{
		semantic.isVarDeclared(m_lexemes.at(m_idx));
		rightValue();
		std::cout << "Successful assignment!\n";
	}
	else
	{
		assign = false;
	}
	semantic.isAssignment(false);
	return assign;
}

void
Syntactic::rightValue()
{
	std::string str_id = m_lexemes.at(m_idx);

	m_idx++;
	if (m_lexemes.at(m_idx) == "=" && syn_aritExpr())
	{
		m_idx++;
		if (m_lexemes.at(m_idx) != ";")
			throw SyntacticException{ "Missing assignment \";\" symbol." };
	}
	else
		throw SyntacticException{ "Assignment doesn't have \"=\" symbol." };

	semantic.setValue();
}

bool
Syntactic::syn_aritExpr()
{
	m_idx++;

	if (syn_term() && syn_aritExpr2())
	{
		return true;
	}
	else
		throw SyntacticException{ "Invalid arithmetic syntax." };

	std::cout << "Successful arithmetic expression!\n";

}

bool
Syntactic::syn_term()
{

	if (syn_factor() && syn_term2())
		return true;
	else
		return false;

}

bool
Syntactic::syn_factor()
{
	bool isFactor = true;
	if (m_lexemes.at(m_idx) == "(")
	{
		syn_aritExpr();
		m_idx++;
		if (!(m_lexemes.at(m_idx) == ")"))
			throw SyntacticException{ "Invalid factor syntax." };

	}
	else
	{
		switch (m_tokens.at(m_idx).getType())
		{
		case TokenType::id:
		case TokenType::integer:
		case TokenType::real:
		case TokenType::character:
			if (semantic.getAssignment())
				semantic.addFactor(m_lexemes.at(m_idx));
			break;
		default:
			isFactor = false;
		}
	}
	return isFactor;
}

bool
Syntactic::syn_term2()
{
	m_idx++;
	std::string term2_op = m_lexemes.at(m_idx);
	if (term2_op == "*" || term2_op == "/")
	{
		if (semantic.getAssignment())
			semantic.operation(term2_op);
		m_idx++;
		syn_factor();
		syn_term2();
	}
	else
		m_idx--;

	return true;
}

bool
Syntactic::syn_aritExpr2()
{
	m_idx++;
	std::string arit_op = m_lexemes.at(m_idx);
	if (arit_op == "+" || arit_op == "-")
	{
		if (semantic.getAssignment())
			semantic.operation(arit_op);
		m_idx++;
		syn_term();
		syn_aritExpr2();
	}
	else
		m_idx--;

	return true;

}

bool
Syntactic::syn_iteration()
{
	std::string struc_while;
	m_idx++;
	size_t end = m_idx + 2;
	if (end > m_list_size)
		return false;


	for (; m_idx < end; m_idx++) // next analysis doesn't need m_idx++
		struc_while.append(m_lexemes.at(m_idx));
	m_idx--;

	if (struc_while == "while(" && syn_relationalExpr())
	{
		m_idx++;
		if (m_lexemes.at(m_idx) == ")")
		{
			m_idx++;
			syn_block();
		}
		else
			throw SyntacticException{ "Invalid while structure syntax." };

	}
	else
		throw SyntacticException{ "Invalid while structure inicialization syntax." };

	std::cout << "Successful loop!\n";
	return true;
}

bool
Syntactic::syn_relationalExpr()
{
	bool rel_expr = false;
	if (syn_aritExpr() && relationalOp())
	{
		rel_expr = syn_aritExpr();
	}
	return rel_expr;
}

bool
Syntactic::relationalOp()
{
	m_idx++;
	return m_tokens.at(m_idx).getType() == TokenType::op_rel;
}