#include <iostream>

#include "JS.h"
#include "Node.h"
#include "Value.h"

void Node::assertType(Type type) {
	if (getType() != type)
		throw std::runtime_error("Assertion failed: " + std::string(typeid(*this).name()) + " at [" +
			std::to_string(start) + " ... " + std::to_string(end) + "] is not an instance of " + stringify(type));
}

const char * stringify(Node::Type type) {
	switch (type) {
		case Node::Type::Invalid: return "Invalid";
		case Node::Type::Program: return "Program";
		case Node::Type::BlockStatement: return "BlockStatement";
		case Node::Type::Identifier: return "Identifier";
		case Node::Type::VariableDeclarator: return "VariableDeclarator";
		case Node::Type::VariableDeclaration: return "VariableDeclaration";
		default:
			return "???";
	}
}

std::unique_ptr<Node> Node::fromJSON(const nlohmann::json &json) {
	const std::string &type = json.at("type");

	if (type == "Program")
		return std::make_unique<ProgramNode>(json);

	if (type == "BlockStatement")
		return std::make_unique<BlockStatementNode>(json);

	if (type == "Identifier")
		return std::make_unique<IdentifierNode>(json);

	if (type == "VariableDeclarator")
		return std::make_unique<VariableDeclaratorNode>(json);

	if (type == "VariableDeclaration")
		return std::make_unique<VariableDeclarationNode>(json);

	if (type == "ExpressionStatement")
		return std::make_unique<ExpressionStatementNode>(json);

	if (type == "IfStatement")
		return std::make_unique<IfStatementNode>(json);

	if (type == "BinaryExpression")
		return std::make_unique<BinaryExpressionNode>(json);

	if (type == "ArrayExpression")
		return std::make_unique<ArrayExpressionNode>(json);

	std::cerr << json.dump() << std::endl;
	throw std::invalid_argument("Node::fromJSON failed: type \"" + type + "\" not handled");
}

void Node::absorbPosition(const nlohmann::json &json) {
	start = json.at("start");
	end   = json.at("end");
}

ProgramNode::ProgramNode(const nlohmann::json &json) {
	absorbPosition(json);
	for (const auto &subnode: json.at("body"))
		body.emplace_back(Node::fromJSON(subnode));
}

std::pair<Result, Value *> ProgramNode::interpret(Context &context) {
	for (auto &node: body) {
		auto [result, value] = node->interpret(context);
		if (result == Result::Return)
			return {result, value};
	}

	return {Result::None, nullptr};
}

BlockStatementNode::BlockStatementNode(const nlohmann::json &json) {
	absorbPosition(json);
	for (const auto &subnode: json.at("body"))
		body.emplace_back(Node::fromJSON(subnode));
}

std::pair<Result, Value *> BlockStatementNode::interpret(Context &context) {
	context.stack.push();

	for (auto &node: body) {
		auto [result, value] = node->interpret(context);
		if (result != Result::None) {
			context.stack.pop();
			return {result, value};
		}
	}

	context.stack.pop();
	return {Result::None, nullptr};
}

IdentifierNode::IdentifierNode(const nlohmann::json &json): name(json.at("name")) {
	absorbPosition(json);
}

VariableDeclaratorNode::VariableDeclaratorNode(const nlohmann::json &json):
id(Node::fromJSON(json.at("id"))), init(Node::fromJSON(json.at("init"))) {
	absorbPosition(json);
}

VariableDeclarationNode::VariableDeclarationNode(const nlohmann::json &json): kind(getKind(json.at("kind"))) {
	absorbPosition(json);
	for (const auto &subjson: json.at("declarations"))
		declarators.push_back(std::make_unique<VariableDeclaratorNode>(subjson));
}

