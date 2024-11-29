#include "parser.h"

/// Private 

int Parser::adj_table[MAX_ROWS][MAX_COLUMNS];
Map<string, long> Parser::_keywords;

void Parser::build_keyword_list() {
    if (_keywords.empty()) {
        string standard[] = { "select", "from", "where", "insert", "into",
            "make", "table", "*", "," };
        for (long k = SELECT; k <= COMMA; k++)
            _keywords.insert(standard[k - 1], k);

        string relational[] = { "=", "<", ">", "<=", ">=", "!=" };
        for (string r : relational)
            _keywords.insert(r, RELATIONAL_OPERATOR);

        string logical[] = { "and", "or", "not", "AND", "OR", "NOT"};
        for (string l : logical)
            _keywords.insert(l, LOGICAL_OPERATOR);

        // custom enums
        _keywords.insert("(", LPARENT);
        _keywords.insert(")", RPARENT);
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

        // INSERT (states 25-29)

        mark_fail(_table, 20);
        mark_fail(_table, 21);
        mark_fail(_table, 22);
        mark_fail(_table, 23);

        mark_success(_table, 24);

        mark_cell(0, _table, INSERT, 20);
        mark_cell(20, _table, INTO, 21);
        mark_cell(21, _table, SYMBOL, 22);
        mark_cell(22, _table, VALUES, 23);

        mark_cell(23, _table, SYMBOL, 24);
        mark_cell(24, _table, COMMA, 23);

        // MAKE

        mark_fail(_table, 40);
        mark_fail(_table, 41);
        mark_fail(_table, 43);

        mark_success(_table, 42); ///< TODO : Mark fail and see if a command that makes a table with no fields is invalid
        mark_success(_table, 44);

        mark_cell(0, _table, MAKE, 40);
        mark_cell(40, _table, TABLE, 41);
        mark_cell(41, _table, SYMBOL, 42);
        mark_cell(42, _table, FIELDS, 43);
        mark_cell(43, _table, SYMBOL, 44);
        mark_cell(44, _table, COMMA, 43);

        // SELECT (states 1-17)

        mark_fail(_table, 1);
        mark_fail(_table, 2);
        mark_fail(_table, 3);
        mark_fail(_table, 5);
        mark_fail(_table, 6);
        mark_fail(_table, 7);
        mark_fail(_table, 9);
        mark_fail(_table, 10);
        mark_fail(_table, 11);
        mark_fail(_table, 12);
        mark_fail(_table, 14);
        mark_fail(_table, 15);
        mark_fail(_table, 16);
        mark_fail(_table, 17);

        mark_success(_table, 4);
        mark_success(_table, 8);
        mark_success(_table, 13);

        mark_cell(0, _table, SELECT, 1);
        mark_cell(1, _table, SYMBOL, 16);
        mark_cell(16, _table, COMMA, 17);
        mark_cell(17, _table, SYMBOL, 16);
        mark_cell(16, _table, FROM, 3);
        mark_cell(1, _table, ASTERIK, 2);
        mark_cell(2, _table, FROM, 3);
        mark_cell(3, _table, SYMBOL, 4);
        mark_cell(4, _table, WHERE, 5);

        mark_cell(5, _table, SYMBOL, 6);
        mark_cell(6, _table, RELATIONAL_OPERATOR, 7);
        mark_cell(7, _table, SYMBOL, 8);
        mark_cell(8, _table, LOGICAL_OPERATOR, 14);

        mark_cell(8, _table, RPARENT, 13); ///< Logically doesn't make sense but I have parenth_check in get_parse_tree()

        mark_cell(14, _table, SYMBOL, 6);
        mark_cell(14, _table, LPARENT, 9);


        mark_cell(5, _table, LPARENT, 9);
        mark_cell(9, _table, SYMBOL, 10);
        mark_cell(10, _table, RELATIONAL_OPERATOR, 11);
        mark_cell(11, _table, SYMBOL, 12);
        mark_cell(12, _table, RPARENT, 13);
        mark_cell(13, _table, RPARENT, 13);
        mark_cell(13, _table, LOGICAL_OPERATOR, 14);

        mark_cell(12, _table, LOGICAL_OPERATOR, 15);
        mark_cell(15, _table, LPARENT, 9);
        mark_cell(15, _table, SYMBOL, 10);

    }
}

