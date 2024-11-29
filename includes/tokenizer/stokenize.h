#ifndef STOKENIZE_H
#define STOKENIZE_H

#include <cassert>
#include <cstring>
#include "../token/child_tokens.h"
#include "constants.h"
#include "state_machine_functions.h"

class STokenizer {
private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table(int _table[][MAX_COLUMNS]);

    //extract the longest string that match
    //     one of the acceptable token types
    bool get_token(int& start_state, string& token);
    //---------------------------------
    unsigned char _buffer[MAX_BUFFER + 1];       //input string
    int _pos;                       //current position in the string
    static int _table[MAX_ROWS][MAX_COLUMNS];

    bool _done; ///< True if no more tokens to be extracted, false otherwise

public:
    STokenizer();
    STokenizer(char str[]);
    bool done();            //true: there are no more tokens
    bool more();            //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, string& t) {
        string token_str;
        int state = 0;
        bool token_retrieved = s.get_token(state, token_str);
        if (token_retrieved)    
            t = token_str;
        return s;
    }

    //set a new string as the input string
    void set_string(char str[]);
};

#endif