std::pair<Result, Value *> VariableDeclarationNode::interpret(Context &context) {
	for (const auto &declarator: declarators) {
		declarator->id->assertType(Type::Identifier);

		const std::string &name = dynamic_cast<IdentifierNode &>(*declarator->id).name;
		const bool must_be_unique = kind == DeclarationKind::Let || kind == DeclarationKind::Const;

		if (must_be_unique && context.stack.inLastScope(name))
			throw std::runtime_error("Name \"" + name + "\" already exists in top scope");

		if (declarator->init)
			context.stack.insert(name, declarator->init->evaluate(context));
		else
			context.stack.insert(name, context.makeValue<Undefined>());
	}

	return {Result::None, nullptr};
}

DeclarationKind VariableDeclarationNode::getKind(const std::string &string) {
	if (string == "const")
		return DeclarationKind::Const;
	if (string == "let")
		return DeclarationKind::Let;
	if (string == "var")
		return DeclarationKind::Var;
	return DeclarationKind::Invalid;
}

ExpressionStatementNode::ExpressionStatementNode(const nlohmann::json &json):
	expression(Node::fromJSON(json.at("expression"))) {}

std::pair<Result, Value *> ExpressionStatementNode::interpret(Context &context) {
	expression->evaluate(context);
	return {Result::None, nullptr};
}

IfStatementNode::IfStatementNode(const nlohmann::json &json):
	test(Node::fromJSON(json.at("test"))), consequent(Node::fromJSON(json.at("consequent"))),
	alternate(Node::fromJSON(json.at("alternate"))) {}

std::pair<Result, Value *> IfStatementNode::interpret(Context &context) {
	if (*test->evaluate(context))
		return consequent->interpret(context);
	if (alternate)
		return alternate->interpret(context);
	return {Result::None, nullptr};
}

BinaryExpressionNode::BinaryExpressionNode(const nlohmann::json &json):
	left(Node::fromJSON(json.at("left"))), right(Node::fromJSON(json.at("right"))), op(json.at("alternate")) {}

Value * BinaryExpressionNode::evaluate(Context &context) {
	Value *out = nullptr;

	if (op == "+") {
		out = *left->evaluate(context) + *right->evaluate(context);
	} else if (op == "-") {
		out = *left->evaluate(context) - *right->evaluate(context);
	} else if (op == "*") {
		out = *left->evaluate(context) * *right->evaluate(context);
	} else if (op == "/") {
		out = *left->evaluate(context) / *right->evaluate(context);
	} else if (op == "%") {
		out = *left->evaluate(context) % *right->evaluate(context);
	} else if (op == "&") {
		out = *left->evaluate(context) & *right->evaluate(context);
	} else if (op == "|") {
		out = *left->evaluate(context) | *right->evaluate(context);
	} else if (op == "^") {
		out = *left->evaluate(context) ^ *right->evaluate(context);
	} else if (op == "==") {
		out = *left->evaluate(context) == *right->evaluate(context);
	} else if (op == "!=") {
		out = *left->evaluate(context) != *right->evaluate(context);
	} else if (op == "<") {
		out = *left->evaluate(context) < *right->evaluate(context);
	} else if (op == ">") {
		out = *left->evaluate(context) > *right->evaluate(context);
	} else if (op == "<=") {
		out = *left->evaluate(context) <= *right->evaluate(context);
	} else if (op == ">=") {
		out = *left->evaluate(context) >= *right->evaluate(context);
	} else if (op == "**") {
		out = left->evaluate(context)->power(*right->evaluate(context));
	} else if (op == "&&") {
		Value *left_value = left->evaluate(context);
		out = *left_value? right->evaluate(context) : left_value;
	} else if (op == "||") {
		Value *left_value = left->evaluate(context);
		out = *left_value? left_value : right->evaluate(context);
	}

	if (out == nullptr)
		throw std::invalid_argument("Unrecognized binary operator: \"" + op + '"');

	out->context = &context;
	return out;
}

ArrayExpressionNode::ArrayExpressionNode(const nlohmann::json &json) {
	for (const auto &element: json.at("elements").items())
		elements.push_back(Node::fromJSON(element.value()));
}

Value * ArrayExpressionNode::evaluate(Context &context) {
	auto *out = context.makeValue<Array>();

	for (const auto &element: elements)
		out->values.emplace_back(element->evaluate(context));

	return out;
}
