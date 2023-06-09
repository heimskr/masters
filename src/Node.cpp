#include <cassert>
#include <csignal>
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
	context.stack.push();

	for (auto &node: body) {
		const auto [result, value] = node->interpret(context);

		if (result == Result::Return)
			return std::make_pair(result, value);

		if (result == Result::Break)
			throw JSError("Invalid break statement", node->location);

		if (result == Result::Continue)
			throw JSError("Invalid continue statement", node->location);
	}

	context.stack.pop();
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

void Block::findVariables(std::vector<VariableUsage> &usages) const {
	for (const auto &node: body)
		node->findVariables(usages);
}

// void Block::findKilledVariables(std::unordered_set<std::string> &killed) const {
// 	for (const auto &node: body)
// 		node->findKilledVariables(killed);
// }

VariableDefinition::VariableDefinition(const ASTNode &node):
	ident(*node.text),
	value(node.empty()? nullptr : Expression::create(*node.front())) { absorbPosition(node); }

std::pair<Result, Value *> VariableDefinition::interpret(Context &) {
	throw std::logic_error("Unimplemented");
}

void VariableDefinition::findVariables(std::vector<VariableUsage> &usages) const {
	usages.emplace_back(true, ident);
}

// void VariableDefinition::findKilledVariables(std::unordered_set<std::string> &killed) const {
// 	killed.insert(ident);
// }

VariableDefinitions::VariableDefinitions(const ASTNode &node): kind(getKind(node.at(0)->symbol)) {
	absorbPosition(node);

	for (const auto *subnode: *node.front())
		definitions.push_back(std::make_unique<VariableDefinition>(*subnode));
}

std::pair<Result, Value *> VariableDefinitions::interpret(Context &context) {
	for (const auto &definition: definitions) {
		const auto &name = definition->ident;
		const bool must_be_unique = kind == DeclarationKind::Let || kind == DeclarationKind::Const;

		if (must_be_unique && context.stack.inLastScope(name))
			throw std::runtime_error("Name \"" + name + "\" already exists in deepest scope");

		if (definition->value) {
			context.stack.insert(name, definition->value->evaluate(context));
		} else {
			context.stack.insert(name, context.makeValue<Undefined>());
		}
	}

	return {Result::None, nullptr};
}

void VariableDefinitions::findVariables(std::vector<VariableUsage> &usages) const {
	for (const auto &definition: definitions)
		definition->findVariables(usages);
}

// void VariableDefinitions::findKilledVariables(std::unordered_set<std::string> &killed) const {
// 	for (const auto &definition: definitions)
// 		definition->findKilledVariables(killed);
// }

IfStatement::IfStatement(const ASTNode &node):
	condition(Expression::create(*node.at(0))),
	consequent(Statement::create(*node.at(1))),
	alternate(2 < node.size()? Statement::create(*node.at(2)) : nullptr) { absorbPosition(node); }

std::pair<Result, Value *> IfStatement::interpret(Context &context) {
	if (*condition->evaluate(context))
		return consequent->interpret(context);
	if (alternate)
		return alternate->interpret(context);
	return {Result::None, nullptr};
}

void IfStatement::findVariables(std::vector<VariableUsage> &usages) const {
	assert(condition);
	condition->findVariables(usages);
}

WhileLoop::WhileLoop(const ASTNode &node):
	condition(Expression::create(*node.at(0))),
	body(Statement::create(*node.at(1))) { absorbPosition(node); }

std::pair<Result, Value *> WhileLoop::interpret(Context &context) {
	while (condition->evaluate(context)) {
		const auto [result, value] = body->interpret(context);
		if (result == Result::Continue)
			continue;
		if (result == Result::Break)
			return {Result::None, value};
		if (result == Result::Return)
			return {Result::Return, value};
	}

	return {Result::None, nullptr};
}

void WhileLoop::findVariables(std::vector<VariableUsage> &usages) const {
	assert(condition);
	condition->findVariables(usages);
}

std::pair<Result, Value *> Continue::interpret(Context &) {
	return {Result::Continue, nullptr};
}

std::pair<Result, Value *> Break::interpret(Context &) {
	return {Result::Break, nullptr};
}

Return::Return(const ASTNode &node):
	returnValue(node.empty()? nullptr : Expression::create(*node.front())) { absorbPosition(node); }

std::pair<Result, Value *> Return::interpret(Context &context) {
	if (returnValue)
		return {Result::Return, returnValue->evaluate(context)};
	return {Result::Return, context.makeValue<Undefined>()};
}

