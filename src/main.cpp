#include <iostream>
#include <istream>
#include <iterator>

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

	if (jsParser.errorCount == 0) {
		assert(jsParser.root != nullptr);
		Program program(*jsParser.root);
		Context context;
		context.addDefaults();
		program.interpret(context);
		std::cout << "Number of values before garbage collection: " << context.valuePool.size() << std::endl;
		context.garbageCollect();
		std::cout << "Number of values after garbage collection: " << context.valuePool.size() << std::endl;
	}

	jsParser.done();
}
