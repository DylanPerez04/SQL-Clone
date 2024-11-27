#ifndef CHILD_TOKENS_H
#define CHILD_TOKENS_H

#include <vector>

#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "token.h"

enum OperatorType {
    OR,
    AND,
    EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_EQUAL,
    GREATER_EQUAL,
    NOT_EQUAL,
    UNKNOWN // custom enum used for return purposes
};

struct ResultSet : public Token {
private:
    std::vector<long> _result_set;
public:
    ResultSet();
    ResultSet(const vector<long> set);

    bool add(const long recnum);

    /*
    * AND
    */
    ResultSet* set_intersect(const ResultSet* o);

    /*
    * OR
    */
    ResultSet* set_union(const ResultSet* o);

    std::vector<long> vector_recnos();
};


struct TokenStr : public Token {
    TokenStr(string _token);
};

struct Operator : public Token {
private:
    OperatorType _op_type;
    static Map<string, long> OPERATOR_CODES;
public:
    Operator(string _token, TokenType _type);

    // RPN
    virtual ResultSet* eval(MMap<string, long>& map, Token*& lhs, Token*& rhs) = 0;

    OperatorType op_type() const {
        return _op_type;
    }

    static OperatorType get_operator(string op) {
        if (OPERATOR_CODES.empty()) {
            const string ops[9] = { "or", "and", "=", "<", ">", "<=", ">=", "!=" };
            for (int op = OR; op != UNKNOWN; op++)
                OPERATOR_CODES.insert(ops[op], op);

            OPERATOR_CODES.insert("OR", OR);
            OPERATOR_CODES.insert("AND", AND);
        }
        if (!OPERATOR_CODES.contains(op)) return UNKNOWN;
        return static_cast<OperatorType>(OPERATOR_CODES.get(op));
    }
};

// <, >, =, etc
struct Relational : public Operator {
    Relational(string _token);

    virtual ResultSet* eval(MMap<string, long>& map, Token*& lhs, Token*& rhs) {
        const bool debug = false;

        if (debug) cout << "Relational::eval() : " << endl;

        ResultSet* result = new ResultSet();

        switch (this->op_type()) {
            case EQUAL:
                return new ResultSet(map.get(rhs->token_str()));
            case LESS_THAN:
                for (MMap<string, long>::Iterator it = map.begin(); it != map.end() && (*it).key < rhs->token_str(); it++)
                    result = result->set_union(new ResultSet((*it).value_list));
                break;
            case GREATER_THAN:
                for (MMap<string, long>::Iterator it = map.upper_bound(rhs->token_str()); it != map.end(); it++)
                    result = result->set_union(new ResultSet((*it).value_list));
                break;
            case LESS_EQUAL:
                for (MMap<string, long>::Iterator it = map.begin(); it != map.end() && (*it).key <= rhs->token_str(); it++)
                    result = result->set_union(new ResultSet((*it).value_list));
                break;
            case GREATER_EQUAL:
                for (MMap<string, long>::Iterator it = map.lower_bound(rhs->token_str()); it != map.end(); it++)
                    result = result->set_union(new ResultSet((*it).value_list));
                break;
            case NOT_EQUAL:
                for (MMap<string, long>::Iterator it = map.begin(); it != map.end(); it++)
                    if ((*it).key != rhs->token_str()) result = result->set_union(new ResultSet((*it).value_list));
                break;
            default:
                break;
        }
        return result;
    }
};

// AND, OR, etc
struct Logical : public Operator {
    Logical(string _token);

    virtual ResultSet* eval(MMap<string, long>& map, Token*& lhs, Token*& rhs) {
        ResultSet* lhs_set = static_cast<ResultSet*>(lhs);

        switch (this->op_type()) {
            case AND:
                return lhs_set->set_intersect(static_cast<ResultSet*>(rhs));
            case OR:
                return lhs_set->set_union(static_cast<ResultSet*>(rhs));
            default: 
                break;
        }
        return nullptr;
    }

};

struct LeftParen : public Token {
    LeftParen();

};

struct RightParen : public Token {
    RightParen();
};
#endif