void Return::findVariables(std::vector<VariableUsage> &usages) const {
	if (returnValue)
		returnValue->findVariables(usages);
}

std::pair<Result, Value *> Expression::interpret(Context &context) {
	return {Result::None, evaluate(context)};
}

BinaryExpression::BinaryExpression(const ASTNode &node):
	type(getType(node.symbol)),
	left(Expression::create(*node.at(0))),
	right(Expression::create(*node.at(1))) { absorbPosition(node); }

Value * BinaryExpression::evaluate(Context &context) {
	switch (type) {
		case Type::TripleEquals:
			return *left->evaluate(context) == *right->evaluate(context);

		case Type::TripleNotEquals:
			return *left->evaluate(context) != *right->evaluate(context);

		case Type::Addition:
			return *left->evaluate(context) + *right->evaluate(context);

		case Type::Subtraction:
			return *left->evaluate(context) - *right->evaluate(context);

		case Type::LessThan:
			return *left->evaluate(context) < *right->evaluate(context);

		case Type::LessThanOrEqual:
			return *left->evaluate(context) <= *right->evaluate(context);

		case Type::GreaterThan:
			return *left->evaluate(context) > *right->evaluate(context);

		case Type::GreaterThanOrEqual:
			return *left->evaluate(context) >= *right->evaluate(context);

		case Type::Modulo:
			return *left->evaluate(context) % *right->evaluate(context);

		case Type::Multiplication:
			return *left->evaluate(context) * *right->evaluate(context);

		case Type::Division:
			return *left->evaluate(context) / *right->evaluate(context);

		case Type::Exponentiation:
			return left->evaluate(context)->power(*right->evaluate(context));

		default:
			throw Unimplemented("Can't evaluate BinaryExpression with type " + std::to_string(static_cast<int>(type)) +
				": unimplemented");
	}
}

Value ** BinaryExpression::access(Context &, bool * /*is_const*/) {
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
		case JSTOK_LT:    return Type::LessThan;
		case JSTOK_LTE:   return Type::LessThanOrEqual;
		case JSTOK_GT:    return Type::GreaterThan;
		case JSTOK_GTE:   return Type::GreaterThanOrEqual;
		case JSTOK_MOD:   return Type::Modulo;
		case JSTOK_TIMES: return Type::Multiplication;
		case JSTOK_DIV:   return Type::Division;
		case JSTOK_EXP:   return Type::Exponentiation;

		default:
			throw std::invalid_argument("Unknown symbol in BinaryExpression::getType: " +
				std::string(jsParser.getName(symbol)));
	}
}

void BinaryExpression::findVariables(std::vector<VariableUsage> &usages) const {
	left->findVariables(usages);
	right->findVariables(usages);
}

UnaryExpression::UnaryExpression(const ASTNode &node):
	type(getType(node.symbol)),
	subexpr(Expression::create(*node.front())) { absorbPosition(node); }

Value * UnaryExpression::evaluate(Context &context) {
	switch (type) {
		case Type::Plus:
			return subexpr->evaluate(context)->toNumber();

		case Type::LogicalNot:
			return context.makeValue<Boolean>(!*subexpr->evaluate(context));

		case Type::Negation: {
			auto *number = subexpr->evaluate(context)->toNumber();
			number->number = -number->number;
			return number;
		}

		case Type::PrefixIncrement:
		case Type::PrefixDecrement:
		case Type::PostfixIncrement:
		case Type::PostfixDecrement: {
			auto *lvalue = dynamic_cast<LValueExpression *>(subexpr.get());
			assert(lvalue != nullptr);
			auto *subvalue = lvalue->evaluate(context);
			assert(subvalue);
			assert(lvalue->referenced);
			if (subvalue->getType() != ValueType::Number)
				return context.makeValue<Number>(nan(""));
			auto *number = dynamic_cast<Number *>(subvalue);
			double new_value = nan("");
			switch (type) {
				case Type::PrefixIncrement:  new_value = ++number->number; break;
				case Type::PrefixDecrement:  new_value = --number->number; break;
				case Type::PostfixIncrement: new_value = number->number++; break;
				case Type::PostfixDecrement: new_value = number->number--; break;
				default: std::terminate();
			}
			auto *out = context.makeValue<Number>(new_value);
			*lvalue->referenced = number;
			return out;
		}

		default:
			throw Unimplemented("Can't evaluate UnaryExpression with type " + std::to_string(static_cast<int>(type)) +
				": unimplemented");
	}
}

