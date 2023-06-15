#include <cassert>
#include <csignal>
#include <cmath>
#include <iomanip>
#include <iostream>

#include "ASTNode.h"
#include "Errors.h"
#include "JS.h"
#include "Lexer.h"
#include "Log.h"
#include "Node.h"
#include "Parser.h"
#include "Value.h"

DeclarationKind getKind(int symbol) {
	switch (symbol) {
		case JSTOK_CONST: return DeclarationKind::Const;
		case JSTOK_LET:   return DeclarationKind::Let;
		case JSTOK_VAR:   return DeclarationKind::Var;
		default:
			return DeclarationKind::Invalid;
	}
}

//                                 ______                              _
//      /\                        |  ____|                            (_)
//     /  \   ___ ___ ___  ___ ___| |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//    / /\ \ / __/ __/ _ \/ __/ __|  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//   / ____ \ (_| (_|  __/\__ \__ \ |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//  /_/    \_\___\___\___||___/___/______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//                                            | |
//                                            |_|

AccessExpression::AccessExpression(const ASTNode &node):
	base(Expression::create(*node.at(0))),
	subscript(Expression::create(*node.at(1))) { absorbPosition(node); }

Value * AccessExpression::evaluate(Context &context) {
	Value *lhs = nullptr;
	Value *rhs = nullptr;

	{
		auto saver = context.writeMember(false);
		lhs = base->evaluate(context);
		assert(lhs != nullptr);
	}

	if (!lhs->subscriptable())
		throw TypeError("Can't use [] operator on non-object/array/string (" + lhs->getName() + ')', location);

	{
		auto saver = context.writeMember(false);
		rhs = subscript->evaluate(context);
		assert(rhs != nullptr);
	}

	switch (lhs->ultimateType()) {
		case ValueType::Array: {
			// TODO: verify
			if (lhs->getType() != ValueType::Reference)
				return context.makeValue<Undefined>();

			auto &array = dynamic_cast<Array &>(*lhs->ultimateValue());

			bool is_int = false;
			size_t index = -1;

			if (rhs->ultimateType() == ValueType::Number) {
				const auto number = static_cast<double>(*rhs->ultimateValue());
				double intpart = 0.;
				if (std::modf(number, &intpart) == 0.) {
					is_int = true;
					index = static_cast<size_t>(intpart);
				}
			}

			if (!is_int)
				return access(context, &array, static_cast<std::string>(*rhs));

			if (context.writingMember)
				return array.fetchOrMake(index);

			if (auto *fetched = array[index])
				return fetched;

			return context.makeReference<Undefined>();
		}

		case ValueType::String: {
			if (rhs->ultimateType() == ValueType::Number) {
				const auto number = static_cast<double>(*rhs->ultimateValue());
				double intpart = 0.;
				if (std::modf(number, &intpart) == 0.) {
					auto &string = dynamic_cast<String &>(*lhs->ultimateValue());
					const auto index = static_cast<size_t>(number);
					if (index < string.string.size())
						return context.toValue(std::string(1, string.string.at(index)));
					return context.makeValue<Undefined>();
				}
			}

			return access(context, lhs, static_cast<std::string>(*rhs));
		}

		default:
			return access(context, lhs->ultimateValue(), static_cast<std::string>(*rhs));
	}
}

void AccessExpression::findVariables(std::vector<VariableUsage> &usages) const {
	assert(base != nullptr);
	assert(subscript != nullptr);
	base->findVariables(usages);
	subscript->findVariables(usages);
}

bool AccessExpression::doDelete(Context &context) {
	Value *lhs = nullptr;
	Value *rhs = nullptr;

	{
		auto saver = context.writeMember(false);
		lhs = base->evaluate(context);
		assert(lhs != nullptr);
	}

	if (!lhs->subscriptable())
		throw TypeError("Can't use [] operator on non-object/array/string (" + lhs->getName() + ')', location);

	{
		auto saver = context.writeMember(false);
		rhs = subscript->evaluate(context);
		assert(rhs != nullptr);
	}

	return lhs->doDelete(rhs);
}

//                                 ______                              _
//      /\                        |  ____|                            (_)
//     /  \   _ __ _ __ __ _ _   _| |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//    / /\ \ | '__| '__/ _` | | | |  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//   / ____ \| |  | | | (_| | |_| | |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//  /_/    \_\_|  |_|  \__,_|\__, |______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//                            __/ |           | |
//                           |___/            |_|

ArrayExpression::ArrayExpression(const ASTNode &node) {
	if (!node.empty()) {
		for (const auto &subnode: *node.front()) {
			if (subnode->symbol == JS_EMPTY) {
				isHoley = true;
				expressions.emplace_back(nullptr);
			} else {
				expressions.push_back(Expression::create(*subnode));
			}
		}
	}
}

Value * ArrayExpression::evaluate(Context &context) {
	if (isHoley) {
		Array::Holey map;
		size_t i = 0;
		for (const auto &expression: expressions) {
			if (expression)
				map[i] = context.makeReference(expression->evaluate(context));
			++i;
		}
		return context.makeValue<Array>(std::move(map), i);
	}

	Array::Holeless values;
	for (const auto &expression: expressions) {
		assert(expression);
		values.push_back(context.makeReference(expression->evaluate(context)));
	}
	return context.makeValue<Array>(std::move(values));
}

void ArrayExpression::findVariables(std::vector<VariableUsage> &usages) const {
	for (const auto &expression: expressions)
		if (expression)
			expression->findVariables(usages);
}

//   ____  _                        ______                              _
//  |  _ \(_)                      |  ____|                            (_)
//  | |_) |_ _ __   __ _ _ __ _   _| |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//  |  _ <| | '_ \ / _` | '__| | | |  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//  | |_) | | | | | (_| | |  | |_| | |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//  |____/|_|_| |_|\__,_|_|   \__, |______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//                             __/ |           | |
//                            |___/            |_|

