#pragma once
#include "Token.h"
class Symbol
{
public:
	Symbol(const std::string& type, const size_t& scope, const std::string& id);

	std::string getType() const { return m_type; }
	size_t getScope() const { return m_scope; }
	std::string getId() const { return m_id; }
	void setValue(const std::string& value) { m_value = value; }
private:
	std::string m_type;
	size_t m_scope;
	std::string m_id;
	std::string m_value;
};