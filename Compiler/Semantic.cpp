#include <regex>
#include "Semantic.h"
#include "SemanticException.h"

void
Semantic::varDecl(const std::string& type, const std::string& id)
{
	for (const auto& symb : m_symbol_table)
	{
		if (id == symb.getId() && symb.getScope() == m_scope)
			throw SemanticException{ "already exists!", id };

	}
	m_symbol_table.emplace_back(type, m_scope, id);
}

void
Semantic::isVarDeclared(const std::string& id)
{
	bool var_exist = false;
	size_t i = 0;
	for (const auto& symb : m_symbol_table)
	{
		if (symb.getId() == id && symb.getScope() <= m_scope)
		{
			var_exist = true;
			break;
		}
		i++;
	}
	if (!var_exist)
		throw SemanticException{ "doesn't exist or out of scope!", id };

	Symbol tmp = m_symbol_table.at(i);
	m_var_id = tmp.getId();
	m_var_type = tmp.getType();
	m_var_idx = i;
}

void
Semantic::setValue()
{
	m_symbol_table.at(m_var_idx).setValue(m_expr);
	m_var_id.clear();
	m_var_type.clear();
	m_expr.clear();
}

void
Semantic::scopeIncr()
{
	m_scope++;
}

void
Semantic::scopeDecr()
{
	m_scope--;
}

void Semantic::operation(const std::string& op)
{
	m_op = op.at(0);
}

void Semantic::calculate(int a, int b)
{
	switch (m_op)
	{
	case '+':
		m_expr = std::to_string(a + b);
		break;
	case '-':
		m_expr = std::to_string(a - b);
		break;
	case '*':
		m_expr = std::to_string(a * b);
		break;
	case '/':
		m_expr = std::to_string(a / b);
		break;
	default:
		std::cout << "Impossible calculate() case!";
		break;
	}
}

void Semantic::calculate(float a, float b)
{
	switch (m_op)
	{
	case '+':
		m_expr = std::to_string(a + b);
		break;
	case '-':
		m_expr = std::to_string(a - b);
		break;
	case '*':
		m_expr = std::to_string(a * b);
		break;
	case '/':
		m_expr = std::to_string(a / b);
		break;
	default:
		std::cout << "Impossible calculate() case!";
		break;
	}
}

void
Semantic::isAssignment(bool assign)
{
	m_is_assign = assign;
}

void Semantic::addFactor(const std::string& factor)
{
	if (std::regex_match(factor, std::regex("\\d+")) && m_var_type == "int")
	{
		if (m_expr.empty()) // when there is no number yet inside expression
		{
			m_expr = "0";
		}
		else
		{
			int a = std::stoi(m_expr);
			int b = std::stoi(factor);
			calculate(a, b);

		}

	}
	else if (std::regex_match(factor, std::regex("\\d+\\.\\d+")) && m_var_type == "float")
	{
		if (m_expr.empty())
		{
			m_expr = "0";
		}
		else
		{
			float a = std::stof(m_expr);
			float b = std::stof(factor);
			calculate(a, b);
		}
	}
	else if (!(std::regex_match(factor, std::regex("'[\\w\\W]'")) && m_var_type == "char"))
		throw SemanticException{ "can't be assigned! Different lvalue and rvalue type!", m_var_id };

}