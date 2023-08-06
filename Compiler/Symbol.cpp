#include "Symbol.h"

Symbol::Symbol(const std::string& type, const size_t& scope, const std::string& id)
	:m_type{ type }, m_scope{ scope }, m_id{ id }
{
}