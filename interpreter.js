#!/usr/bin/env node

const acorn = require("acorn");

const if_else_test = `
	let x = 42 + -2;
	if (x == 42) {
		console.log("yes");
	} else if (x == 40) {
		console.log(40);
	} else
		console.log(false);
`;

const objects_test = `
	let d = 100;

	let obj = {
		a: 42,
		b: {
			c: 64
		},
		d,
		f(x) { return x + 1000; }
	};

	console.log(obj.f(1));
`;

const functions_test = `
	function double(x) {
		return x * 2;
	}

	console.log(double(42));
	console.log((function(x) { return !x; })(false));
	console.log((x => -x)(64));
`;

const fibonacci_test = `
	function bad_fibo(n) {
		if (n <= 1)
			return n;
		return bad_fibo(n - 2) + bad_fibo(n - 1);
	}

	console.log(bad_fibo(10));
`;

const arrays_test = `
	let arr = [1, 2, 3];
	arr[3] = 4;
	console.log(arr);
`;

const while_test = `
	let i = 0;
	while (i < 100) {
		if (++i % 2 == 0)
			continue;
		console.log(i);
		if (i == 91)
			break;
	}
`;

const to_parse = while_test;

const parsed = acorn.parse(to_parse, {
	ecmaVersion: "latest"
});

function makeDefaultScope() {
	return [{
		console: {
			log  (s, this_obj, params) { console.log  (...params); },
			info (s, this_obj, params) { console.info (...params); },
			error(s, this_obj, params) { console.error(...params); },
			warn (s, this_obj, params) { console.warn (...params); },
		}
	}];
}

function lookup(name, scopes) {
	for (let i = scopes.length - 1; 0 <= i; --i) {
		if (name in scopes[i]) {
			return scopes[i][name];
		}
	}

	throw `Couldn't find name "${name}"`;
}

function inLastScope(name, scopes) {
	if (scopes.length == 0) {
		throw "Empty scopes provided to inLastScope";
	}

	return name in scopes[scopes.length - 1];
}

function insert(name, value, scopes) {
	if (scopes.length == 0) {
		throw "Empty scopes provided to insert";
	}

	scopes[scopes.length - 1][name] = value;
}

function update(name, scopes, value) {
	for (let i = scopes.length - 1; 0 <= i; --i) {
		if (name in scopes[i]) {
			return scopes[i][name] = value;
		}
	}

	throw `Couldn't find name "${name}"`;
}

function makeFunction(node) {
	return (s, this_obj, args) => {
		let i = 0, last_scope = {};

		for (const param of node.params) {
			if (param.type != "Identifier") {
				console.error(param);
				throw `Expected an identifier as a parameter, got "${param.type}"`;
			}

			last_scope[param.name] = args[i++];
		}

		if (this_obj !== undefined) {
			last_scope.this = this_obj;
		}

		return interpret(node.body, [...s, last_scope])[1];
	};
}

function makeArrowFunction(node) {
	return (s, _, args) => {
		let i = 0, last_scope = {};

		for (const param of node.callee.params) {
			if (param.type != "Identifier") {
				console.error(param);
				throw `Expected an identifier as a parameter, got "${param.type}"`;
			}

			last_scope[param.name] = args[i++];
		}

		return evaluate(node.callee.body, [...s, last_scope]);
	};
}

function interpret(node, scopes) {
	if (node.type == "Program") {
		scopes = makeDefaultScope();

		for (const subnode of node.body) {
			interpret(subnode, scopes);
		}
	} else if (node.type == "BlockStatement") {
		scopes.push({});

		for (const subnode of node.body) {
			const [should_return, return_value] = interpret(subnode, scopes);
			if (should_return) {
				scopes.pop();
				return [should_return, return_value];
			}
		}

		scopes.pop();
	} else if (node.type == "VariableDeclaration") {
		for (const declaration of node.declarations) {
			if (declaration.id.type != "Identifier") {
				throw `Invalid declaration id: ${JSON.stringify(declaration.id)}`;
			}

			const kind = node.kind;
			const name = declaration.id.name;

			if (kind == "let" && inLastScope(name, scopes)) {
				throw `Interpreter error: name "${name}" already exists in scope`;
			}

			insert(name, evaluate(declaration.init, scopes), scopes);
		}
	} else if (node.type == "ExpressionStatement") {
		evaluate(node.expression, scopes);
	} else if (node.type == "IfStatement") {
		if (evaluate(node.test, scopes)) {
			return interpret(node.consequent, scopes);
		} else if (node.alternate) {
			return interpret(node.alternate, scopes);
		}
	} else if (node.type == "FunctionDeclaration") {
		if (node.id.type != "Identifier") {
			console.error(node.id);
			throw `Expected an identifier as a FunctionDeclaration id, got "${node.id.type}"`;
		}

		insert(node.id.name, makeFunction(node), scopes);
	} else if (node.type == "ReturnStatement") {
		return [true, evaluate(node.argument, scopes)];
	} else if (node.type == "BreakStatement") {
		return ["break", undefined];
	} else if (node.type == "ContinueStatement") {
		return ["continue", undefined];
	} else if (node.type == "WhileStatement") {
		while (evaluate(node.test, scopes)) {
			const [should_return, return_value] = interpret(node.body, scopes);
			if (should_return && should_return != "continue") {
				return [false, return_value];
			}
		}
	} else {
		console.error("Unrecognized node in interpret:", node);
	}

	return [false, undefined];
}

