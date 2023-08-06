#pragma once
#include <vector>
#include "Token.h"
#include "Symbol.h"


class Semantic
{
public:
	void varDecl(const std::string& type, const std::string& id);

	void scopeIncr();
	void scopeDecr();
	void operation(const std::string& op);
	void calculate(int a, int b);
	void calculate(float a, float b);

	void isAssignment(bool assign);
	void addFactor(const std::string& factor);
	void isVarDeclared(const std::string& id);
	void setValue();

	bool getAssignment() const { return m_is_assign; }

private:
	size_t m_scope = 0;
	std::vector <Symbol> m_symbol_table;

	bool m_is_assign = false;
	size_t m_var_idx;
	std::string m_var_id;
	std::string m_var_type;
	std::string m_expr;
	char m_op;

};