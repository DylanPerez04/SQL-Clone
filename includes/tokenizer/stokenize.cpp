#include "stokenize.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer() : _buffer(""), _pos(0), _done(true) {
    const bool debug = false;
    // Ensures make_table() isn't called twice
    // row 1 is a success state (will make table if it hasn't been initialized so)
    if (_table[1][0] != 1) {
        if (debug) cout << "STokenizer() : Calling make_table()!" << endl;
        make_table(_table);
    }

    //if (debug) print_table(_table);
}

STokenizer::STokenizer(char str[]) : STokenizer() {
    set_string(str);
}

/**
    @returns True if STokenizer has read all tokens, false otherwise.
*/
bool STokenizer::done() {
    return _done;
}

bool STokenizer::more() {
    return !done();
}

/**
* Overwrites _buffer with the contents of #param str.Sets _pos = 0 and _done = false.
* 
* @param str The new string to overwrite _buffer and be tokenized.
*/
void STokenizer::set_string(char str[]) {
    _pos = 0;
    _done = false;

    // TODO : Use strncpy()

    // i <= strlen(str) so that '\0' is writtein into _buffer
    for (int i = 0; i <= strlen(str) && i < MAX_BUFFER; i++)
        _buffer[i] = str[i];
    _buffer[MAX_BUFFER] = '\0';
}

/**
* Retrieves a token from the STokenizer instance's _buffer cstring, making #param token equal to the token found,
* and #param start_state equal to the last success state jumped to when reading the last character of the new token
* found.
* 
* @param start_state[in, out] The initial state that will be jumped from when reading a char from _buffer for a new token
* @param token[in, out] A reference to string that will be made equal to the token found if a token is found.
* 
* @returns True if a token has been successfully retrieved, false otherwise.
*/
bool STokenizer::get_token(int& start_state, string& token) {
    const bool debug = false;

    assert(_pos >= 0 && start_state >= 0);

    int current_state = start_state; ///< The row in _table currently being looked at.

    size_t 
        current_pos = _pos, ///< The index of the char from _buffer being looked at
        token_pos = _pos; ///< The first index of the substr of _buffer containing a valid token
    unsigned int token_length = 0; ///<  The length of the substring of _buffer containing a valid token

    
     
    /// If get_token() is called, and the first character that would be processed is a '\0', then set _done = true.
    _done = _buffer[current_pos] == '\0';

    while (_buffer[current_pos] != '\0') {
        if (debug) {
            cout << "reading " << _buffer[current_pos] << " (" << (int) _buffer[current_pos] <<  ") ..." << endl
                << "current_state = " << current_state
                << " | table state = " << _table[current_state][(int)_buffer[current_pos]] << endl;
        }

        current_state = _table[current_state][(int)_buffer[current_pos++]];

        if (current_state < 0) {
            if (token_length == 0) { // process unknown characters
                _pos = current_pos;
                token_length = _pos - token_pos;
                start_state = current_state;
            }
            break;
        } else if (_table[current_state][0] == 1) { // reached success state
            _pos = current_pos;
            token_length = _pos - token_pos;
            start_state = current_state;
        }
    }
    token.reserve(token_length);
    token = "";
    if (token_length > 0) {
        for (int i = token_pos; i < token_pos + token_length; i++)
            token.push_back(_buffer[i]);
    }
    return !token.empty();
}

void STokenizer::make_table(int _table[][MAX_COLUMNS]) {
    init_table(_table);

    //doubles:
    mark_fail(_table, 0);            //Mark states 0 and 2 as fail states
    mark_success(_table, 1);         //Mark states 1 and 3 as success states
    mark_fail(_table, 2);
    mark_success(_table, 3);

    mark_cells(0, _table, DIGITS, 1);    //state [0] --- DIGITS ---> [1]
    mark_cells(1, _table, DIGITS, 1);    //state [1] --- DIGITS ---> [1]
    mark_cells(1, _table, '.', '.', 2);  //state [1] --- '.' ------> [2] 
    mark_cells(2, _table, DIGITS, 3);    //state [2] --- DIGITS ---> [3]
    mark_cells(3, _table, DIGITS, 3);    //state [3] --- DIGITS ---> [3]

    //spaces:
    mark_success(_table, 4);

    mark_cells(0, _table, SPACES, 4);
    mark_cells(4, _table, SPACES, 4);

    //alphas:
    mark_success(_table, 6);

    mark_cell(0, _table, (int)'_', 6);
    mark_cells(0, _table, ALFA, 6);
    mark_cell(6, _table, (int)'_', 6);
    mark_cells(6, _table, ALFA, 6);

    //punctuation:
    mark_success(_table, 10);
    mark_success(_table, 11);
    mark_success(_table, 12);
    
    // ?,:;`~
    mark_cells(0, _table, PUNC, 10);
    // '!', '!= (operator)'
    mark_cell(0, _table, (int)'!', 11);
    mark_cell(11, _table, (int)'=', 22);
    //  '.', '.123 (number)'
    mark_cell(0, _table, (int)'.', 12);
    mark_cells(12, _table, DIGITS, 3);

    //operators:
    mark_success(_table, 20);
    mark_success(_table, 21);
    mark_success(_table, 22);
    mark_success(_table, 23);
    mark_success(_table, 24);
    mark_success(_table, 25);
    mark_fail(_table, 26);
    mark_fail(_table, 27);

    // '=', '==', '==='
    mark_cell(0, _table, (int)'=', 20);
    mark_cell(20, _table, (int)'=', 21);
    mark_cell(21, _table, (int)'=', 22);
    // '>', '<', '<=', '>='
    char a_brackets[] = { '<', '>' };
    mark_cells(0, _table, a_brackets, 21);
    // '+', '+-', '++', '-1.0 (number)', '-.1 (number)'
    mark_cell(0, _table, (int)'+', 23);
    char pm[] = { '+', '-' };
    mark_cells(23, _table, pm, 22);
    mark_cell(0, _table, (int)'-', 24);
    mark_cell(24, _table, (int)'-', 22);
    mark_cells(24, _table, DIGITS, 1);
    mark_cell(24, _table, (int)'.', 2);

    // '%', '%%', '&&', '||'
    mark_cell(0, _table, (int)'%', 25);
    mark_cell(25, _table, (int)'%', 22);
    mark_cell(0, _table, (int)'&', 26);
    mark_cell(26, _table, (int)'&', 22);
    mark_cell(0, _table, (int)'|', 27);
    mark_cell(27, _table, (int)'|', 22);
}