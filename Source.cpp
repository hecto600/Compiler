#include <iostream>
#include <iomanip>

#include "IOFile.h"
#include "Lexicon.h"
#include "Token.h"
#include "Syntactic.h"

int main()
{
	IOFile input{ "Files/input.txt" }; // Path of code file 
	string code{ input.getInput() };
	std::cout << input;

	Lexicon lexicon{ code };

	std::cout << "\n" << std::setw(15) << "" << "LEXICON ANALYSIS:\n";
	std::vector<Token> tokenList{ lexicon.LexiconAnalysis() };
	std::vector<Token> invalidList{ lexicon.getInvalidList() };
	std::cout << "Token list:" << tokenList << "\nInvalid List:" << invalidList << "\n";

	std::cout << std::setw(15) << "" << "SYNTATIC & SEMANTIC ANALYSIS:\n";
	Syntactic syntactic{ tokenList };


	input.saveAnalysis();

}