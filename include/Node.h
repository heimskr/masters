#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "ASTNode.h"

class Context;
class Statement;
class Value;

enum class Result {None, Break, Continue, Return};
enum class DeclarationKind {Invalid = 0, Const, Let, Var};

DeclarationKind getKind(int symbol);

enum class NodeType {
	Invalid = 0, Program, Block, IfStatement, BinaryExpression, UnaryExpression, VariableDefinition,
	VariableDefinitions, FunctionCall,
};

class Node {
	public:
		ASTLocation location;

		virtual ~Node() = default;
		virtual NodeType getType() const { return NodeType::Invalid; }

		virtual Value * evaluate(Context &) {
			throw std::runtime_error("Cannot evaluate node of type " + std::string(typeid(*this).name()));
		}

		virtual std::pair<Result, Value *> interpret(Context &) {
			throw std::runtime_error("Cannot interpret node of type " + std::string(typeid(*this).name()));
		}

		void assertType(NodeType);

		static std::unique_ptr<Node> fromAST(const ASTNode &);

	protected:
		void absorbPosition(const ASTNode &);
};

const char * stringify(NodeType);

template <typename T>
class Makeable {
	template <typename... Args>
	static std::unique_ptr<T> make(Args &&...args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
};

class Program: public Node {
	public:
		std::vector<std::unique_ptr<Statement>> body;

		Program(const ASTNode &);

		NodeType getType() const override { return NodeType::Program; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class Statement: public Node {
	protected:
		Statement() = default;

	public:
		static std::unique_ptr<Statement> create(const ASTNode &);
};

class Expression: public Statement {
	protected:
		Expression() = default;

	public:
		static std::unique_ptr<Expression> create(const ASTNode &);

		std::pair<Result, Value *> interpret(Context &) override;
};

class BinaryExpression: public Expression {
	public:
		enum class Type {
			Invalid = 0, LogicalAnd, LogicalOr, BitwiseAnd, BitwiseOr, BitwiseXor, TripleEquals, TripleNotEquals,
			LeftShift, RightShiftArithmetic, RightShiftLogical, LessThan, GreaterThan, LessThanOrEqual,
			GreaterThanOrEqual, Addition, Subtraction, Multiplication, Exponentiation, Division, Assignment, Modulo,
			AdditionAssignment, SubtractionAssignment, MultiplicationAssignment, ExponentiationAssignment,
			DivisionAssignment, ModuloAssignment, LeftShiftAssignment, RightShiftArithmeticAssignment,
			RightShiftLogicalAssignment, BitwiseAndAssignment, BitwiseOrAssignment, LogicalOrAssignment,
			LogicalAndAssignment, BitwiseXorAssignment, In, Instanceof,
		};

		Type type = Type::Invalid;
		std::unique_ptr<Expression> left;
		std::unique_ptr<Expression> right;

		BinaryExpression(const ASTNode &);

		NodeType getType() const override { return NodeType::BinaryExpression; }
		Value * evaluate(Context &) override;

		static Type getType(int symbol);
		static std::unordered_set<Type> assignmentTypes;

	private:
		Value ** access(Context &, bool *is_const = nullptr);
		Value * evaluateAccess(Context &);
};

class UnaryExpression: public Expression {
	public:
		enum class Type {
			Invalid = 0, LogicalNot, BitwiseNot, Plus, Negation, PrefixIncrement, PrefixDecrement, PostfixIncrement,
			PostfixDecrement, Delete, Void, Typeof,
		};

		Type type = Type::Invalid;
		std::unique_ptr<Expression> subexpr;

		UnaryExpression(const ASTNode &);

		NodeType getType() const override { return NodeType::UnaryExpression; }
		Value * evaluate(Context &) override;

		static Type getType(int symbol);
};

class NewExpression: public Expression {
	public:
		std::unique_ptr<Expression> classExpression;
		std::vector<std::unique_ptr<Expression>> arguments;
};

class Block: public Statement {
	public:
		std::vector<std::unique_ptr<Statement>> body;

		Block(const ASTNode &);

		NodeType getType() const override { return NodeType::Block; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class VariableDefinition: public Node {
	public:
		std::string ident;
		std::unique_ptr<Expression> value;

		VariableDefinition(const ASTNode &);
		NodeType getType() const override { return NodeType::VariableDefinition; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class VariableDefinitions: public Statement {
	public:
		DeclarationKind kind = DeclarationKind::Invalid;
		std::vector<std::unique_ptr<VariableDefinition>> definitions;

		VariableDefinitions(const ASTNode &);

		NodeType getType() const override { return NodeType::VariableDefinitions; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class IfStatement: public Statement {
	public:
		std::unique_ptr<Expression> condition;
		std::unique_ptr<Statement> consequent;
		std::unique_ptr<Statement> alternate;

		IfStatement(const ASTNode &);

		NodeType getType() const override { return NodeType::IfStatement; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class Identifier: public Expression {
	public:
		std::string name;

		Identifier(const ASTNode &);

		Value * evaluate(Context &) override;
};

struct NumberLiteral: public Expression {
	public:
		double value;
		NumberLiteral(const ASTNode &node);
		Value * evaluate(Context &) override;
};

struct StringLiteral: public Expression {
	public:
		std::string value;
		StringLiteral(const ASTNode &node);
		Value * evaluate(Context &) override;
};

struct BooleanLiteral: public Expression {
	public:
		bool value;
		BooleanLiteral(const ASTNode &node);
		Value * evaluate(Context &) override;
};

struct FunctionCall: public Expression {
	public:
		std::unique_ptr<Expression> function;
		std::vector<std::unique_ptr<Expression>> arguments;

		FunctionCall(const ASTNode &);

		NodeType getType() const override { return NodeType::FunctionCall; }
		Value * evaluate(Context &) override;
};
