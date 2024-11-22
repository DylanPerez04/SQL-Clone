#include "parser.h"

/// Private 

int Parser::adj_table[MAX_ROWS][MAX_COLUMNS];
Map<string, long> Parser::_keywords;

void Parser::build_keyword_list() {
    if (_keywords.empty()) {
        string standard[] = { "select", "from", "where", "insert", "into",
            "make", "table", "*", "\"", "," };
        for (long k = SELECT; k <= COMMA; k++)
            _keywords.insert(standard[k - 1], k);
        _keywords.insert("\'", QUOTE);

        string relational[] = { "=", "<", ">", "<=", ">=", "!=" };
        for (string r : relational)
            _keywords.insert(r, RELATIONAL_OPERATOR);

        string logical[] = { "and", "or" };
        for (string l : logical)
            _keywords.insert(l, LOGICAL_OPERATOR);

        // custom enums
        _keywords.insert("fields", FIELDS);
        _keywords.insert("values", VALUES);
    }
}

void Parser::make_table(int _table[][MAX_COLUMNS]) {
    const bool debug = false;
    // Column 0 of state 1 will always be -1 (ensures to not repeat make_table() functionality)
    if (_table[1][0] != -1) {
        if (debug) cout << "make_table() : adj_table not initialized! Making table..." << endl;
        init_table(_table); ///< Clear table

        mark_fail(_table, 0);

        // INSERT

        mark_fail(_table, 20);
        mark_fail(_table, 21);
        mark_fail(_table, 22);
        mark_fail(_table, 23);
        mark_fail(_table, 25);
        mark_fail(_table, 26);
        mark_fail(_table, 27);

        mark_success(_table, 24);
        mark_success(_table, 28);


        mark_cell(0, _table, INSERT, 20);
        mark_cell(20, _table, INTO, 21);
        mark_cell(21, _table, SYMBOL, 22);
        mark_cell(22, _table, VALUES, 23);

        mark_cell(23, _table, SYMBOL, 24);
        mark_cell(24, _table, COMMA, 25);
        mark_cell(25, _table, SYMBOL, 24);

        mark_cell(23, _table, TK_STRING, 24);
        mark_cell(25, _table, TK_STRING, 24);

        //mark_cell(23, _table, QUOTE, 26);
        //mark_cell(25, _table, SYMBOL, 24);
        //mark_cell(25, _table, QUOTE, 26);
        //mark_cell(26, _table, SYMBOL, 27);
        //mark_cell(27, _table, SYMBOL, 27);
        //mark_cell(27, _table, QUOTE, 28);

        // MAKE

        mark_fail(_table, 40);
        mark_fail(_table, 41);
        mark_fail(_table, 43);
        mark_fail(_table, 45);

        mark_success(_table, 42);
        mark_success(_table, 44);

        mark_cell(0, _table, MAKE, 40);
        mark_cell(40, _table, TABLE, 41);
        mark_cell(41, _table, SYMBOL, 42);
        mark_cell(42, _table, FIELDS, 43);
        mark_cell(43, _table, SYMBOL, 44);
        mark_cell(44, _table, COMMA, 45);
        mark_cell(45, _table, SYMBOL, 44);

        // SELECT

        mark_fail(_table, 5);
        mark_fail(_table, 6);
        mark_fail(_table, 7);
        mark_fail(_table, 9);
        mark_fail(_table, 10);
        mark_fail(_table, 11);
        mark_fail(_table, 13);
        mark_fail(_table, 14);
        mark_fail(_table, 15);
        mark_fail(_table, 17);
        mark_fail(_table, 18);

        mark_success(_table, 8);
        mark_success(_table, 12);
        mark_success(_table, 16);

        mark_cell(0, _table, SELECT, 5);
        mark_cell(5, _table, SYMBOL, 17);
        mark_cell(17, _table, COMMA, 18);
        mark_cell(18, _table, SYMBOL, 17);
        mark_cell(17, _table, FROM, 7);
        mark_cell(5, _table, ASTERIK, 6);
        mark_cell(6, _table, FROM, 7);
        mark_cell(7, _table, SYMBOL, 8);
        mark_cell(8, _table, WHERE, 9);
        mark_cell(9, _table, SYMBOL, 10);
        mark_cell(10, _table, RELATIONAL_OPERATOR, 11);
        mark_cell(11, _table, SYMBOL, 12);
        mark_cell(12, _table, LOGICAL_OPERATOR, 13);
        mark_cell(13, _table, SYMBOL, 10);
        mark_cell(11, _table, TK_STRING, 12);
        //mark_cell(11, _table, QUOTE, 14);
        //mark_cell(14, _table, SYMBOL, 15);
        //mark_cell(15, _table, SYMBOL, 15);
        //mark_cell(15, _table, QUOTE, 16);
        mark_cell(16, _table, LOGICAL_OPERATOR, 13);
    }
}

