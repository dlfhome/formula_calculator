#include "formula/token.h"

namespace formula {

const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::NUMBER:         return "NUMBER";
        case TokenType::IDENTIFIER:     return "IDENTIFIER";
        case TokenType::PLUS:           return "PLUS";
        case TokenType::MINUS:          return "MINUS";
        case TokenType::MULTIPLY:       return "MULTIPLY";
        case TokenType::DIVIDE:         return "DIVIDE";
        case TokenType::MODULO:         return "MODULO";
        case TokenType::POWER:          return "POWER";
        case TokenType::GREATER:        return "GREATER";
        case TokenType::LESS:           return "LESS";
        case TokenType::EQUAL:          return "EQUAL";
        case TokenType::GREATER_EQUAL:  return "GREATER_EQUAL";
        case TokenType::LESS_EQUAL:     return "LESS_EQUAL";
        case TokenType::NOT_EQUAL:      return "NOT_EQUAL";
        case TokenType::LOGICAL_AND:    return "LOGICAL_AND";
        case TokenType::LOGICAL_OR:     return "LOGICAL_OR";
        case TokenType::LOGICAL_NOT:    return "LOGICAL_NOT";
        case TokenType::QUESTION:       return "QUESTION";
        case TokenType::COLON:          return "COLON";
        case TokenType::LEFT_PAREN:     return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:    return "RIGHT_PAREN";
        case TokenType::COMMA:          return "COMMA";
        case TokenType::END:            return "END";
        case TokenType::INVALID:        return "INVALID";
        default:                        return "UNKNOWN";
    }
}

} // namespace formula
