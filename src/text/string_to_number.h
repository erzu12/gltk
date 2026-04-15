#pragma once

#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <memory>
#include <ostream>
#include <span>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <vector>
namespace gltk {

struct Token {
    std::string value;
    enum class Type { Number, Operator, Parentesis } type;
};

template <typename T>
    requires std::is_arithmetic_v<T>
struct IExpression {
    virtual ~IExpression() = default;
    virtual T evaluate() const = 0;
};

template <typename T>
    requires std::is_arithmetic_v<T>
struct NumberExpression : public IExpression<T> {
    T value;
    explicit NumberExpression(T value) : value(value) {}
    T evaluate() const override { return value; }
};

template <typename T>
    requires std::is_arithmetic_v<T>
struct UnaryExpression : public IExpression<T> {
    char op;
    std::unique_ptr<IExpression<T>> operand;
    UnaryExpression(char op, std::unique_ptr<IExpression<T>> operand) : op(op), operand(std::move(operand)) {}
    T evaluate() const override {
        T val = operand->evaluate();
        switch (op) {
        case '+':
            return val;
        case '-':
            return -val;
        default:
            throw std::runtime_error("Invalid unary operator");
        }
    }
};

template <typename T>
    requires std::is_arithmetic_v<T>
struct BinaryExpression : public IExpression<T> {
    char op;
    std::unique_ptr<IExpression<T>> left;
    std::unique_ptr<IExpression<T>> right;
    BinaryExpression(char op, std::unique_ptr<IExpression<T>> left, std::unique_ptr<IExpression<T>> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}
    T evaluate() const override {
        T leftVal = left->evaluate();
        T rightVal = right->evaluate();
        switch (op) {
        case '+':
            return leftVal + rightVal;
        case '-':
            return leftVal - rightVal;
        case '*':
            return leftVal * rightVal;
        case '/':
            if (rightVal == 0) {
                throw std::runtime_error("Division by zero");
            }
            return leftVal / rightVal;
        case '^':
            return std::pow(leftVal, rightVal);
        case '%':
            if (rightVal == 0) {
                throw std::runtime_error("Modulo by zero");
            }
            return std::fmod(leftVal, rightVal);
        default:
            throw std::runtime_error("Invalid binary operator");
        }
    }
};

template <typename T>
    requires std::is_arithmetic_v<T>
class Parser;

class stringTools {
  public:
    template <typename T>
        requires std::is_arithmetic_v<T>
    static T stringToNumber(const std::string &str) {
        if (str.empty()) {
            throw std::invalid_argument("Empty string");
        }
        std::string lowerStr;
        std::transform(str.begin(), str.end(), std::back_inserter(lowerStr), ::tolower);
        lowerStr.erase(std::remove_if(lowerStr.begin(), lowerStr.end(), ::isspace), lowerStr.end());
        if (lowerStr == "inf") {
            return std::numeric_limits<T>::infinity();
        } else if (lowerStr == "-inf") {
            return -std::numeric_limits<T>::infinity();
        } else if (lowerStr == "e") {
            return static_cast<T>(std::exp(1.0));
        } else if (lowerStr == "pi") {
            return static_cast<T>(std::acos(-1.0));
        }
        std::unordered_set<char> validChars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'e', '.', '+', '-'};
        std::string filteredStr;
        std::copy_if(str.begin(), str.end(), std::back_inserter(filteredStr), [&validChars](char c) {
            return validChars.count(c) > 0;
        });
        try {
            if constexpr (std::is_integral_v<T>) {
                if (filteredStr.find('.') != std::string::npos || filteredStr.find('e') != std::string::npos) {
                    throw std::invalid_argument("Invalid integer format: " + str);
                }
                return static_cast<T>(std::stoll(filteredStr));
            } else {
                std::cout << "Filtered string: " << filteredStr << std::endl;
                return static_cast<T>(std::stod(filteredStr));
            }
        } catch (const std::exception &e) {
            throw std::invalid_argument("Invalid number format: " + str);
        }
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    static T numericExpressionToNumber(const std::string &str) {
        Parser<T> parser;
        auto expression = parser.parse(str);
        return expression->evaluate();
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    static std::string numberToString(T number, int decimalPlaces = 3, int maxLength = 8) {
        if (std::isinf(number)) {
            return (number > 0) ? "inf" : "-inf";
        } else if (std::isnan(number)) {
            return "nan";
        } else {
            std::string str = std::format("{:.{}f}", number, decimalPlaces);
            // Remove trailing zeros and decimal point if not needed
            str.erase(str.find_last_not_of('0') + 1, std::string::npos);
            if (str.back() == '.') {
                str.pop_back();
            }
            int digitsBeforeDecimal = str.find('.') != std::string::npos ? str.find('.') : str.length();
            if (digitsBeforeDecimal > maxLength) {
                str = std::format("{:.{}g}", number, maxLength - 4);
            } else if (str.length() > maxLength) {
                str = std::format("{:.{}g}", number, maxLength);
            }
            return str;
        }
    }

  private:
    stringTools() = delete;
};

template <typename T>
    requires std::is_arithmetic_v<T>
class Parser {
    std::vector<Token> tokens;
    std::span<Token> currentTokens;