BinaryExpression::BinaryExpression(const ASTNode &node):
	type(getType(node.symbol)),
	left(Expression::create(*node.at(0))),
	right(Expression::create(*node.at(1))) { absorbPosition(node); }

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

		case Type::BitwiseAnd:
			return *left->evaluate(context) & *right->evaluate(context);

		case Type::BitwiseOr:
			return *left->evaluate(context) | *right->evaluate(context);

		case Type::BitwiseXor:
			return *left->evaluate(context) ^ *right->evaluate(context);

		case Type::LeftShift:
			return *left->evaluate(context) << *right->evaluate(context);

		case Type::RightShiftArithmetic:
			return left->evaluate(context)->shiftRightArithmetic(*right->evaluate(context));

		case Type::RightShiftLogical:
			return left->evaluate(context)->shiftRightLogical(*right->evaluate(context));

		case Type::Exponentiation:
			return left->evaluate(context)->power(*right->evaluate(context));

		case Type::LogicalAnd:
			if (!*left->evaluate(context))
				return context.toValue(false);
			return context.toValue(static_cast<bool>(*right->evaluate(context)));

		case Type::LogicalOr:
			if (*left->evaluate(context))
				return context.toValue(true);
			return context.toValue(static_cast<bool>(*right->evaluate(context)));

		case Type::Comma:
			left->evaluate(context);
			return right->evaluate(context);

		case Type::In:
			return right->evaluate(context)->contains(*left->evaluate(context));

		case Type::Instanceof:
			throw Unimplemented();

		case Type::Assignment:
		case Type::AdditionAssignment:
		case Type::SubtractionAssignment:
		case Type::MultiplicationAssignment:
		case Type::DivisionAssignment:
		case Type::ModuloAssignment:
		case Type::BitwiseAndAssignment:
		case Type::BitwiseOrAssignment:
		case Type::BitwiseXorAssignment:
		case Type::ExponentiationAssignment:
		case Type::LogicalAndAssignment:
		case Type::LogicalOrAssignment:
		case Type::LeftShiftAssignment:
		case Type::RightShiftArithmeticAssignment:
		case Type::RightShiftLogicalAssignment:
			return evaluateAccess(context);

		case Type::Invalid:
			break;
	}

	throw Unimplemented("Can't evaluate BinaryExpression with type " + std::to_string(static_cast<int>(type)) +
		": unimplemented");
}

Value * BinaryExpression::evaluateAccess(Context &context) {
	auto saver = context.writeMember();

	bool is_const = false;

	Value *lhs = left->evaluate(context);
	auto *reference = lhs->cast<Reference>();

	if (!reference) {
		WARN("LHS of BinaryExpression isn't a reference, but instead " << lhs->getName() << ": " << *lhs);
		return right->evaluate(context);
	}

	assert(reference != nullptr);
	Reference *original_this = reference->referenceContext.thisObj;
	reference = reference->unwrap();
	assert(reference != nullptr);

	if (is_const)
		throw JSError("Can't assign to const variable");

	Value *right_value = right->evaluate(context);

	Value *left_value = reference;
	Value *out_value = nullptr;

	auto *function = reference->ultimateValue()->cast<Function>();
	if (function != nullptr && function->isProperty) {
		assert(original_this != nullptr);
		left_value = FunctionCaller::call(function, context, {}, original_this);
	}

	switch (type) {
		case Type::Assignment:
			out_value = right_value;
			break;
		case Type::AdditionAssignment:
			out_value = *left_value + *right_value;
			break;
		case Type::SubtractionAssignment:
			out_value = *left_value - *right_value;
			break;
		case Type::MultiplicationAssignment:
			out_value = *left_value * *right_value;
			break;
		case Type::DivisionAssignment:
			out_value = *left_value / *right_value;
			break;
		case Type::ModuloAssignment:
			out_value = *left_value % *right_value;
			break;
		case Type::BitwiseAndAssignment:
			out_value = *left_value & *right_value;
			break;
		case Type::BitwiseOrAssignment:
			out_value = *left_value | *right_value;
			break;
		case Type::BitwiseXorAssignment:
			out_value = *left_value ^ *right_value;
			break;
		case Type::ExponentiationAssignment:
			out_value = left_value->power(*right_value);
			break;
		case Type::LogicalAndAssignment:
			out_value = *left_value && *right_value;
			break;
		case Type::LogicalOrAssignment:
			out_value = *left_value || *right_value;
			break;
		case Type::LeftShiftAssignment:
			out_value = *left_value << *right_value;
			break;
		case Type::RightShiftArithmeticAssignment:
			out_value = left_value->shiftRightArithmetic(*right_value);
			break;
		case Type::RightShiftLogicalAssignment:
			out_value = left_value->shiftRightLogical(*right_value);
			break;
		default:
			throw std::logic_error("Unsupported assignment operator: " + std::to_string(static_cast<int>(type)));
	}

	assert(out_value != nullptr);

	if (function != nullptr && function->isProperty) {
		assert(original_this != nullptr);
		return FunctionCaller::call(function, context, {out_value}, original_this);
	}

	reference->referent = out_value;
	return reference;
}

