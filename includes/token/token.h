#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

using namespace std;

enum TokenType {
    STRING,
    RPAREN, // )
    LPAREN, // (
    LOGIC,
    RELAT,
    RESULT
};

class Token {
private:
    string _token;
    TokenType _type;

public:
    Token(string token, TokenType type);
    TokenType type() const;
    //string type_string();
    string token_str() const;

    virtual ~Token();

    friend ostream& operator <<(ostream& outs, const Token& t) {
        outs << "|" << t.token_str() << "|";
        return outs;
    }

};

#endif