bool Parser::get_parse_tree(Queue<string> q) {
    const bool debug = true;
    if (debug) cout << "get_parse_tree() q = " << q << endl;

    /*
    * push_back parenthesis but do not consider them during the adjacency table; keep track of string to insert when finding QUOTE (")
    */

    int state = 0;
    int rparens_needed = 0;
    while (!q.empty() && state != -1) {
        string token = q.pop();

        if (token == "(" || token == ")") {
            ptree.get("condition").push_back(token);
            if (token == "(") rparens_needed++;
            else rparens_needed--;
            continue;
        }

        Keyword key_type = get_column(token);
        state = adj_table[state][key_type];

        if (key_type == TK_STRING) {
            token.pop_back();
            token.erase(0, 1);
        }

        if (debug) cout << "get_parse_tree() : token = " << token << " | state = " << state << endl;

        switch (state) {
        case 40: // make
        case 20: // insert
        case 5: // select
            ptree.get("command").push_back(token);
            break;
        case 42: // symbol
        case 22:
        case 8:
            ptree.get("table_name").push_back(token);
            break;
        case 44: // symbol
            ptree.get("col").push_back(token);
            break;
        case 24: // symbol
            ptree.get("values").push_back(token);
            break;
        //case 27:
        //case 15:
        //    if (!to_insert.empty()) to_insert.push_back(' ');
        //    to_insert.append(token);
        //    if (debug) cout << "get_parse_tree() : to_insert = " << to_insert << endl;
        //    break;
        //case 28:
        //    ptree.get("values").push_back(to_insert);
        //    to_insert.clear();
        //    break;
        case 6:
        case 17:
            ptree.get("fields").push_back(token);
            break;
        case 9: // where
            ptree.get("where").push_back("yes");
            break;
        case 10: // symbol
            ptree.get("condition").push_back(token);
            break;
        case 11:
        case 12:
        case 13:
            ptree.get("condition").push_back(token);
            break;
        //case 16:
        //    ptree.get("condition").push_back(to_insert);
        //    to_insert.clear();
        //    break;
        default:
            break;
        }
    }

    invalid_query = !is_success(adj_table, state) && rparens_needed != 0;
    if (invalid_query) ptree.clear();

    if (debug) cout << "get_parse_tree() : invalid_query = " << boolalpha << invalid_query << endl;
    if (debug) cout << ptree << endl;

    return !invalid_query;
}

/// Public

Parser::Parser(const char* s) : invalid_query(false) {
    const bool debug = false;
    if (debug) cout << "Parser() : " << endl;
    /// Initialize "make" adjacency table

    make_table(adj_table);

    /// build keyword list
    build_keyword_list();

    /// Copy string to internal buffer and tokenize buffer
    set_string(s);
}

void Parser::set_string(const char* s) {
    const bool debug = true;
    strncpy(_buffer, s, MAX_BUFFER);
    _buffer[MAX_BUFFER] = '\0';
    input_q.clear();
    ptree.clear();

    if(debug) cout << "set_string() : _buffer = " << _buffer << endl;

    /// Tokenize _buffer into input_q
    STokenizer stk(_buffer);
    string token;
    stk >> token;
    while(stk.more()) {
        if(debug) cout << "set_string() : token = " << token << endl;
        input_q.push(token);
        stk >> token;
    }

    if (debug) cout << "set_string() input_q = : " << input_q << endl;

    get_parse_tree(input_q);
}

Keyword Parser::get_column(string token) {
    assert(!token.empty());
    if ((token.front() == '\"' && token.back() == '\"')
        || (token.front() == '\'' && token.back() == '\'')) {
        return TK_STRING;
    }
    if (!_keywords.contains(token)) return SYMBOL;
    return static_cast<Keyword>(_keywords.get(token));
}

bool Parser::fail() {
    return invalid_query;
}

MMap<string, string>& Parser::parse_tree() {
    return ptree;
}