BinaryExpression::Type BinaryExpression::getType(int symbol) {
	switch (symbol) {
		case JSTOK_TEQ:        return Type::TripleEquals;
		case JSTOK_NTEQ:       return Type::TripleNotEquals;
		case JSTOK_PLUS:       return Type::Addition;
		case JSTOK_MINUS:      return Type::Subtraction;
		case JSTOK_LT:         return Type::LessThan;
		case JSTOK_LTE:        return Type::LessThanOrEqual;
		case JSTOK_GT:         return Type::GreaterThan;
		case JSTOK_GTE:        return Type::GreaterThanOrEqual;
		case JSTOK_MOD:        return Type::Modulo;
		case JSTOK_TIMES:      return Type::Multiplication;
		case JSTOK_DIV:        return Type::Division;
		case JSTOK_EXP:        return Type::Exponentiation;
		case JSTOK_ASSIGN:     return Type::Assignment;
		case JSTOK_COMMA:      return Type::Comma;
		case JSTOK_PLUSEQ:     return Type::AdditionAssignment;
		case JSTOK_MINUSEQ:    return Type::SubtractionAssignment;
		case JSTOK_DIVEQ:      return Type::DivisionAssignment;
		case JSTOK_TIMESEQ:    return Type::MultiplicationAssignment;
		case JSTOK_MODEQ:      return Type::ModuloAssignment;
		case JSTOK_SRAEQ:      return Type::RightShiftArithmeticAssignment;
		case JSTOK_SRLEQ:      return Type::RightShiftLogicalAssignment;
		case JSTOK_SLEQ:       return Type::LeftShiftAssignment;
		case JSTOK_ANDEQ:      return Type::BitwiseAndAssignment;
		case JSTOK_LANDEQ:     return Type::LogicalAndAssignment;
		case JSTOK_OREQ:       return Type::BitwiseOrAssignment;
		case JSTOK_LOREQ:      return Type::LogicalOrAssignment;
		case JSTOK_XOREQ:      return Type::BitwiseXorAssignment;
		case JSTOK_EXPEQ:      return Type::ExponentiationAssignment;
		case JSTOK_LAND:       return Type::LogicalAnd;
		case JSTOK_LOR:        return Type::LogicalOr;
		case JSTOK_OR:         return Type::BitwiseOr;
		case JSTOK_XOR:        return Type::BitwiseXor;
		case JSTOK_AND:        return Type::BitwiseAnd;
		case JSTOK_IN:         return Type::In;
		case JSTOK_INSTANCEOF: return Type::Instanceof;
		default:
			throw std::invalid_argument("Unknown symbol in BinaryExpression::getType: " +
				std::string(jsParser.getName(symbol)));
	}
}

void BinaryExpression::findVariables(std::vector<VariableUsage> &usages) const {
	left->findVariables(usages);
	right->findVariables(usages);
}

//   ____  _            _
//  |  _ \| |          | |
//  | |_) | | ___   ___| | __
//  |  _ <| |/ _ \ / __| |/ /
//  | |_) | | (_) | (__|   <
//  |____/|_|\___/ \___|_|\_\.

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

//   ____              _                  _      _ _                 _
//  |  _ \            | |                | |    (_) |               | |
//  | |_) | ___   ___ | | ___  __ _ _ __ | |     _| |_ ___ _ __ __ _| |
//  |  _ < / _ \ / _ \| |/ _ \/ _` | '_ \| |    | | __/ _ \ '__/ _` | |
//  | |_) | (_) | (_) | |  __/ (_| | | | | |____| | ||  __/ | | (_| | |
//  |____/ \___/ \___/|_|\___|\__,_|_| |_|______|_|\__\___|_|  \__,_|_|

BooleanLiteral::BooleanLiteral(const ASTNode &node): value(node.symbol == JSTOK_TRUE) {
	absorbPosition(node);
}

Value * BooleanLiteral::evaluate(Context &context) {
	return context.makeValue<Boolean>(value);
}

//   ____                 _
//  |  _ \               | |
//  | |_) |_ __ ___  __ _| | __
//  |  _ <| '__/ _ \/ _` | |/ /
//  | |_) | | |  __/ (_| |   <
//  |____/|_|  \___|\__,_|_|\_\.


std::pair<Result, Value *> Break::interpret(Context &) {
	return {Result::Break, nullptr};
}

//    _____            _   _
//   / ____|          | | (_)
//  | |     ___  _ __ | |_ _ _ __  _   _  ___
//  | |    / _ \| '_ \| __| | '_ \| | | |/ _ \.
//  | |___| (_) | | | | |_| | | | | |_| |  __/
//   \_____\___/|_| |_|\__|_|_| |_|\__,_|\___|

std::pair<Result, Value *> Continue::interpret(Context &) {
	return {Result::Continue, nullptr};
}

//   _____       _      _       ______                              _
//  |  __ \     | |    | |     |  ____|                            (_)
//  | |  | | ___| | ___| |_ ___| |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//  | |  | |/ _ \ |/ _ \ __/ _ \  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//  | |__| |  __/ |  __/ ||  __/ |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//  |_____/ \___|_|\___|\__\___|______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//                                         | |
//                                         |_|

DeleteExpression::DeleteExpression(const ASTNode &node):
	toDelete(Expression::create(*node.front())) { absorbPosition(node); }

Value * DeleteExpression::evaluate(Context &context) {
	return context.toValue(toDelete->doDelete(context));
}

void DeleteExpression::findVariables(std::vector<VariableUsage> &usages) const {
	toDelete->findVariables(usages);
}

//   _____        _   ______                              _
//  |  __ \      | | |  ____|                            (_)
//  | |  | | ___ | |_| |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//  | |  | |/ _ \| __|  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//  | |__| | (_) | |_| |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//  |_____/ \___/ \__|______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//                               | |
//                               |_|

DotExpression::DotExpression(const ASTNode &node):
	base(Expression::create(*node.at(0))),
	ident(*node.at(1)->text) { absorbPosition(node); }

Value * DotExpression::evaluate(Context &context) {
	Value *lhs = nullptr;

	{
		auto saver = context.writeMember(false);
		lhs = base->evaluate(context);
	}

	return access(context, lhs->ultimateValue(), ident);
}

void DotExpression::findVariables(std::vector<VariableUsage> &usages) const {
	assert(base != nullptr);
	base->findVariables(usages);
}