UnaryExpression::Type UnaryExpression::getType(int symbol) {
	switch (symbol) {
		case JSTOK_PLUS:       return Type::Plus;
		case JSTOK_MINUS:      return Type::Negation;
		case JSTOK_PLUSPLUS:   return Type::PrefixIncrement;
		case JSTOK_MINUSMINUS: return Type::PrefixDecrement;
		case JS_POSTPLUS:      return Type::PostfixIncrement;
		case JS_POSTMINUS:     return Type::PostfixDecrement;
		case JSTOK_NOT:        return Type::LogicalNot;
		default:
			throw std::invalid_argument("Unknown symbol in UnaryExpression::getType: " +
				std::string(jsParser.getName(symbol)));
	}
}

void UnaryExpression::findVariables(std::vector<VariableUsage> &usages) const {
	subexpr->findVariables(usages);
}

std::unique_ptr<Expression> Expression::create(const ASTNode &node) {
	switch (node.symbol) {
		case JSTOK_TEQ:
		case JSTOK_NTEQ:
		case JSTOK_LT:
		case JSTOK_LTE:
		case JSTOK_GT:
		case JSTOK_GTE:
		case JSTOK_MOD:
		case JSTOK_TIMES:
		case JSTOK_DIV:
		case JSTOK_EXP:
			return std::make_unique<BinaryExpression>(node);

		case JSTOK_PLUS:
		case JSTOK_MINUS:
			if (node.size() == 1)
				return std::make_unique<UnaryExpression>(node);
			return std::make_unique<BinaryExpression>(node);

		case JSTOK_PLUSPLUS:
		case JS_POSTPLUS:
		case JSTOK_MINUSMINUS:
		case JS_POSTMINUS:
		case JSTOK_NOT:
			return std::make_unique<UnaryExpression>(node);

		case JSTOK_IDENT:
			return std::make_unique<Identifier>(node);

		case JSTOK_NUMBER:
			return std::make_unique<NumberLiteral>(node);

		case JSTOK_STRING:
			return std::make_unique<StringLiteral>(node);

		case JSTOK_LPAREN:
			return std::make_unique<FunctionCall>(node);

		case JSTOK_TRUE:
		case JSTOK_FALSE:
			return std::make_unique<BooleanLiteral>(node);

		case JSTOK_FUNCTION:
			return std::make_unique<FunctionExpression>(node);

		case JS_OBJECT:
			return std::make_unique<ObjectExpression>(node);

		case JSTOK_PERIOD:
			return std::make_unique<DotExpression>(node);

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
		case JSTOK_WHILE:
			return std::make_unique<WhileLoop>(node);
		case JSTOK_CONTINUE:
			return std::make_unique<Continue>();
		case JSTOK_BREAK:
			return std::make_unique<Break>();
		case JSTOK_RETURN:
			return std::make_unique<Return>(node);
		case JS_BLOCK:
			return std::make_unique<Block>(node);
		case JSTOK_LPAREN:
		case JSTOK_FUNCTION:
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
		return context.makeValue<Reference>(result);

	throw ReferenceError(name, location);
}

void Identifier::findVariables(std::vector<VariableUsage> &usages) const {
	usages.emplace_back(false, name);
}

NumberLiteral::NumberLiteral(const ASTNode &node): value(parseDouble(*node.text)) {
	absorbPosition(node);
}

Value * NumberLiteral::evaluate(Context &context) {
	return context.makeValue<Number>(value);
}

StringLiteral::StringLiteral(const ASTNode &node): value(node.unquote()) {
	absorbPosition(node);
}

Value * StringLiteral::evaluate(Context &context) {
	return context.makeValue<String>(value);
}

BooleanLiteral::BooleanLiteral(const ASTNode &node): value(node.symbol == JSTOK_TRUE) {
	absorbPosition(node);
}

Value * BooleanLiteral::evaluate(Context &context) {
	return context.makeValue<Boolean>(value);
}

FunctionCall::FunctionCall(const ASTNode &node): function(Expression::create(*node.front())) {
	for (const auto *subnode: *node.at(1))
		arguments.emplace_back(Expression::create(*subnode));
}

Value * FunctionCall::evaluate(Context &context) {
	assert(function);

	Value *evaluated_function = function->evaluate(context);
	assert(evaluated_function != nullptr);

	if (evaluated_function->ultimateType() != ValueType::Function)
		throw TypeError('"' + static_cast<std::string>(*evaluated_function) + "\" is not a function");

	auto &cast_function = dynamic_cast<Function &>(*evaluated_function->ultimateValue());

	std::vector<Value *> argument_values;
	argument_values.reserve(arguments.size());

	for (const auto &argument: arguments)
		argument_values.emplace_back(argument->evaluate(context));

	return cast_function.function(context, argument_values, cast_function.thisObj);
}

void FunctionCall::findVariables(std::vector<VariableUsage> &usages) const {
	if (function)
		function->findVariables(usages);
	for (const auto &argument: arguments)
		argument->findVariables(usages);
}

FunctionExpression::FunctionExpression(const ASTNode &node):
name(2 < node.size()? *node.at(2)->text : ""),
body(std::make_unique<Block>(*node.at(1))) {
	for (const auto *subnode: *node.front())
		arguments.push_back(std::make_unique<Identifier>(*subnode));
}

Value * FunctionExpression::evaluate(Context &context) {
	Value **this_obj = nullptr;

	if (context.nextThis != nullptr)
		this_obj = &context.nextThis;
	else
	 	context.stack.lookup("this");

	return context.makeValue<Function>([this](Context &context, const std::vector<Value *> &argument_values, Value *this_obj) {
		context.stack.push();

		size_t i = 0;

		for (; i < std::min(arguments.size(), argument_values.size()); ++i)
			context.stack.insert(arguments.at(i)->name, argument_values.at(i));

		for (; i < arguments.size(); ++i)
			context.stack.insert(arguments.at(i)->name, context.makeValue<Undefined>());

		context.stack.insert("arguments", context.makeValue<Array>(argument_values));
		context.stack.insert("this", this_obj);

		const auto [result, value] = body->interpret(context);

		context.stack.pop();

		return value;
	}, this_obj != nullptr? *this_obj : nullptr, assembleClosure(context));
}

std::pair<Result, Value *> FunctionExpression::interpret(Context &context) {
	if (!name.empty())
		context.stack.insert(name, evaluate(context));
	return {Result::None, evaluate(context)};
}

void FunctionExpression::findVariables(std::vector<VariableUsage> &usages) const {
	if (!name.empty())
		usages.emplace_back(true, name);
}

std::unordered_set<Value *> FunctionExpression::assembleClosure(Context &context) const {
	std::vector<VariableUsage> usages;
	body->findVariables(usages);

	std::unordered_set<Value *> closure;
	std::unordered_set<std::string> killed;

	for (const auto &[was_killed, variable]: usages) {
		if (was_killed) {
			killed.insert(variable);
		} else if (!killed.contains(variable)) {
			Value **found = context.stack.lookup(variable);
			if (found != nullptr) {
				assert(*found != nullptr);
				closure.insert(*found);
			}
		}
	}

	return closure;
}

ObjectExpression::ObjectExpression(const ASTNode &node) {
	if (!node.empty())
		for (const auto &subnode: *node.front())
			map[*subnode->text] = Expression::create(*subnode->front());
}

Value * ObjectExpression::evaluate(Context &context) {
	auto *out = context.makeValue<Object>();
	FieldSaver saver(context, &Context::nextThis);
	context.nextThis = out;
	for (const auto &[key, expression]: map)
		out->map[key] = expression->evaluate(context);
	return out;
}

void ObjectExpression::findVariables(std::vector<VariableUsage> &usages) const {
	for (const auto &[key, expression]: map)
		expression->findVariables(usages);
}

DotExpression::DotExpression(const ASTNode &node):
	base(Expression::create(*node.at(0))),
	ident(*node.at(1)->text) { absorbPosition(node); }

Value * DotExpression::evaluate(Context &context) {
	auto *lhs = base->evaluate(context);
	// TODO: update when boxing support is added
	if (lhs->ultimateType() != ValueType::Object)
		throw TypeError("Can't use . operator on non-object", location);

	auto *object = dynamic_cast<Object *>(lhs->ultimateValue());

	if (auto iter = object->map.find(ident); iter != object->map.end())
		return context.makeValue<Reference>(&iter->second);

	return context.makeValue<Undefined>();
}

void DotExpression::findVariables(std::vector<VariableUsage> &usages) const {
	assert(base != nullptr);
	base->findVariables(usages);
}
