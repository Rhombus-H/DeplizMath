#include "parser/parser.h"
#include "core/constants.h"
#include "core/errors.h"

Parser::Parser(const std::string& input)
    : tok(input), current_token(tok.next()) {}

void Parser::advance() {
    current_token = tok.next();
}

void Parser::expect(TokenType type) {
    if (current_token.type != type) {
        if (type == TokenType::RParen) {
            throw UnmatchedParenError(current_token.position);
        }
        throw UnexpectedTokenError(current_token.value, current_token.position);
    }
    advance();
}

ParseResult Parser::parse() {
    // парсим левую часть
    ExprPtr leftSide = parseExpr();

    // обязательно должен быть оператор: = > < >= <=
    if (current_token.type == TokenType::EndOfInput) {
        throw ParseError("Expected '=' or comparison operator. Use format: y = f(x) or lhs = rhs");
    }

    // === ЗНАК РАВНО -> функция или уравнение ===
    if (current_token.type == TokenType::Equal) {
        advance();
        ExprPtr rightSide = parseExpr();

        if (current_token.type != TokenType::EndOfInput) {
            throw UnexpectedTokenError(current_token.value, current_token.position);
        }

        // если lhs это просто "y" -> пробуем как функцию
        // (pipeline потом доп. проверит содержит ли rhs y)
        VariableLit* varCheck = std::get_if<VariableLit>(&leftSide->data);
        if (varCheck != nullptr && varCheck->name == "y") {
            // пока считаем что это функция. если rhs содержит y,
            // pipeline переклассифицирует в уравнение
            ParsedFunction func;
            func.expr = std::move(rightSide);
            return func;
        }

        // иначе уравнение (x^2+y^2=1, x=y^2, и тд)
        ParsedEquation eq;
        eq.lhs = std::move(leftSide);
        eq.rhs = std::move(rightSide);
        return eq;
    }

    // === НЕРАВЕНСТВО ===
    bool isInequality = false;
    CompareOp compOp = CompareOp::Less;

    if (current_token.type == TokenType::Greater) {
        isInequality = true;
        compOp = CompareOp::Greater;
    } else if (current_token.type == TokenType::Less) {
        isInequality = true;
        compOp = CompareOp::Less;
    } else if (current_token.type == TokenType::GreaterEqual) {
        isInequality = true;
        compOp = CompareOp::GreaterEqual;
    } else if (current_token.type == TokenType::LessEqual) {
        isInequality = true;
        compOp = CompareOp::LessEqual;
    }

    if (isInequality) {
        advance();
        ExprPtr rightSide = parseExpr();

        if (current_token.type != TokenType::EndOfInput) {
            throw UnexpectedTokenError(current_token.value, current_token.position);
        }

        // определяем тип
        bool isTwoVar = true;
        VariableLit* yCheck = std::get_if<VariableLit>(&leftSide->data);
        if (yCheck != nullptr && yCheck->name == "y") {
            isTwoVar = false;
        }

        ParsedInequality ineq;
        ineq.op = compOp;
        ineq.lhs = std::move(leftSide);
        ineq.rhs = std::move(rightSide);
        ineq.twoVariable = isTwoVar;
        return ineq;
    }

    throw UnexpectedTokenError(current_token.value, current_token.position);
}

ExprPtr Parser::parseExpression() {
    ExprPtr result = parseExpr();
    if (current_token.type != TokenType::EndOfInput) {
        throw UnexpectedTokenError(current_token.value, current_token.position);
    }
    return result;
}

ExprPtr Parser::parseExpr() {
    ExprPtr left = parseTerm();

    while (true) {
        if (current_token.type == TokenType::Plus) {
            advance();
            ExprPtr right = parseTerm();
            left = makeBinaryOp('+', std::move(left), std::move(right));
        } else if (current_token.type == TokenType::Minus) {
            advance();
            ExprPtr right = parseTerm();
            left = makeBinaryOp('-', std::move(left), std::move(right));
        } else {
            break;
        }
    }

    return left;
}

ExprPtr Parser::parseTerm() {
    ExprPtr left = parseUnary();

    while (true) {
        if (current_token.type == TokenType::Star) {
            advance();
            ExprPtr right = parseUnary();
            left = makeBinaryOp('*', std::move(left), std::move(right));
        } else if (current_token.type == TokenType::Slash) {
            advance();
            ExprPtr right = parseUnary();
            left = makeBinaryOp('/', std::move(left), std::move(right));
        } else {
            break;
        }
    }

    return left;
}

ExprPtr Parser::parseUnary() {
    if (current_token.type == TokenType::Minus) {
        advance();
        ExprPtr inner = parseUnary();
        return makeUnaryOp('-', std::move(inner));
    }
    return parsePower();
}

ExprPtr Parser::parsePower() {
    ExprPtr base = parseAtom();

    if (current_token.type == TokenType::Caret) {
        advance();
        ExprPtr exp = parseUnary();
        return makeBinaryOp('^', std::move(base), std::move(exp));
    }

    return base;
}

ExprPtr Parser::parseAtom() {
    if (current_token.type == TokenType::Number) {
        double val = std::stod(current_token.value);
        advance();
        return makeNumber(val);
    }

    if (current_token.type == TokenType::Identifier) {
        std::string name = current_token.value;
        advance();

        if (current_token.type == TokenType::LParen) {
            return parseFunctionCall(name);
        }

        std::optional<double> constVal = findConstant(name);
        if (constVal.has_value()) {
            double v = constVal.value();
            return makeNumber(v);
        }

        return makeVariable(name);
    }

    if (current_token.type == TokenType::LParen) {
        advance();
        ExprPtr inner = parseExpr();
        expect(TokenType::RParen);
        return inner;
    }

    throw UnexpectedTokenError(current_token.value, current_token.position);
}

ExprPtr Parser::parseFunctionCall(const std::string& funcName) {
    std::optional<FuncEntry> funcInfo = findFunction(funcName);
    if (!funcInfo.has_value()) {
        throw UnknownFunctionError(funcName);
    }

    expect(TokenType::LParen);

    std::vector<ExprPtr> arguments;
    if (current_token.type != TokenType::RParen) {
        ExprPtr arg1 = parseExpr();
        arguments.push_back(std::move(arg1));

        while (current_token.type == TokenType::Comma) {
            advance();
            ExprPtr nextArg = parseExpr();
            arguments.push_back(std::move(nextArg));
        }
    }

    expect(TokenType::RParen);

    int expectedArity = funcInfo.value().arity;
    int actualArity = (int)arguments.size();
    if (actualArity != expectedArity) {
        std::string msg = "Function '" + funcName + "' expects " +
                          std::to_string(expectedArity) + " argument(s), got " +
                          std::to_string(actualArity);
        throw ParseError(msg);
    }

    return makeFuncCall(funcName, std::move(arguments));
}
