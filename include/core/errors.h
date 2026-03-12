#pragma once

#include <stdexcept>
#include <string>

// MathPlotError -> ParseError -> UnexpectedTokenError, UnmatchedParenError, UnknownFunctionError
// MathPlotError -> MathError  -> DivisionByZeroError, DomainError
// MathPlotError -> FileIOError

// базовый класс для всех ошибок
class MathPlotError : public std::runtime_error {
public:
    explicit MathPlotError(const std::string& msg)
        : std::runtime_error(msg) {}
};

// ошибки парсера
class ParseError : public MathPlotError {
public:
    explicit ParseError(const std::string& msg)
        : MathPlotError(msg) {}
};

// math ошибки
class MathError : public MathPlotError {
public:
    explicit MathError(const std::string& msg)
        : MathPlotError(msg) {}
};

// ошибка файлов
class FileIOError : public MathPlotError {
public:
    explicit FileIOError(const std::string& msg)
        : MathPlotError("File I/O error: " + msg) {}
};


class UnexpectedTokenError : public ParseError {
public:
    UnexpectedTokenError(const std::string& found, std::size_t pos)
        : ParseError("Unexpected token '" + found + "' at position " + std::to_string(pos)) {}
};

class UnmatchedParenError : public ParseError {
public:
    explicit UnmatchedParenError(std::size_t pos)
        : ParseError("Unmatched parenthesis at position " + std::to_string(pos)) {}
};

class UnknownFunctionError : public ParseError {
public:
    explicit UnknownFunctionError(const std::string& name)
        : ParseError("Unknown function: " + name) {}
};

class DivisionByZeroError : public MathError {
public:
    DivisionByZeroError()
        : MathError("Division by zero") {}
};

class DomainError : public MathError {
public:
    explicit DomainError(const std::string& msg)
        : MathError("Domain error: " + msg) {}
};
