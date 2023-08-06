#pragma once
#include <string>

class SyntacticException
{
public:
	explicit SyntacticException(const std::string& error);
	std::string what() const { return m_error; }

private:
	std::string m_error;
};