bool DotExpression::doDelete(Context &context) {
	Value *lhs = nullptr;

	{
		auto saver = context.writeMember(false);
		lhs = base->evaluate(context);
		assert(lhs != nullptr);
	}

	if (!lhs->subscriptable())
		throw TypeError("Can't use [] operator on non-object/array/string (" + lhs->getName() + ')', location);

	return lhs->doDelete(ident);
}

//   ______                              _
//  |  ____|                            (_)
//  | |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//  |  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//  | |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//  |______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//              | |
//              |_|

std::pair<Result, Value *> Expression::interpret(Context &context) {
	return {Result::None, evaluate(context)};
}

std::unique_ptr<Expression> Expression::create(const ASTNode &node) {
	std::unique_ptr<Expression> out;

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
		case JSTOK_ASSIGN:
		case JSTOK_COMMA:
		case JSTOK_PLUSEQ:
		case JSTOK_MINUSEQ:
		case JSTOK_DIVEQ:
		case JSTOK_TIMESEQ:
		case JSTOK_MODEQ:
		case JSTOK_SRAEQ:
		case JSTOK_SRLEQ:
		case JSTOK_SLEQ:
		case JSTOK_ANDEQ:
		case JSTOK_LANDEQ:
		case JSTOK_OREQ:
		case JSTOK_LOREQ:
		case JSTOK_XOREQ:
		case JSTOK_EXPEQ:
		case JSTOK_LAND:
		case JSTOK_LOR:
		case JSTOK_OR:
		case JSTOK_XOR:
		case JSTOK_AND:
		case JSTOK_IN:
		case JSTOK_INSTANCEOF:
			out = std::make_unique<BinaryExpression>(node);
			break;

		case JSTOK_PLUS:
		case JSTOK_MINUS:
			if (node.size() == 1)
				out = std::make_unique<UnaryExpression>(node);
			else
				out = std::make_unique<BinaryExpression>(node);
			break;

		case JSTOK_PLUSPLUS:
		case JS_POSTPLUS:
		case JSTOK_MINUSMINUS:
		case JS_POSTMINUS:
		case JSTOK_NOT:
			out = std::make_unique<UnaryExpression>(node);
			break;

		case JSTOK_IDENT:
			out = std::make_unique<Identifier>(node);
			break;

		case JSTOK_NUMBER:
		case JSTOK_NAN:
		case JSTOK_INFINITY:
			out = std::make_unique<NumberLiteral>(node);
			break;

		case JSTOK_STRING:
			out = std::make_unique<StringLiteral>(node);
			break;

		case JSTOK_UNDEFINED:
			out = std::make_unique<UndefinedLiteral>(node);
			break;

		case JSTOK_NULL:
			out = std::make_unique<NullLiteral>(node);
			break;

		case JSTOK_LPAREN:
			out = std::make_unique<FunctionCall>(node);
			break;

		case JSTOK_TRUE:
		case JSTOK_FALSE:
			out = std::make_unique<BooleanLiteral>(node);
			break;

		case JSTOK_FUNCTION:
			out = std::make_unique<FunctionExpression>(node);
			break;

		case JS_OBJECT:
			out = std::make_unique<ObjectExpression>(node);
			break;

		case JSTOK_PERIOD:
			out = std::make_unique<DotExpression>(node);
			break;

		case JS_ARRAY:
			out = std::make_unique<ArrayExpression>(node);
			break;

		case JSTOK_LSQUARE:
			out = std::make_unique<AccessExpression>(node);
			break;

		case JSTOK_NEW:
			out = std::make_unique<NewExpression>(node);
			break;

		case JSTOK_DELETE:
			out = std::make_unique<DeleteExpression>(node);
			break;

		default:
			node.debug();
			throw std::invalid_argument("Unhandled symbol in Expression::create: " + std::string(node.getName()));
	}

	out->absorbPosition(node);
	return out;
}

//   ______         _
//  |  ____|       | |
//  | |__ ___  _ __| |     ___   ___  _ __
//  |  __/ _ \| '__| |    / _ \ / _ \| '_ \.
//  | | | (_) | |  | |___| (_) | (_) | |_) |
//  |_|  \___/|_|  |______\___/ \___/| .__/
//                                   | |
//                                   |_|

ForLoop::ForLoop(const ASTNode &node):
	setup(Statement::create(*node.at(0))),
	condition(Expression::create(*node.at(1))),
	postaction(Expression::create(*node.at(2))),
	body(Statement::create(*node.at(3))) { absorbPosition(node); }

std::pair<Result, Value *> ForLoop::interpret(Context &context) {
	body->validateInSingleStatementContext();

	// Unlike in a while loop, pushing/popping the scope stack is necessary because any variable declarations inside
	// the for loop's setup statement can't interfere with the outer scope.

	context.stack.push();
	setup->interpret(context);

	while (*condition->evaluate(context)) {
		const auto [result, value] = body->interpret(context);

		if (result == Result::Break)
			return {Result::None, value};

		if (result == Result::Return)
			return {Result::Return, value};

		postaction->interpret(context);
	}

	context.stack.pop();

	return {Result::None, nullptr};
}

void ForLoop::findVariables(std::vector<VariableUsage> &usages) const {
	assert(condition);
	condition->findVariables(usages);
}

//   ______                _   _              _____      _ _
//  |  ____|              | | (_)            / ____|    | | |
//  | |__ _   _ _ __   ___| |_ _  ___  _ __ | |     __ _| | |
//  |  __| | | | '_ \ / __| __| |/ _ \| '_ \| |    / _` | | |
//  | |  | |_| | | | | (__| |_| | (_) | | | | |___| (_| | | |
//  |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|\_____\__,_|_|_|

FunctionCall::FunctionCall(const ASTNode &node): function(Expression::create(*node.front())) {
	for (const auto *subnode: *node.at(1))
		arguments.emplace_back(Expression::create(*subnode));
}

