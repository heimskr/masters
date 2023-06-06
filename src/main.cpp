#include <iostream>
#include <istream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "JS.h"
#include "Node.h"
#include "Parser.h"

int main(int, char **) {
	// https://stackoverflow.com/a/202097/227663
	std::cin >> std::noskipws;
	std::istream_iterator<char> ibegin(std::cin), iend;
	const std::string input(ibegin, iend);

	jsParser.in(input);
	jsParser.debug(false, false);
	jsParser.parse();

	auto program = Node::fromAST(parsed);
	if (!program) {
		std::cerr << "Program node is null!\n";
		return 1;
	}

	Context context;
	program->interpret(context);
}
