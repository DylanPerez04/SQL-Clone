#ifndef PARSER_H
#define PARSER_H

#include "../queue/MyQueue.h"
#include "../token/child_tokens.h"
#include "../tokenizer/constants.h"
#include "../tokenizer/stokenize.h"

enum Keyword {
    SELECT = 1,
    FROM,
    WHERE,
    INSERT,
    INTO,
    MAKE,
    //CREATE,
    TABLE,
    ASTERIK,
    QUOTE,
    COMMA,
    RELATIONAL_OPERATOR, // <, >, <=, >=, etc
    LOGICAL_OPERATOR, // and, or, etc
    SYMBOL,
    //NUMBER,
    //TK_TABLE,
    /// Custom enums
    LPARENT,
    RPARENT,
    FIELDS,
    VALUES
}; //14

class Parser {
private:
    MMap<string, string> ptree;
    Queue<string> input_q;

    char _buffer[MAX_BUFFER + 1]; // no limit to command length
    static int adj_table[MAX_ROWS][MAX_COLUMNS];

    bool invalid_query;

    static Map<string, long> _keywords;

    //map_st OPERATOR_LOGICAL
    //map_st OPERATOR_RELATIONAL;

    void build_keyword_list();

    void make_table(int _table[][MAX_COLUMNS]);

    /*
    * Sets ptree. Returns true if ends at a success state (valid SQL query).
    * False otherwise.
    */
    bool get_parse_tree(Queue<string> q);

public:
    Parser(const char* s);

    /*
    * Tokenizes _buffer into input_q. Calls get_parse_tree().
    * Sets/resets invalid_query
    */
    void set_string(const char* s);

    Keyword get_column(string token);

    bool fail();

    MMap<string, string>& parse_tree();

};

#endif // !PARSER_H
