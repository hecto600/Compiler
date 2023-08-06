#include "SemanticException.h"

SemanticException::SemanticException(const std::string& error)
	:m_error{ error }
{
}

SemanticException::SemanticException(const std::string& error, const std::string& id)
	:m_error{error}, m_id{id}
{
}
std::string
SemanticException::what() const
{
	return std::string("Variable \"") + m_id + "\" " + m_error;
}