bool Parser::get_parse_tree(Queue<string> q) {
    const bool debug = true;
    if (debug) cout << "get_parse_tree() q = " << q << endl;

    int state = 0;
    vector<char> parenth_check;

    while (!q.empty() && state != -1) {
        string token = q.pop();
        Keyword key_type = get_column(token);
        size_t prev_state = state;
        state = adj_table[state][key_type];

        if (state == -1)
            assert(key_type != SYMBOL && prev_state != 15);    

        if (debug) cout << "get_parse_tree() : token = " << token << " | state = " << state << endl;

        switch (state) {
            case 40: ///< make
            case 20: ///< insert
            case 1: ///< select
                ptree.get("command").push_back(token);
                break;
            case 42: ///< make table <symbol>
            case 22: ///< insert into <symbol>
            case 4: ///< select ... from <symbol>
                ptree.get("table_name").push_back(token);
                break;
            case 44: ///< make table ... fields <symbol, ...>
                ptree.get("col").push_back(token);
                break;
            case 24: ///< insert into ... values <symbol, ...>
                ptree.get("values").push_back(token);
                break;
            case 2: ///< select <*>
            case 16: ///< select <field, ...>
                ptree.get("fields").push_back(token);
                break;
            case 5: ///< where
                ptree.get("where").push_back("yes");
                break;
            case 6:
            case 7:
            case 8:
            case 10:
            case 11:
            case 12:
            case 14:
            case 15:
                ptree.get("condition").push_back(token);
                break;
            case 9: ///< Handle parenthesis (TODO : See if I can combine these two cases with the previous ones above)
            case 13:
                ptree.get("condition").push_back(token);
                if (token == "(")
                    parenth_check.push_back(token.front());
                else if(token == ")") {
                    if (parenth_check.empty()) {
                        /// Invalid command if parenthesis are out of whack | ex. ... where )(fname = first)
                        if (debug) cout << "get_parse_tree() : Invalid number of RPARENS for _buffer = " << _buffer << endl;
                        state = -1;
                        break;
                    }
                    parenth_check.pop_back();
                }
                break;
            default:
                break;
        }
    }

    invalid_query = parenth_check.size() != 0 || !is_success(adj_table, state);
    if (invalid_query) {
        if (debug && parenth_check.size() != 0) cout << "get_parse_tree() : Invalid number of parenthesis for _buffer = " << _buffer << endl;
        ptree.clear();
    }

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
    const bool debug = false;
    /// Init Parser::_buffer
    strncpy(_buffer, s, MAX_BUFFER);
    _buffer[MAX_BUFFER] = '\0';

    /// Reset Parser fields
    input_q.clear();
    ptree.clear();
    invalid_query = false;

    if(debug) cout << "set_string() : _buffer = " << _buffer << endl;

    /// Tokenize _buffer into input_q
    STokenizer stk(_buffer);

    string quote_token;
    char* quote_used = nullptr;
    bool quote_flag = false;

    string token;
    stk >> token;
    while(stk.more()) {
        if(debug) cout << "set_string() : token = " << token << endl;
        if (token == "\"" || token == "\'") {
            if (quote_used == nullptr) quote_used = new char(token.front());
            if (debug) cout << "*quote_used = " << (*quote_used) << endl;
            if (token.front() == *quote_used)
                quote_flag = !quote_flag;
            else
                quote_token.append(token);

            if (!quote_flag) {
                input_q.push(quote_token);
                quote_token.clear();
            }
            stk >> token;
            continue;
        }

        if (quote_flag)
            quote_token.append(token);
        else
            if(token.find(' ') == -1 && token.find('\t') == -1 && token.find('\n') == -1) input_q.push(token); ///< Discard SPACES tokens if not quote
        stk >> token;
    }

    if (debug) cout << "set_string() input_q = : " << input_q << endl;

    get_parse_tree(input_q);
}

Keyword Parser::get_column(string token) {
    assert(!token.empty());
    if (!_keywords.contains(token)) return SYMBOL;
    return static_cast<Keyword>(_keywords.get(token));
}

bool Parser::fail() {
    return invalid_query;
}

MMap<string, string>& Parser::parse_tree() {
    return ptree;
}
