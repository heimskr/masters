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
		context.makeGlobal<Function>("print", [](Context &context, const std::vector<Value *> &arguments, Value *) {
			for (Value *value: arguments)
				std::cout << static_cast<std::string>(*value) << std::endl;
			return context.makeValue<Undefined>();
		});
		program.interpret(context);
		context.garbageCollect();
	}

	jsParser.done();
}