Value * FunctionCall::evaluate(Context &context) {
	assert(function);

	Value *evaluated_function = function->evaluate(context);
	assert(evaluated_function != nullptr);

	if (evaluated_function->ultimateType() != ValueType::Function)
		throw TypeError("Value " + static_cast<std::string>(*evaluated_function) + " (" + evaluated_function->getName()
			+ ") is not a function");

	auto &cast_function = dynamic_cast<Function &>(*evaluated_function->ultimateValue());

	std::vector<Value *> argument_values;
	argument_values.reserve(arguments.size());

	for (const auto &argument: arguments)
		argument_values.emplace_back(argument->evaluate(context));

	Reference *this_obj = nullptr;

	if (auto *evaluated_reference = evaluated_function->cast<Reference>())
		if (auto *this_from_object = evaluated_reference->referenceContext.thisObj)
			this_obj = this_from_object->unwrap();

	return FunctionCaller::call(&cast_function, context, argument_values, this_obj);
}

void FunctionCall::findVariables(std::vector<VariableUsage> &usages) const {
	if (function)
		function->findVariables(usages);
	for (const auto &argument: arguments)
		argument->findVariables(usages);
}

Value * FunctionCaller::call(Function *function, Context &context, const std::vector<Value *> &arguments,
                             Reference *this_obj) {
	FieldSaver saved_function(context, &Context::currentFunction);
	context.currentFunction = function;

	ClosureGuard guard(context, function->closure);
	if (Value *out = function->function(context, arguments, this_obj == nullptr? function->thisObj : this_obj))
		return out;
	return context.makeValue<Undefined>();
}

//   ______                _   _             ______                              _
//  |  ____|              | | (_)           |  ____|                            (_)
//  | |__ _   _ _ __   ___| |_ _  ___  _ __ | |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//  |  __| | | | '_ \ / __| __| |/ _ \| '_ \|  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//  | |  | |_| | | | | (__| |_| | (_) | | | | |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//  |_|   \__,_|_| |_|\___|\__|_|\___/|_| |_|______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//                                                      | |
//                                                      |_|

FunctionExpression::FunctionExpression(const ASTNode &node):
name(2 < node.size()? *node.at(2)->text : ""),
body(std::make_unique<Block>(*node.at(1))) {
	for (const auto *subnode: *node.front())
		arguments.push_back(std::make_unique<Identifier>(*subnode));
}

