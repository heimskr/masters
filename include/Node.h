#pragma once

#include <memory>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class Context;
class Value;

enum class Result {None, Break, Continue, Return};
enum class DeclarationKind {Invalid = 0, Const, Let, Var};

class Node {
	public:
		enum class Type {
			Invalid = 0, Program, BlockStatement, Identifier, VariableDeclarator, VariableDeclaration,
			ExpressionStatement, BinaryExpression,
		};

		int start = -1;
		int end = -1;

		virtual ~Node() = default;
		virtual Type getType() const { return Type::Invalid; }

		virtual Value * evaluate(Context &) {
			throw std::runtime_error("Cannot evaluate node of type " + std::string(typeid(*this).name()));
		}

		virtual std::pair<Result, Value *> interpret(Context &) {
			throw std::runtime_error("Cannot interpret node of type " + std::string(typeid(*this).name()));
		}

		void assertType(Type);

		static std::unique_ptr<Node> fromJSON(const nlohmann::json &);

	protected:
		void absorbPosition(const nlohmann::json &);
};

const char * stringify(Node::Type);

template <typename T>
class Makeable {
	template <typename... Args>
	static std::unique_ptr<T> make(Args &&...args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
};

class ProgramNode: public Node {
	public:
		std::vector<std::unique_ptr<Node>> body;

		ProgramNode(const nlohmann::json &);
		~ProgramNode() override = default;

		Type getType() const override { return Type::Program; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class BlockStatementNode: public Node {
	public:
		std::vector<std::unique_ptr<Node>> body;

		BlockStatementNode(const nlohmann::json &);
		~BlockStatementNode() override = default;

		Type getType() const override { return Type::BlockStatement; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class IdentifierNode: public Node {
	public:
		std::string name;

		IdentifierNode(const nlohmann::json &);
		~IdentifierNode() override = default;

		Type getType() const override { return Type::Identifier; }
};

class VariableDeclaratorNode: public Node {
	public:
		std::unique_ptr<Node> id;
		std::unique_ptr<Node> init;

		VariableDeclaratorNode(const nlohmann::json &);
		~VariableDeclaratorNode() = default;

		Type getType() const override { return Type::VariableDeclarator; }
};

class VariableDeclarationNode: public Node {
	public:
		DeclarationKind kind;
		std::vector<std::unique_ptr<VariableDeclaratorNode>> declarators;

		VariableDeclarationNode(const nlohmann::json &);
		~VariableDeclarationNode() = default;

		Type getType() const override { return Type::VariableDeclaration; }
		std::pair<Result, Value *> interpret(Context &) override;

	private:
		static DeclarationKind getKind(const std::string &);
};

class ExpressionStatementNode: public Node {
	public:
		std::unique_ptr<Node> expression;

		ExpressionStatementNode(const nlohmann::json &);
		~ExpressionStatementNode() = default;

		Type getType() const override { return Type::ExpressionStatement; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class IfStatementNode: public Node {
	public:
		std::unique_ptr<Node> test;
		std::unique_ptr<Node> consequent;
		std::unique_ptr<Node> alternate;

		IfStatementNode(const nlohmann::json &);
		~IfStatementNode() = default;

		Type getType() const override { return Type::ExpressionStatement; }
		std::pair<Result, Value *> interpret(Context &) override;
};

class BinaryExpressionNode: public Node {
	public:
		std::unique_ptr<Node> left;
		std::unique_ptr<Node> right;
		std::string op;

		BinaryExpressionNode(const nlohmann::json &);
		~BinaryExpressionNode() = default;

		Type getType() const override { return Type::BinaryExpression; }
		Value * evaluate(Context &) override;
};
