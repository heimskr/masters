#include <cassert>
#include <cmath>
#include <iostream>

#include "ASTNode.h"
#include "Errors.h"
#include "JS.h"
#include "Lexer.h"
#include "Node.h"
#include "Parser.h"
#include "Value.h"

std::unordered_set<BinaryExpression::Type> BinaryExpression::assignmentTypes {
	BinaryExpression::Type::Assignment,
	BinaryExpression::Type::AdditionAssignment,
	BinaryExpression::Type::SubtractionAssignment,
	BinaryExpression::Type::MultiplicationAssignment,
	BinaryExpression::Type::ExponentiationAssignment,
	BinaryExpression::Type::DivisionAssignment,
	BinaryExpression::Type::ModuloAssignment,
	BinaryExpression::Type::LeftShiftAssignment,
	BinaryExpression::Type::RightShiftArithmeticAssignment,
	BinaryExpression::Type::RightShiftLogicalAssignment,
	BinaryExpression::Type::BitwiseAndAssignment,
	BinaryExpression::Type::BitwiseOrAssignment,
	BinaryExpression::Type::LogicalOrAssignment,
	BinaryExpression::Type::LogicalAndAssignment,
	BinaryExpression::Type::BitwiseXorAssignment,
};

DeclarationKind getKind(int symbol) {
	switch (symbol) {
		case JSTOK_CONST: return DeclarationKind::Const;
		case JSTOK_LET:   return DeclarationKind::Let;
		case JSTOK_VAR:   return DeclarationKind::Var;
		default:
			return DeclarationKind::Invalid;
	}
}

const char * stringify(NodeType type) {
	switch (type) {
		case NodeType::Invalid:             return "Invalid";
		case NodeType::Program:             return "Program";
		case NodeType::Block:               return "Block";
		case NodeType::VariableDefinition:  return "VariableDefinition";
		case NodeType::VariableDefinitions: return "VariableDefinitions";
		case NodeType::IfStatement:         return "IfStatement";
		case NodeType::BinaryExpression:    return "BinaryExpression";
		case NodeType::UnaryExpression:     return "UnaryExpression";
		default:
			return "???";
	}
}

void Node::assertType(NodeType type) {
	if (getType() != type)
		throw std::runtime_error("Assertion failed: " + std::string(typeid(*this).name()) + " at [" +
			static_cast<std::string>(location) + "] is not an instance of " + stringify(type));
}

std::unique_ptr<Node> Node::fromAST(const ASTNode &node) {
	throw std::invalid_argument("Node::fromAST failed: symbol \"" + std::string(node.getName()) + "\" not handled");
}

void Node::absorbPosition(const ASTNode &node) {
	location = node.location;
}

Program::Program(const ASTNode &node) {
	absorbPosition(node);

	for (const auto *subnode: node)
		body.emplace_back(Statement::create(*subnode));
}

std::pair<Result, Value *> Program::interpret(Context &context) {
	for (auto &node: body) {
		const auto [result, value] = node->interpret(context);

		if (result == Result::Return)
			return {result, value};

		if (result == Result::Break)
			throw JSException("Invalid break statement", node->location);

		if (result == Result::Continue)
			throw JSException("Invalid continue statement", node->location);
	}

	return {Result::None, nullptr};
}

Block::Block(const ASTNode &node) {
	absorbPosition(node);

	for (const auto *subnode: node)
		body.emplace_back(Statement::create(*subnode));
}

std::pair<Result, Value *> Block::interpret(Context &context) {
	context.stack.push();

	for (auto &node: body) {
		const auto [result, value] = node->interpret(context);
		if (result != Result::None) {
			context.stack.pop();
			return {result, value};
		}
	}

	context.stack.pop();
	return {Result::None, nullptr};
}

VariableDefinition::VariableDefinition(const ASTNode &node):
	ident(*node.at(0)->text),
	value(1 < node.size()? Expression::create(*node.at(1)) : nullptr) {}

std::pair<Result, Value *> VariableDefinition::interpret(Context &context) {
	throw std::logic_error("Unimplemented");
}

VariableDefinitions::VariableDefinitions(const ASTNode &node): kind(getKind(node.at(0)->symbol)) {
	absorbPosition(node);

	for (const auto *subnode: *node.front())
		definitions.push_back(std::make_unique<VariableDefinition>(*subnode));
}

std::pair<Result, Value *> VariableDefinitions::interpret(Context &context) {
	for (const auto &definition: definitions) {
		// const std::string &name = dynamic_cast<IdentifierNode &>(*declarator->id).name;
		// const bool must_be_unique = kind == DeclarationKind::Let || kind == DeclarationKind::Const;

		// if (must_be_unique && context.stack.inLastScope(name))
		// 	throw std::runtime_error("Name \"" + name + "\" already exists in top scope");

		// if (declarator->init)
		// 	context.stack.insert(name, declarator->init->evaluate(context));
		// else
		// 	context.stack.insert(name, context.makeValue<Undefined>());
	}

	return {Result::None, nullptr};
}