Value * FunctionExpression::evaluate(Context &context) {
	Reference *this_obj = nullptr;

	if (context.nextThis != nullptr)
		this_obj = context.nextThis;
	else
		this_obj = context.stack.lookup("this");

	return context.makeValue<Function>([this](Context &context, const std::vector<Value *> &argument_values,
	                                          Reference *this_obj) {
		context.stack.push();

		size_t i = 0;

		for (; i < std::min(arguments.size(), argument_values.size()); ++i)
			context.stack.insert(arguments.at(i)->name, argument_values.at(i));

		for (; i < arguments.size(); ++i)
			context.stack.insert(arguments.at(i)->name, context.makeValue<Undefined>());

		Array::Holeless argument_references;
		for (Value *value: argument_values)
			argument_references.push_back(context.makeReference(value));

		context.stack.insert("arguments", context.makeValue<Array>(argument_references));
		if (this_obj != nullptr)
			context.stack.insert("this", this_obj->referent);
		else
			WARN("this_obj is null");

		const auto [result, value] = body->interpret(context);

		context.stack.pop();

		return value;
	}, this_obj, assembleClosure(context));
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

Closure FunctionExpression::assembleClosure(Context &context) const {
	std::vector<VariableUsage> usages;
	body->findVariables(usages);

	std::unordered_set<Reference *> closure;
	std::unordered_map<std::string, Reference *> closureMap;
	std::unordered_set<std::string> killed;

	for (const auto &[was_killed, variable]: usages) {
		if (variable == "this")
			continue;
		if (was_killed) {
			killed.insert(variable);
		} else if (!killed.contains(variable)) {
			Reference *found = context.stack.lookup(variable);
			if (found != nullptr) {
				assert(found->referent != nullptr);
				closure.insert(found);
				closureMap.emplace(variable, found);
			}
		}
	}

	return {std::move(closure), std::move(closureMap)};
}

//   _____    _            _   _  __ _
//  |_   _|  | |          | | (_)/ _(_)
//    | |  __| | ___ _ __ | |_ _| |_ _  ___ _ __
//    | | / _` |/ _ \ '_ \| __| |  _| |/ _ \ '__|
//   _| || (_| |  __/ | | | |_| | | | |  __/ |
//  |_____\__,_|\___|_| |_|\__|_|_| |_|\___|_|

Identifier::Identifier(const ASTNode &node): name(*node.text) {
	absorbPosition(node);
}

Value * Identifier::evaluate(Context &context) {
	if (Reference *result = context.stack.lookup(name))
		return result;

	throw ReferenceError(name, location);
}

void Identifier::findVariables(std::vector<VariableUsage> &usages) const {
	usages.emplace_back(false, name);
}

bool Identifier::doDelete(Context &) {
	// TODO!: delete global variables, throw an error if deleting let/const'd variable
	return true;
}

//   _____  __ _____ _        _                            _
//  |_   _|/ _/ ____| |      | |                          | |
//    | | | || (___ | |_ __ _| |_ ___ _ __ ___   ___ _ __ | |_
//    | | |  _\___ \| __/ _` | __/ _ \ '_ ` _ \ / _ \ '_ \| __|
//   _| |_| | ____) | || (_| | ||  __/ | | | | |  __/ | | | |_
//  |_____|_||_____/ \__\__,_|\__\___|_| |_| |_|\___|_| |_|\__|

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

//   _   _               ______                              _
//  | \ | |             |  ____|                            (_)
//  |  \| | _____      _| |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//  | . ` |/ _ \ \ /\ / /  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//  | |\  |  __/\ V  V /| |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//  |_| \_|\___| \_/\_/ |______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//                                  | |
//                                  |_|


NewExpression::NewExpression(const ASTNode &node): classExpression(Expression::create(*node.front())) {
	for (const auto *subnode: *node.at(1))
		arguments.emplace_back(Expression::create(*subnode));
}

Value * NewExpression::evaluate(Context &context) {
	Function *constructor = classExpression->evaluate(context)->ultimateValue()->cast<Function>();
	assert(constructor != nullptr);

	Object *object = context.makeValue<Object>();
	object->customPrototype = constructor->getFunctionPrototype();

	std::vector<Value *> argument_values;
	argument_values.reserve(arguments.size());

	for (const auto &argument: arguments)
		argument_values.emplace_back(argument->evaluate(context));

	constructor->function(context, argument_values, context.makeReference(object));
	return object;
}

void NewExpression::findVariables(std::vector<VariableUsage> &usages) const {
	if (classExpression)
		classExpression->findVariables(usages);
	for (const auto &argument: arguments)
		argument->findVariables(usages);
}

//   _   _           _
//  | \ | |         | |
//  |  \| | ___   __| | ___
//  | . ` |/ _ \ / _` |/ _ \.
//  | |\  | (_) | (_| |  __/
//  |_| \_|\___/ \__,_|\___|

std::unique_ptr<Node> Node::fromAST(const ASTNode &node) {
	throw std::invalid_argument("Node::fromAST failed: symbol \"" + std::string(node.getName()) + "\" not handled");
}

void Node::absorbPosition(const ASTNode &node) {
	location = node.location;
}

//   _   _       _ _ _      _ _                 _
//  | \ | |     | | | |    (_) |               | |
//  |  \| |_   _| | | |     _| |_ ___ _ __ __ _| |
//  | . ` | | | | | | |    | | __/ _ \ '__/ _` | |
//  | |\  | |_| | | | |____| | ||  __/ | | (_| | |
//  |_| \_|\__,_|_|_|______|_|\__\___|_|  \__,_|_|

NullLiteral::NullLiteral(const ASTNode &node) {
	absorbPosition(node);
}

Value * NullLiteral::evaluate(Context &context) {
	return context.makeValue<Null>();
}

//   _   _                 _               _      _ _                 _
//  | \ | |               | |             | |    (_) |               | |
//  |  \| |_   _ _ __ ___ | |__   ___ _ __| |     _| |_ ___ _ __ __ _| |
//  | . ` | | | | '_ ` _ \| '_ \ / _ \ '__| |    | | __/ _ \ '__/ _` | |
//  | |\  | |_| | | | | | | |_) |  __/ |  | |____| | ||  __/ | | (_| | |
//  |_| \_|\__,_|_| |_| |_|_.__/ \___|_|  |______|_|\__\___|_|  \__,_|_|

NumberLiteral::NumberLiteral(const ASTNode &node): value(parseDouble(*node.text)) {
	absorbPosition(node);
}

Value * NumberLiteral::evaluate(Context &context) {
	return context.makeValue<Number>(value);
}

//    ____  _     _           _
//   / __ \| |   (_)         | |     /\.
//  | |  | | |__  _  ___  ___| |_   /  \   ___ ___ ___  ___ ___  ___  _ __
//  | |  | | '_ \| |/ _ \/ __| __| / /\ \ / __/ __/ _ \/ __/ __|/ _ \| '__|
//  | |__| | |_) | |  __/ (__| |_ / ____ \ (_| (_|  __/\__ \__ \ (_) | |
//   \____/|_.__/| |\___|\___|\__/_/    \_\___\___\___||___/___/\___/|_|
//              _/ |
//             |__/

Value * ObjectAccessor::access(Context &context, Value *lhs, const std::string &property) {
	assert(lhs != nullptr);

	if (auto *out = access(context, lhs, property, {context.makeReference(lhs)}, context.writingMember)) {
		if (!context.writingMember)
			if (auto *function = out->ultimateValue()->cast<Function>(); function && function->isProperty)
				return FunctionCaller::call(function, context, {}, context.makeReference(lhs));
		return out;
	}

	return context.makeValue<Reference>(context.makeValue<Undefined>(), false);
}

Value * ObjectAccessor::access(Context &context, Value *lhs, const std::string &property,
                               const ReferenceContext &ref_context, bool can_create) {
	assert(lhs != nullptr);

	if (property == "prototype") {
		if (auto *function = lhs->ultimateValue()->cast<Function>())
			return function->getFunctionPrototype();
		if (Object *prototype = lhs->getPrototype(context))
			return prototype;
		return context.makeReference<Undefined>();
	}

	// First, we attempt to find the property without the possibility of creation.
	// If it fails, we try the prototype chain next.
	if (auto *found = lhs->access(property, false))
		return found->withContext(ref_context);

	if (Object *prototype = lhs->getPrototype(context); prototype != nullptr && prototype != lhs) {
		if (auto *out = access(context, prototype, property, ref_context, false)) {
			// If the prototype member is a property function, its calling will be handled by the assignment operator.
			// If not, we'd return the reference anyway. Therefore, we can return early here if this is a writing
			// context.
			if (can_create)
				return out;

			if (auto *function = out->ultimateValue()->cast<Function>(); function && function->isProperty)
				return FunctionCaller::call(function, context, {}, context.makeReference(lhs));

			return out;
		}
	}

	if (can_create)
		if (auto *found = lhs->access(property, true))
			return found->withContext(ref_context);

	return nullptr;
}

bool ObjectAccessor::doDelete(Context &, Value *object, Value *property) {
	assert(object != nullptr);

	if (static_cast<std::string>(*property) == "prototype")
		return true;

	return object->doDelete(property);
}

//    ____  _     _           _   ______                              _
//   / __ \| |   (_)         | | |  ____|                            (_)
//  | |  | | |__  _  ___  ___| |_| |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//  | |  | | '_ \| |/ _ \/ __| __|  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//  | |__| | |_) | |  __/ (__| |_| |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//   \____/|_.__/| |\___|\___|\__|______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//              _/ |                         | |
//             |__/                          |_|

ObjectExpression::ObjectExpression(const ASTNode &node) {
	if (!node.empty())
		for (const auto &subnode: *node.front())
			map[*subnode->text] = Expression::create(*subnode->front());
}

Value * ObjectExpression::evaluate(Context &context) {
	auto *out = context.makeValue<Object>();
	FieldSaver saver(context, &Context::nextThis);
	context.nextThis = context.makeReference(out);
	for (const auto &[key, expression]: map)
		out->map[key] = context.makeReference(expression->evaluate(context));
	return out;
}

void ObjectExpression::findVariables(std::vector<VariableUsage> &usages) const {
	for (const auto &[key, expression]: map)
		expression->findVariables(usages);
}

//   _____
//  |  __ \.
//  | |__) | __ ___   __ _ _ __ __ _ _ __ ___
//  |  ___/ '__/ _ \ / _` | '__/ _` | '_ ` _ \.
//  | |   | | | (_) | (_| | | | (_| | | | | | |
//  |_|   |_|  \___/ \__, |_|  \__,_|_| |_| |_|
//                    __/ |
//                   |___/

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

//   _____      _
//  |  __ \    | |
//  | |__) |___| |_ _   _ _ __ _ __
//  |  _  // _ \ __| | | | '__| '_ \.
//  | | \ \  __/ |_| |_| | |  | | | |
//  |_|  \_\___|\__|\__,_|_|  |_| |_|

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

//    _____ _        _                            _
//   / ____| |      | |                          | |
//  | (___ | |_ __ _| |_ ___ _ __ ___   ___ _ __ | |_
//   \___ \| __/ _` | __/ _ \ '_ ` _ \ / _ \ '_ \| __|
//   ____) | || (_| | ||  __/ | | | | |  __/ | | | |_
//  |_____/ \__\__,_|\__\___|_| |_| |_|\___|_| |_|\__|

std::unique_ptr<Statement> Statement::create(const ASTNode &node) {
	std::unique_ptr<Statement> out;

	switch (node.symbol) {
		case JSTOK_LET:
		case JSTOK_CONST:
		case JSTOK_VAR:
			out = std::make_unique<VariableDefinitions>(node);
			break;

		case JSTOK_IF:
			out = std::make_unique<IfStatement>(node);
			break;

		case JSTOK_WHILE:
			out = std::make_unique<WhileLoop>(node);
			break;

		case JSTOK_FOR:
			out = std::make_unique<ForLoop>(node);
			break;

		case JSTOK_CONTINUE:
			out = std::make_unique<Continue>();
			break;

		case JSTOK_BREAK:
			out = std::make_unique<Break>();
			break;

		case JSTOK_RETURN:
			out = std::make_unique<Return>(node);
			break;

		case JS_BLOCK:
			out = std::make_unique<Block>(node);
			break;

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
		case JSTOK_ASSIGN:
		case JSTOK_COMMA:
		case JSTOK_PLUS:
		case JSTOK_MINUS:
		case JSTOK_PLUSPLUS:
		case JS_POSTPLUS:
		case JSTOK_MINUSMINUS:
		case JS_POSTMINUS:
		case JSTOK_NOT:
		case JSTOK_IDENT:
		case JSTOK_NUMBER:
		case JSTOK_NAN:
		case JSTOK_INFINITY:
		case JSTOK_STRING:
		case JSTOK_UNDEFINED:
		case JSTOK_NULL:
		case JSTOK_LPAREN:
		case JSTOK_TRUE:
		case JSTOK_FALSE:
		case JSTOK_FUNCTION:
		case JS_OBJECT:
		case JSTOK_PERIOD:
		case JS_ARRAY:
		case JSTOK_LSQUARE:
		case JSTOK_NEW:
		case JSTOK_DELETE:
		case JSTOK_PLUSEQ:
		case JSTOK_MINUSEQ:
		case JSTOK_DIVEQ:
		case JSTOK_TIMESEQ:
		case JSTOK_MODEQ:
		case JSTOK_SRAEQ:
		case JSTOK_SRLEQ:
		case JSTOK_SLEQ:
		case JSTOK_ANDEQ:
		case JSTOK_LANDEQ:
		case JSTOK_OREQ:
		case JSTOK_LOREQ:
		case JSTOK_XOREQ:
		case JSTOK_EXPEQ:
		case JSTOK_LAND:
		case JSTOK_LOR:
		case JSTOK_OR:
		case JSTOK_XOR:
		case JSTOK_AND:
		case JSTOK_IN:
		case JSTOK_INSTANCEOF:
			out = Expression::create(node);
			break;

		case JS_EMPTY:
			out = std::make_unique<EmptyStatement>(node);
			break;

		default:
			node.debug();
			throw std::invalid_argument("Unhandled symbol in Statement::create: " + std::string(node.getName()));
	}

	out->absorbPosition(node);
	return out;
}

//    _____ _        _             _      _ _                 _
//   / ____| |      (_)           | |    (_) |               | |
//  | (___ | |_ _ __ _ _ __   __ _| |     _| |_ ___ _ __ __ _| |
//   \___ \| __| '__| | '_ \ / _` | |    | | __/ _ \ '__/ _` | |
//   ____) | |_| |  | | | | | (_| | |____| | ||  __/ | | (_| | |
//  |_____/ \__|_|  |_|_| |_|\__, |______|_|\__\___|_|  \__,_|_|
//                            __/ |
//                           |___/

StringLiteral::StringLiteral(const ASTNode &node): value(node.unquote()) {
	absorbPosition(node);
}

Value * StringLiteral::evaluate(Context &context) {
	return context.makeValue<String>(value);
}

//   _    _                        ______                              _
//  | |  | |                      |  ____|                            (_)
//  | |  | |_ __   __ _ _ __ _   _| |__  __  ___ __  _ __ ___  ___ ___ _  ___  _ __
//  | |  | | '_ \ / _` | '__| | | |  __| \ \/ / '_ \| '__/ _ \/ __/ __| |/ _ \| '_ \.
//  | |__| | | | | (_| | |  | |_| | |____ >  <| |_) | | |  __/\__ \__ \ | (_) | | | |
//   \____/|_| |_|\__,_|_|   \__, |______/_/\_\ .__/|_|  \___||___/___/_|\___/|_| |_|
//                            __/ |           | |
//                           |___/            |_|

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
			assert(subvalue != nullptr);
			assert(subvalue->getType() == ValueType::Reference);
			if (subvalue->ultimateType() != ValueType::Number)
				return context.makeValue<Number>(nan(""));
			auto *number = subvalue->ultimateValue()->cast<Number>();
			double new_value = nan("");
			switch (type) {
				case Type::PrefixIncrement:  new_value = ++number->number; break;
				case Type::PrefixDecrement:  new_value = --number->number; break;
				case Type::PostfixIncrement: new_value = number->number++; break;
				case Type::PostfixDecrement: new_value = number->number--; break;
				default: std::terminate();
			}
			return context.makeValue<Number>(new_value);
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

//   _    _           _       __ _                _ _      _ _                 _
//  | |  | |         | |     / _(_)              | | |    (_) |               | |
//  | |  | |_ __   __| | ___| |_ _ _ __   ___  __| | |     _| |_ ___ _ __ __ _| |
//  | |  | | '_ \ / _` |/ _ \  _| | '_ \ / _ \/ _` | |    | | __/ _ \ '__/ _` | |
//  | |__| | | | | (_| |  __/ | | | | | |  __/ (_| | |____| | ||  __/ | | (_| | |
//   \____/|_| |_|\__,_|\___|_| |_|_| |_|\___|\__,_|______|_|\__\___|_|  \__,_|_|

UndefinedLiteral::UndefinedLiteral(const ASTNode &node) {
	absorbPosition(node);
}

Value * UndefinedLiteral::evaluate(Context &context) {
	return context.makeValue<Undefined>();
}

//  __      __        _       _     _      _____        __ _       _ _   _
//  \ \    / /       (_)     | |   | |    |  __ \      / _(_)     (_) | (_)
//   \ \  / /_ _ _ __ _  __ _| |__ | | ___| |  | | ___| |_ _ _ __  _| |_ _  ___  _ __
//    \ \/ / _` | '__| |/ _` | '_ \| |/ _ \ |  | |/ _ \  _| | '_ \| | __| |/ _ \| '_ \.
//     \  / (_| | |  | | (_| | |_) | |  __/ |__| |  __/ | | | | | | | |_| | (_) | | | |
//      \/ \__,_|_|  |_|\__,_|_.__/|_|\___|_____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|

VariableDefinition::VariableDefinition(const ASTNode &node):
	ident(*node.text),
	value(node.empty()? nullptr : Expression::create(*node.front())) { absorbPosition(node); }

std::pair<Result, Value *> VariableDefinition::interpret(Context &) {
	throw Unimplemented();
}

void VariableDefinition::findVariables(std::vector<VariableUsage> &usages) const {
	usages.emplace_back(true, ident);
}

//  __      __        _       _     _      _____        __ _       _ _   _
//  \ \    / /       (_)     | |   | |    |  __ \      / _(_)     (_) | (_)
//   \ \  / /_ _ _ __ _  __ _| |__ | | ___| |  | | ___| |_ _ _ __  _| |_ _  ___  _ __  ___
//    \ \/ / _` | '__| |/ _` | '_ \| |/ _ \ |  | |/ _ \  _| | '_ \| | __| |/ _ \| '_ \/ __|
//     \  / (_| | |  | | (_| | |_) | |  __/ |__| |  __/ | | | | | | | |_| | (_) | | | \__ \.
//      \/ \__,_|_|  |_|\__,_|_.__/|_|\___|_____/ \___|_| |_|_| |_|_|\__|_|\___/|_| |_|___/

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
			Value *evaluated =definition->value->evaluate(context);
			assert(evaluated != nullptr);
			if (auto *reference = evaluated->cast<Reference>())
				context.stack.insert(name, evaluated->shallowCopy());
			else
				context.stack.insert(name, evaluated);
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

void VariableDefinitions::validateInSingleStatementContext() const {
	if (kind == DeclarationKind::Let || kind == DeclarationKind::Const)
		throw SyntaxError("Lexical declaration cannot appear in a single-statement context");
}

//  __          ___     _ _      _
//  \ \        / / |   (_) |    | |
//   \ \  /\  / /| |__  _| | ___| |     ___   ___  _ __
//    \ \/  \/ / | '_ \| | |/ _ \ |    / _ \ / _ \| '_ \.
//     \  /\  /  | | | | | |  __/ |___| (_) | (_) | |_) |
//      \/  \/   |_| |_|_|_|\___|______\___/ \___/| .__/
//                                                | |
//                                                |_|

WhileLoop::WhileLoop(const ASTNode &node):
	condition(Expression::create(*node.at(0))),
	body(Statement::create(*node.at(1))) { absorbPosition(node); }

std::pair<Result, Value *> WhileLoop::interpret(Context &context) {
	body->validateInSingleStatementContext();

	// Pushing/popping the scope stack isn't necessary because there's no need for it in a non-block statement
	// (let/const is invalid in a single-statement context) and a block does it itself.

	while (*condition->evaluate(context)) {
		const auto [result, value] = body->interpret(context);
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
