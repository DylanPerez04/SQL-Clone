#include "token.h"

Token::Token(string token, TokenType type) : _token(token), _type(type) {}

TokenType Token::type() const {
    return static_cast<TokenType>(_type);
}

string Token::token_str() const {
    return _token;
}
