#include "IOFile.h"
#include <iostream>
IOFile::IOFile(const string& input) : ist{ input }
{
	m_input = readInput();
}

string IOFile::readInput() {
	ostringstream buffer;
	buffer << ist.rdbuf();
	m_input = buffer.str();
	return m_input;
}

void IOFile::saveAnalysis() {
	output.open("./Files/output.txt");
	output << "The analysis log come here!";
}

std::ostream& operator<<(std::ostream& out, const IOFile& iof) {
	out << iof.m_input << "\n";
	return out;
}
