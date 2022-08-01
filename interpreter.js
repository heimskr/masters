#!/usr/bin/env node

const acorn = require("acorn");

const if_else_test = `
	let x = 42 + -2;
	if (x == 42) {
		print("yes");
	} else if (x == 40) {
		print(40);
	} else
		print(false);
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

	print(obj);
`;

const to_parse = objects_test;

const parsed = acorn.parse(to_parse, {
	ecmaVersion: "latest"
});

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

function interpret(node, scopes) {
	if (node.type == "Program") {
		scopes = [[]];

		for (const subnode of node.body) {
			interpret(subnode, scopes);
		}
	} else if (node.type == "BlockStatement") {
		scopes.push([]);

		for (const subnode of node.body) {
			interpret(subnode, scopes);
		}
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
			interpret(node.consequent, scopes);
		} else {
			interpret(node.alternate, scopes);
		}
	} else {
		console.log(node);
	}
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
	} else if (node.type == "CallExpression") {
		if (node.callee.type != "Identifier" || node.callee.name != "print") {
			console.error(node.callee);
			throw `Unsupported callee: ${JSON.stringify(node.callee)}`;
		}

		console.log(...node.arguments.map(argument => evaluate(argument, scopes)));
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
	} else {
		console.log(node);
		return undefined;
	}
}

interpret(parsed);
