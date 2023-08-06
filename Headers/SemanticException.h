#pragma once
#include <string>
class SemanticException
{
public:
	explicit SemanticException(const std::string& error);
	SemanticException(const std::string& error, const std::string& id);
	std::string what() const;
	
private:
	std::string m_error;
	std::string m_id;

};