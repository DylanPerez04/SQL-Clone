#include "child_tokens.h"
    
// TokenStr

TokenStr::TokenStr(string _token) : Token(_token, STRING) {}

// Operators

Map<string, long> Operator::OPERATOR_CODES{};

Operator::Operator(string _token, TokenType _type) : Token(_token, _type) {
    this->_op_type = get_operator(_token);
}

Operator::~Operator() {}

OperatorType Operator::op_type() const { return _op_type; }

// <, >, =, etc
Relational::Relational(string _token) : Operator(_token, RELAT) {}

Relational::~Relational() {}

// AND, OR, etc
Logical::Logical(string _token) : Operator(_token, LOGIC) {}

Logical::~Logical() {}

// LeftParen

LeftParen::LeftParen() : Token("(", LPAREN) {}

// RightParen

RightParen::RightParen() : Token(")", RPAREN) {}

// ResultSet
ResultSet::ResultSet() : Token("", RESULT) {}

ResultSet::ResultSet(const vector<long> set) : Token("", RESULT) {
    _result_set = set;

    /// Check if #param set is sorted, if not, run insertion sort (O(n))
    bool is_sorted = true;
    for(size_t i = 1; i < _result_set.size() && is_sorted; i++)
        is_sorted = _result_set[i] >= _result_set[i - 1];

    if (!is_sorted) {
        // insertion sort
        for (size_t i = 1; i < _result_set.size(); i++) {
            size_t swap_index = i;
            for (size_t j = i - 1; j >= 0 && _result_set[i] < _result_set[j]; j--)
                swap_index = j;
            if (i != swap_index) {
                int temp = _result_set[i];
                _result_set[i] = _result_set[swap_index];
                _result_set[swap_index] = temp;
            }
        }
    }
}

bool ResultSet::add(const long recnum) {
    const bool debug = false;
    if (debug) cout << "add() : Adding " << recnum << " to ResultSet" << endl;
    
    // binary insert
    int lower_lim = 0, upper_lim = _result_set.size();
    for (int i = (upper_lim + lower_lim) / 2; lower_lim < upper_lim; i = (upper_lim + lower_lim) / 2) {
        int current = _result_set[i];
        if (current == recnum) return false;
        if (current < recnum) 
            lower_lim = i + 1;
        else // if (current > recnum)
            upper_lim = i;
    }

    if (lower_lim == upper_lim) _result_set.insert(_result_set.begin() + lower_lim, recnum);
    else if (lower_lim >= _result_set.size()) _result_set.push_back(recnum);
    else _result_set.insert(_result_set.begin(), recnum);

    if (debug) {
        cout << "cond() : _result_set = [";
        std::vector<long>::iterator it = _result_set.begin();
        while (it != _result_set.end())
            cout << *it++ << " ";
        cout << "]" << endl;
    }

    return true;
}

ResultSet* ResultSet::set_intersect(const ResultSet* o) {
    const bool debug = false;
    if (debug) cout << "set_intersection() : this->_result_set.size() = " << this->_result_set.size() << " | o->_result_set.size() = " << o->_result_set.size() << endl;

    ResultSet* intersect = new ResultSet();

    // double-pointer method O(n_1 + n_2)

    int index_a = 0, index_b = 0;
    while (index_a < this->_result_set.size() && index_b < o->_result_set.size()) {
        long recnum_a = this->_result_set[index_a], recnum_b = o->_result_set[index_b];
        if (recnum_a == recnum_b) {
            intersect->add(recnum_a);
            index_a++;
            index_b++;
        }
        else if (recnum_a < recnum_b) {
            index_a++;
        }
        else { // if(recnum_a > recnum_b)
            index_b++;
        }
    }

    if (debug) {
        cout << "set_intersection() : intersect = [";
        for (int i = 0; i < intersect->vector_recnos().size(); i++)
            cout << intersect->vector_recnos().at(i) << " ";
        cout << "]" << endl;
    }

    return intersect;
}

ResultSet* ResultSet::set_union(const ResultSet* o) {
    const bool debug = false;
    if (debug) cout << "set_union() : this->_result_set.size() = " << this->_result_set.size() << " | o->_result_set.size() = " << o->_result_set.size() <<  endl;
    vector<long> _union;

    size_t index_a = 0, index_b = 0;
    long recno_a = 0, recno_b = 0;
    long to_insert = 0;
    while (index_a < this->_result_set.size() && index_b < o->_result_set.size()) {
        recno_a = this->_result_set[index_a]; recno_b = o->_result_set[index_b];
        if (recno_a == recno_b) {
            to_insert = recno_a; // eliminate duplicate values (O(n_1 + n_2))
            index_a++;
            index_b++;
        }
        else if (recno_a < recno_b) {
            to_insert = recno_a;
            index_a++;
        }
        else { // if (recno_a > recno_b)
            to_insert = recno_b;
            index_b++;
        }
        _union.push_back(to_insert);
    }

    /// Add remaining recnos 
    int i = (index_a < this->_result_set.size() ? index_a : index_b);
    const vector<long>& remaining = (index_a < this->_result_set.size() ? this->_result_set : o->_result_set);
    for (i; i < remaining.size(); i++) {
        _union.push_back(remaining.at(i));
    }

    if (debug) {
        cout << "set_union() : _union = [";
        for (int i = 0; i < _union.size(); i++)
            cout << _union[i] << " ";
        cout << "]" << endl;
    }

    return new ResultSet(_union);
}

std::vector<long> ResultSet::vector_recnos() {
    return this->_result_set;
}