// ExpressionStatement::ExpressionStatement(const ASTNode &node):
// 	expression(Node::fromJSON(json.at("expression"))) {}

// std::pair<Result, Value *> ExpressionStatement::interpret(Context &context) {
// 	expression->evaluate(context);
// 	return {Result::None, nullptr};
// }

IfStatement::IfStatement(const ASTNode &node):
	condition(Expression::create(*node.at(0))),
	consequent(Statement::create(*node.at(1))),
	alternate(2 < node.size()? Statement::create(*node.at(2)) : nullptr) {}

std::pair<Result, Value *> IfStatement::interpret(Context &context) {
	if (*condition->evaluate(context))
		return consequent->interpret(context);
	if (alternate)
		return alternate->interpret(context);
	return {Result::None, nullptr};
}

// BinaryExpressionNode::BinaryExpressionNode(const ASTNode &node):
// 	left(Node::fromJSON(json.at("left"))), right(Node::fromJSON(json.at("right"))), op(json.at("alternate")) {}

// Value * BinaryExpressionNode::evaluate(Context &context) {
// 	Value *out = nullptr;

// 	if (op == "+") {
// 		out = *left->evaluate(context) + *right->evaluate(context);
// 	} else if (op == "-") {
// 		out = *left->evaluate(context) - *right->evaluate(context);
// 	} else if (op == "*") {
// 		out = *left->evaluate(context) * *right->evaluate(context);
// 	} else if (op == "/") {
// 		out = *left->evaluate(context) / *right->evaluate(context);
// 	} else if (op == "%") {
// 		out = *left->evaluate(context) % *right->evaluate(context);
// 	} else if (op == "&") {
// 		out = *left->evaluate(context) & *right->evaluate(context);
// 	} else if (op == "|") {
// 		out = *left->evaluate(context) | *right->evaluate(context);
// 	} else if (op == "^") {
// 		out = *left->evaluate(context) ^ *right->evaluate(context);
// 	} else if (op == "==") {
// 		out = *left->evaluate(context) == *right->evaluate(context);
// 	} else if (op == "!=") {
// 		out = *left->evaluate(context) != *right->evaluate(context);
// 	} else if (op == "<") {
// 		out = *left->evaluate(context) < *right->evaluate(context);
// 	} else if (op == ">") {
// 		out = *left->evaluate(context) > *right->evaluate(context);
// 	} else if (op == "<=") {
// 		out = *left->evaluate(context) <= *right->evaluate(context);
// 	} else if (op == ">=") {
// 		out = *left->evaluate(context) >= *right->evaluate(context);
// 	} else if (op == "**") {
// 		out = left->evaluate(context)->power(*right->evaluate(context));
// 	} else if (op == "&&") {
// 		Value *left_value = left->evaluate(context);
// 		out = *left_value? right->evaluate(context) : left_value;
// 	} else if (op == "||") {
// 		Value *left_value = left->evaluate(context);
// 		out = *left_value? left_value : right->evaluate(context);
// 	}

// 	if (out == nullptr)
// 		throw std::invalid_argument("Unrecognized binary operator: \"" + op + '"');

// 	out->context = &context;
// 	return out;
// }

// ArrayExpressionNode::ArrayExpressionNode(const ASTNode &node) {
// 	for (const auto &element: json.at("elements").items())
// 		elements.push_back(Node::fromJSON(element.value()));
// }

// Value * ArrayExpressionNode::evaluate(Context &context) {
// 	auto *out = context.makeValue<Array>();

// 	for (const auto &element: elements)
// 		out->values.emplace_back(element->evaluate(context));

// 	return out;
// }

// LiteralNode::LiteralNode(const ASTNode &node) {
// 	const auto &value_json = json.at("value");
// 	if (value_json.is_string())
// 		value = value_json.get<std::string>();
// 	else if (value_json.is_number())
// 		value = value_json.get<double>();
// 	else
// 		throw std::runtime_error("Invalid literal value: " + value_json.dump());
// }

// Value * LiteralNode::evaluate(Context &context) {
// 	if (std::holds_alternative<std::string>(value))
// 		return context.makeValue<String>(std::get<std::string>(value));

// 	if (std::holds_alternative<double>(value))
// 		return context.makeValue<Number>(std::get<double>(value));

// 	throw std::runtime_error("LiteralNode value has an invalid type");
// }

BinaryExpression::BinaryExpression(const ASTNode &node):
	type(getType(node.symbol)),
	left(Expression::create(*node.at(0))),
	right(Expression::create(*node.at(1))) {}

Value * BinaryExpression::evaluate(Context &context) {
	throw Unimplemented();
}

Value ** BinaryExpression::access(Context &context, bool *is_const) {
	throw std::logic_error("Unimplemented");
}