function evaluate(node, scopes) {
	if (node.type == "BinaryExpression") {
		switch (node.operator) {
			case "+":
				return evaluate(node.left, scopes) + evaluate(node.right, scopes);
			case "-":
				return evaluate(node.left, scopes) - evaluate(node.right, scopes);
			case "*":
				return evaluate(node.left, scopes) * evaluate(node.right, scopes);
			case "/":
				return evaluate(node.left, scopes) / evaluate(node.right, scopes);
			case "%":
				return evaluate(node.left, scopes) % evaluate(node.right, scopes);
			case "&":
				return evaluate(node.left, scopes) & evaluate(node.right, scopes);
			case "|":
				return evaluate(node.left, scopes) | evaluate(node.right, scopes);
			case "^":
				return evaluate(node.left, scopes) ^ evaluate(node.right, scopes);
			case "==":
				return evaluate(node.left, scopes) == evaluate(node.right, scopes);
			case "!=":
				return evaluate(node.left, scopes) != evaluate(node.right, scopes);
			case "<":
				return evaluate(node.left, scopes) < evaluate(node.right, scopes);
			case ">":
				return evaluate(node.left, scopes) > evaluate(node.right, scopes);
			case "<=":
				return evaluate(node.left, scopes) <= evaluate(node.right, scopes);
			case ">=":
				return evaluate(node.left, scopes) >= evaluate(node.right, scopes);
			case "**":
				return evaluate(node.left, scopes) ** evaluate(node.right, scopes);
			case "&&": {
				const left = evaluate(node.left, scopes);
				return left? evaluate(node.right, scopes) : left;
			}
			case "||": {
				const left = evaluate(node.left, scopes);
				return left? left : evaluate(node.right, scopes);
			}
			default:
				throw `Unrecognized binary operator: "${node.operator}"`;
		}
	} else if (node.type == "Literal") {
		return node.value;
	} else if (node.type == "Identifier") {
		return lookup(node.name, scopes);
	} else if (node.type == "FunctionExpression") {
		return makeFunction(node);
	} else if (node.type == "ArrowFunctionExpression") {
		return makeArrowFunction(node);
	} else if (node.type == "CallExpression") {
		let callee = evaluate(node.callee, scopes);

		if (typeof callee != "function") {
			throw "Not a function";
		}

		return callee(scopes, {}, node.arguments.map(argument => evaluate(argument, scopes)));
	} else if (node.type == "UnaryExpression") {
		switch (node.operator) {
			case "-":
				return -evaluate(node.argument, scopes);
			case "!":
				return !evaluate(node.argument, scopes);
			default:
				throw `Unrecognized unary operator: "${node.operator}"`;
		}
	} else if (node.type == "ObjectExpression") {
		let out = {};

		for (const property of node.properties) {
			if (property.key.type == "Identifier") {
				out[property.key.name] = evaluate(property.value, scopes);
			} else {
				out[evaluate(property.key, scopes)] = evaluate(property.value, scopes);
			}
		}

		return out;
	} else if (node.type == "ArrayExpression") {
		return node.elements.map(element => evaluate(element, scopes));
	} else if (node.type == "MemberExpression") {
		let obj = evaluate(node.object, scopes);

		if (typeof obj != "object") {
			console.error({obj});
			throw `Can't access member of non-object (type ${typeof obj})`;
		}

		if (node.property.type == "Identifier") {
			return obj[node.property.name];
		} else {
			return obj[evaluate(node.property, scopes)];
		}
	} else if (node.type == "UpdateExpression") {
		if (node.argument.type == "Identifier") {
			const old_value = lookup(node.argument.name, scopes);
			let new_value = old_value;

			if (node.operator == "++") {
				++new_value;
			} else if (node.operator == "--") {
				--new_value;
			} else {
				throw `Unrecognized UpdateExpression operator: ${node.operator}`;
			}

			update(node.argument.name, scopes, new_value);
			return node.prefix? new_value : old_value;
		} else {
			throw `Unsupported argument type in UpdateExpression: ${node.argument.type}`;
		}
	} else {
		console.error("Unrecognized node in evaluate:", node);
		return undefined;
	}
}

interpret(parsed);
