#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "ASTNode.h"
#include "Errors.h"
#include "Generator.h"

class Context;
class Statement;
class Value;

enum class Result {None, Break, Continue, Return};
enum class DeclarationKind {Invalid = 0, Const, Let, Var};

DeclarationKind getKind(int symbol);

enum class NodeType {
	Invalid = 0, Program, Block, IfStatement, BinaryExpression, UnaryExpression, VariableDefinition,
	VariableDefinitions, FunctionCall, WhileLoop, Continue, Break, FunctionExpression, Return,
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

		/** Helps find all variables inside a function body for the purpose of assembling a closure set. */
		virtual Generator<std::string> findVariables() const {
			throw Unimplemented("Can't find variables in node of type " + std::string(typeid(*this).name()));
		}

		virtual Generator<std::string> findKilledVariables() const { co_return; }

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
	public:
		static std::unique_ptr<Expression> create(const ASTNode &);

		std::pair<Result, Value *> interpret(Context &) override;

	protected:
		Expression() = default;
};

class LValueExpression: public Expression {
	public:
		Value **referenced = nullptr;

	protected:
		LValueExpression() = default;
};

class Identifier: public LValueExpression {
	public:
		std::string name;
		Identifier(const ASTNode &);
		Value * evaluate(Context &) override;
		Generator<std::string> findVariables() const override;
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
		Generator<std::string> findVariables() const override;

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
		Generator<std::string> findVariables() const override;

		static Type getType(int symbol);
};

class NewExpression: public Expression {
	public:
		std::unique_ptr<Expression> classExpression;
		std::vector<std::unique_ptr<Expression>> arguments;
		// Generator<std::string> findVariables() const override;
};

class Block: public Statement {
	public:
		std::vector<std::unique_ptr<Statement>> body;

		Block(const ASTNode &);

		NodeType getType() const override { return NodeType::Block; }
		std::pair<Result, Value *> interpret(Context &) override;
		Generator<std::string> findVariables() const override;
		Generator<std::string> findKilledVariables() const override;
};

class FunctionExpression: public Expression {
	public:
		std::string name;
		std::vector<std::unique_ptr<Identifier>> arguments;
		std::unique_ptr<Block> body;

		FunctionExpression(const ASTNode &);

		NodeType getType() const override { return NodeType::FunctionExpression; }
		Value * evaluate(Context &) override;
		std::pair<Result, Value *> interpret(Context &) override;
		Generator<std::string> findVariables() const override;
};

class VariableDefinition: public Node {
	public:
		std::string ident;
		std::unique_ptr<Expression> value;

		VariableDefinition(const ASTNode &);
		NodeType getType() const override { return NodeType::VariableDefinition; }
		std::pair<Result, Value *> interpret(Context &) override;
		/** A variable definition kills the parent-scope variable for the rest of the scope. */
		Generator<std::string> findVariables() const override { co_return; }
		Generator<std::string> findKilledVariables() const override;
};

class VariableDefinitions: public Statement {
	public:
		DeclarationKind kind = DeclarationKind::Invalid;
		std::vector<std::unique_ptr<VariableDefinition>> definitions;

		VariableDefinitions(const ASTNode &);

		NodeType getType() const override { return NodeType::VariableDefinitions; }
		std::pair<Result, Value *> interpret(Context &) override;
		Generator<std::string> findVariables() const override { co_return; }
		Generator<std::string> findKilledVariables() const override;
};

class IfStatement: public Statement {
	public:
		std::unique_ptr<Expression> condition;
		std::unique_ptr<Statement> consequent;
		std::unique_ptr<Statement> alternate;

		IfStatement(const ASTNode &);

		NodeType getType() const override { return NodeType::IfStatement; }
		std::pair<Result, Value *> interpret(Context &) override;
		Generator<std::string> findVariables() const override;
};

class WhileLoop: public Statement {
	public:
		std::unique_ptr<Expression> condition;
		std::unique_ptr<Statement> body;

		WhileLoop(const ASTNode &);

		NodeType getType() const override { return NodeType::WhileLoop; }
		std::pair<Result, Value *> interpret(Context &) override;
		Generator<std::string> findVariables() const override;
};

class Continue: public Statement {
	public:
		Continue() = default;
		NodeType getType() const override { return NodeType::Continue; }
		std::pair<Result, Value *> interpret(Context &) override;
		Generator<std::string> findVariables() const override { co_return; }
};

class Break: public Statement {
	public:
		Break() = default;
		NodeType getType() const override { return NodeType::Break; }
		std::pair<Result, Value *> interpret(Context &) override;
		Generator<std::string> findVariables() const override { co_return; }
};

class Return: public Statement {
	public:
		std::unique_ptr<Expression> returnValue;
		Return(const ASTNode &);
		NodeType getType() const override { return NodeType::Return; }
		std::pair<Result, Value *> interpret(Context &) override;
		Generator<std::string> findVariables() const override;
};

struct NumberLiteral: public Expression {
	public:
		double value;
		NumberLiteral(const ASTNode &node);
		Value * evaluate(Context &) override;
		Generator<std::string> findVariables() const override { co_return; }
};

struct StringLiteral: public Expression {
	public:
		std::string value;
		StringLiteral(const ASTNode &node);
		Value * evaluate(Context &) override;
		Generator<std::string> findVariables() const override { co_return; }
};

struct BooleanLiteral: public Expression {
	public:
		bool value;
		BooleanLiteral(const ASTNode &node);
		Value * evaluate(Context &) override;
		Generator<std::string> findVariables() const override { co_return; }
};

struct FunctionCall: public Expression {
	public:
		std::unique_ptr<Expression> function;
		std::vector<std::unique_ptr<Expression>> arguments;

		FunctionCall(const ASTNode &);

		NodeType getType() const override { return NodeType::FunctionCall; }
		Value * evaluate(Context &) override;
		Generator<std::string> findVariables() const override;
};