Value * BinaryExpression::evaluateAccess(Context &context) {
	auto saver = context.writeMember();

	bool is_const = false;
	Value **accessed = access(context, &is_const);
	assert(accessed);
	assert(*accessed);
	if (is_const)
		throw JSError("Can't assign to const variable");

	Value *right_value = right->evaluate(context);

	switch (type) {
		case Type::Assignment:
			return *accessed = right_value;
		case Type::AdditionAssignment:
			return *accessed = **accessed + *right_value;
		case Type::SubtractionAssignment:
			return *accessed = **accessed - *right_value;
		case Type::MultiplicationAssignment:
			return *accessed = **accessed * *right_value;
		case Type::DivisionAssignment:
			return *accessed = **accessed / *right_value;
		case Type::ModuloAssignment:
			return *accessed = **accessed % *right_value;
		case Type::BitwiseAndAssignment:
			return *accessed = **accessed & *right_value;
		case Type::BitwiseOrAssignment:
			return *accessed = **accessed | *right_value;
		case Type::BitwiseXorAssignment:
			return *accessed = **accessed ^ *right_value;
		case Type::ExponentiationAssignment:
			return *accessed = (*accessed)->power(*right_value);
		case Type::LogicalAndAssignment:
			return *accessed = **accessed && *right_value;
		case Type::LogicalOrAssignment:
			return *accessed = **accessed || *right_value;
		case Type::LeftShiftAssignment:
			return *accessed = **accessed << *right_value;
		case Type::RightShiftArithmeticAssignment:
			return *accessed = (*accessed)->shiftRightArithmetic(*right_value);
		case Type::RightShiftLogicalAssignment:
			return *accessed = (*accessed)->shiftRightLogical(*right_value);
		default:
			throw std::logic_error("Unsupported assignment operator: " + std::to_string(static_cast<int>(type)));
	}
}

BinaryExpression::Type BinaryExpression::getType(int symbol) {
	switch (symbol) {
		case JSTOK_TEQ:   return Type::TripleEquals;
		case JSTOK_NTEQ:  return Type::TripleNotEquals;
		case JSTOK_PLUS:  return Type::Addition;
		case JSTOK_MINUS: return Type::Subtraction;

		default:
			throw std::invalid_argument("Unknown symbol in BinaryExpression::getType: " +
				std::string(jsParser.getName(symbol)));
	}
}

UnaryExpression::UnaryExpression(const ASTNode &node):
	type(getType(node.symbol)),
	subexpr(Expression::create(*node.front())) {}

Value * UnaryExpression::evaluate(Context &context) {
	throw Unimplemented();
}

UnaryExpression::Type UnaryExpression::getType(int symbol) {
	switch (symbol) {
		default:
			throw std::invalid_argument("Unknown symbol in UnaryExpression::getType: " +
				std::string(jsParser.getName(symbol)));
	}
}

// Value ** AssignmentExpressionNode::access(Context &context, bool *const_out) {
// 	if (left->getType() == Node::Type::Identifier) {
// 		auto *identifier = dynamic_cast<IdentifierNode *>(left.get());
// 		return context.stack.lookup(identifier->name, const_out);
// 	}

// 	throw std::logic_error("Assignment expression LHS type " + std::string(stringify(left->getType())) +
// 		" unsupported");
// }

std::unique_ptr<Expression> Expression::create(const ASTNode &node) {
	switch (node.symbol) {
		case JSTOK_TEQ:
		case JSTOK_NTEQ:
			return std::make_unique<BinaryExpression>(node);

		case JSTOK_PLUS:
		case JSTOK_MINUS:
			if (node.size() == 1)
				return std::make_unique<UnaryExpression>(node);
			return std::make_unique<BinaryExpression>(node);

		case JSTOK_IDENT:
			return std::make_unique<Identifier>(node);

		case JSTOK_NUMBER:
			return std::make_unique<NumberLiteral>(node);

		default:
			node.debug();
			throw std::invalid_argument("Unhandled symbol in Expression::create: " + std::string(node.getName()));
	}
}

std::unique_ptr<Statement> Statement::create(const ASTNode &node) {
	switch (node.symbol) {
		case JSTOK_LET:
		case JSTOK_CONST:
		case JSTOK_VAR:
			return std::make_unique<VariableDefinitions>(node);
		case JSTOK_IF:
			return std::make_unique<IfStatement>(node);
		case JS_BLOCK:
			return std::make_unique<Block>(node);
		case JSTOK_LPAREN:
			return Expression::create(node);
		default:
			node.debug();
			throw std::invalid_argument("Unhandled symbol in Statement::create: " + std::string(node.getName()));
	}

}

Identifier::Identifier(const ASTNode &node): name(*node.text) {
	absorbPosition(node);
}

Value * Identifier::evaluate(Context &context) {
	if (Value **result = context.stack.lookup(name))
		return *result;

	throw ReferenceError(name, location);
}

NumberLiteral::NumberLiteral(const ASTNode &node): value(parseDouble(*node.text)) {
	absorbPosition(node);
}

Value * NumberLiteral::evaluate(Context &context) {
	return context.makeValue<Number>(value);
}