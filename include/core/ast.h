#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>
#include "core/types.h"

// forward declaration
struct ExprNode;
using ExprPtr = std::unique_ptr<ExprNode>;

// --- AST node types (variant) ---

struct NumberLit {
    double value;
};

struct VariableLit {
    std::string name; // x or y
};

struct BinaryOp {
    char op; // + - * / ^
    ExprPtr left;
    ExprPtr right;
};

struct UnaryOp {
    char op; // just minus basically
    ExprPtr operand;
};

struct FuncCall {
    std::string name;
    std::vector<ExprPtr> args;
};

using ExprVariant = std::variant<NumberLit, VariableLit, BinaryOp, UnaryOp, FuncCall>;

// основной узел AST
struct ExprNode {
    ExprVariant data;

    explicit ExprNode(ExprVariant d) : data(std::move(d)) {}

    // Rule of Five
    ExprNode(const ExprNode&) = delete;
    ExprNode& operator=(const ExprNode&) = delete;
    ExprNode(ExprNode&&) noexcept = default;
    ExprNode& operator=(ExprNode&&) noexcept = default;
    ~ExprNode() = default;
};

// хелпер функции для создания узлов

inline ExprPtr makeNumber(double v) {
    return std::make_unique<ExprNode>(NumberLit{v});
}

inline ExprPtr makeVariable(std::string name) {
    return std::make_unique<ExprNode>(VariableLit{std::move(name)});
}

inline ExprPtr makeBinaryOp(char op, ExprPtr left, ExprPtr right) {
    return std::make_unique<ExprNode>(BinaryOp{op, std::move(left), std::move(right)});
}

inline ExprPtr makeUnaryOp(char op, ExprPtr operand) {
    return std::make_unique<ExprNode>(UnaryOp{op, std::move(operand)});
}

inline ExprPtr makeFuncCall(std::string name, std::vector<ExprPtr> args) {
    return std::make_unique<ExprNode>(FuncCall{std::move(name), std::move(args)});
}

// результат парсинга - функция, уравнение или неравенство
struct ParsedFunction {
    ExprPtr expr; // правая часть (y = expr)
};

struct ParsedEquation {
    ExprPtr lhs; // левая часть
    ExprPtr rhs; // правая часть
    // рисуется как неявная кривая (x^2+y^2=1, x=y^2, и тд)
};

struct ParsedInequality {
    CompareOp op;
    ExprPtr lhs;
    ExprPtr rhs;
    bool twoVariable; // если есть и x и y (например x^2+y^2<9)
};

using ParseResult = std::variant<ParsedFunction, ParsedInequality, ParsedEquation>;