  public:
    Parser() = default;

    std::unique_ptr<IExpression<T>> parse(const std::string &str) {
        tokens = tokenize(str);
        currentTokens = tokens;
        return parseExpression();
    }

  private:
    std::vector<Token> tokenize(const std::string &str) {
        std::vector<Token> tokens;
        std::string currentNumber;
        for (char c : str) {
            if (std::isdigit(c) || c == '.' || c == 'e' || c == 'p' || c == 'i' || c == 'n' || c == 'f' ||
                c == '+' && !currentNumber.empty() && currentNumber.back() == 'e' ||
                c == '-' && !currentNumber.empty() && currentNumber.back() == 'e') {
                currentNumber += c;
            } else if (c == ' ' || c == '\t') {
                if (!currentNumber.empty()) {
                    tokens.push_back({currentNumber, Token::Type::Number});
                    currentNumber.clear();
                }
            } else if (c == '(' || c == ')') {
                if (!currentNumber.empty()) {
                    tokens.push_back({currentNumber, Token::Type::Number});
                    currentNumber.clear();
                }
                tokens.push_back({std::string(1, c), Token::Type::Parentesis});
            } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%') {
                if (!currentNumber.empty()) {
                    tokens.push_back({currentNumber, Token::Type::Number});
                    currentNumber.clear();
                }
                tokens.push_back({std::string(1, c), Token::Type::Operator});
            } else {
                throw std::invalid_argument(std::string("Invalid character in expression: ") + c);
            }
        }
        if (!currentNumber.empty()) {
            tokens.push_back({currentNumber, Token::Type::Number});
        }
        return tokens;
    }
    void consumeToken() {
        if (currentTokens.empty()) {
            throw std::runtime_error("Unexpected end of expression");
        }
        currentTokens = currentTokens.subspan(1);
    }

    std::unique_ptr<IExpression<T>> parseExpression() {
        auto node = parseTerm();
        while (!currentTokens.empty() && (currentTokens.front().value == "+" || currentTokens.front().value == "-")) {
            char op = currentTokens.front().value[0];
            consumeToken();
            auto right = parseTerm();
            node = std::make_unique<BinaryExpression<T>>(op, std::move(node), std::move(right));
        }
        return node;
    }

    std::unique_ptr<IExpression<T>> parseTerm() {
        auto node = parseExponent();
        while (!currentTokens.empty() && (currentTokens.front().value == "*" || currentTokens.front().value == "/" ||
                                          currentTokens.front().value == "%")) {
            char op = currentTokens.front().value[0];
            consumeToken();
            auto right = parseExponent();
            node = std::make_unique<BinaryExpression<T>>(op, std::move(node), std::move(right));
        }
        return node;
    }

    std::unique_ptr<IExpression<T>> parseExponent() {
        auto node = parseUnary();
        while (!currentTokens.empty() && currentTokens.front().value == "^") {
            char op = currentTokens.front().value[0];
            consumeToken();
            auto right = parseUnary();
            node = std::make_unique<BinaryExpression<T>>(op, std::move(node), std::move(right));
        }
        return node;
    }

    std::unique_ptr<IExpression<T>> parseUnary() {
        if (!currentTokens.empty() && (currentTokens.front().value == "+" || currentTokens.front().value == "-")) {
            char op = currentTokens.front().value[0];
            consumeToken();
            auto operand = parseUnary();
            return std::make_unique<UnaryExpression<T>>(op, std::move(operand));
        }
        return parseFactor();
    }

    std::unique_ptr<IExpression<T>> parseFactor() {
        if (currentTokens.empty()) {
            throw std::runtime_error("Unexpected end of expression");
        }
        if (currentTokens.front().type == Token::Type::Number) {
            T value = stringTools::stringToNumber<T>(currentTokens.front().value);
            consumeToken();
            return std::make_unique<NumberExpression<T>>(value);
        } else if (currentTokens.front().type == Token::Type::Parentesis && currentTokens.front().value == "(") {
            consumeToken();
            auto node = parseExpression();
            if (currentTokens.empty() || currentTokens.front().type != Token::Type::Parentesis ||
                currentTokens.front().value != ")") {
                throw std::runtime_error("Expected ')'");
            }
            consumeToken();
            return node;
        } else {
            throw std::runtime_error("Unexpected token: " + currentTokens.front().value);
        }
    }
};

} // namespace gltk
