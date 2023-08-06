#pragma once
#include <string>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::ofstream;
using std::ostringstream;
using std::string;

class IOFile {
private:
	ofstream output;
	ifstream ist;
	string m_input;
	string readInput();
public:
	string getInput() const { return m_input; }
	explicit IOFile(const string& input);
	void saveAnalysis() ;
	friend std::ostream& operator<<(std::ostream& out, const IOFile& iof);
};