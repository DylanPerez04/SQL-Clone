#include "child_tokens.h"
    
// TokenStr

TokenStr::TokenStr(string _token) : Token(_token, STRING) {}

// Operators

Map<string, long> Operator::OPERATOR_CODES = Map<string, long>();

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
    const bool debug = false;
    
    if(debug) cout << "ResultSet() : Start | set = " << set << endl;

    _result_set = set;

    // Bubble sort (for debugging purposes)
    size_t n = _result_set.size();
    for (size_t i = 1; i < n; i++) { ///< Bubble sort only needs to run n - 1 times
        bool sorted = true;
        for (size_t j = 0; j < n - i; j++) {
            if (_result_set[j] > _result_set[j + 1]) {
                sorted = false;
                long temp = _result_set[j];
                _result_set[j] = _result_set[j + 1];
                _result_set[j + 1] = temp;
            }
        }

        if (sorted) break;
    }

    if(debug) cout << "ResultSet() : End } _result_set = " << _result_set << endl;


    /// Check if #param set is sorted, if not, run insertion sort (O(n))
    //bool is_sorted = true;
    //for(size_t i = 1; i < _result_set.size() && is_sorted; i++)
    //    is_sorted = _result_set[i] >= _result_set[i - 1];

    //if (!is_sorted) {
    //    // insertion sort
    //    for (size_t i = 1; i < _result_set.size(); i++) {
    //        size_t swap_index = i;
    //        for (size_t j = i - 1; j >= 0 && _result_set[i] < _result_set[j]; j--)
    //            swap_index = j;
    //        if (i != swap_index) {
    //            int temp = _result_set[i];
    //            _result_set[i] = _result_set[swap_index];
    //            _result_set[swap_index] = temp;
    //        }
    //    }
    //}
}

bool ResultSet::add(const long recnum) {
    const bool debug = false;
    if (debug) cout << "add() : Adding " << recnum << " to ResultSet" << endl;
    
    for (int i = 0; i < _result_set.size(); i++) {
        if (_result_set[i] == recnum) return false;
        if (recnum < _result_set[i]) {
            _result_set.insert(_result_set.begin() + i, recnum);
            return true;
        }
    }

    _result_set.push_back(recnum);

    // binary insert
    //int lower_lim = 0, upper_lim = _result_set.size();
    //for (int i = (upper_lim + lower_lim) / 2; lower_lim < upper_lim; i = (upper_lim + lower_lim) / 2) {
    //    int current = _result_set[i];
    //    if (current == recnum) return false;
    //    if (current < recnum) 
    //        lower_lim = i + 1;
    //    else // if (current > recnum)
    //        upper_lim = i;
    //}

    //if (lower_lim == upper_lim) _result_set.insert(_result_set.begin() + lower_lim, recnum);
    //else if (lower_lim >= _result_set.size()) _result_set.push_back(recnum);
    //else _result_set.insert(_result_set.begin(), recnum);

    //if (debug) {
    //    cout << "cond() : _result_set = [";
    //    std::vector<long>::iterator it = _result_set.begin();
    //    while (it != _result_set.end())
    //        cout << *it++ << " ";
    //    cout << "]" << endl;
    //}

    //return true;
}

void ResultSet::set_intersect(ResultSet o) {
    const bool debug = false;
    if (debug) cout << "set_intersection() : this->_result_set.size() = " << this->_result_set.size() << " | o->_result_set.size() = " << o._result_set.size() << endl;

    ResultSet* intersect = new ResultSet();

    for (int i = 0; i < this->_result_set.size(); i++) {
        for (int j = 0; j < o._result_set.size(); j++) {
            if (this->_result_set[i] == o._result_set[j]) {
                intersect->add(this->_result_set[i]);
                break;
            }
        }
    }

    this->_result_set = intersect->_result_set;
    delete intersect;

    /// double-pointer method O(n_1 + n_2)

    //ResultSet* intersect = new ResultSet();

    //int index_a = 0, index_b = 0;
    //while (index_a < this->_result_set.size() && index_b < o._result_set.size()) {
    //    long recnum_a = this->_result_set[index_a], recnum_b = o._result_set[index_b];
    //    if (recnum_a == recnum_b) {
    //        intersect->add(recnum_a);
    //        index_a++;
    //        index_b++;
    //    }
    //    else if (recnum_a < recnum_b) {
    //        index_a++;
    //    }
    //    else { // if(recnum_a > recnum_b)
    //        index_b++;
    //    }
    //}

    //if (debug) {
    //    cout << "set_intersection() : intersect = [";
    //    for (int i = 0; i < intersect->vector_recnos().size(); i++)
    //        cout << intersect->vector_recnos().at(i) << " ";
    //    cout << "]" << endl;
    //}

    //this->_result_set = intersect->_result_set;
    //delete intersect;
}

void ResultSet::set_union(ResultSet o) {
    const bool debug = false;
    if (debug) cout << "set_union() : this->_result_set.size() = " << this->_result_set.size() << " | o->_result_set.size() = " << o._result_set.size() <<  endl;

    for (int i = 0; i < o._result_set.size(); i++) {
        bool dup_found = false;
        for (int j = 0; j < this->_result_set.size(); j++) {
            if (o._result_set[i] == this->_result_set[j]) {
                dup_found = true;
                break;
            }
        }
        if (!dup_found) this->add(o._result_set[i]);
    }

    /// double-pointer method O(n_1 + n_2)

    //vector<long> _union;

    //size_t index_a = 0, index_b = 0;
    //long recno_a = 0, recno_b = 0;
    //long to_insert = 0;
    //while (index_a < this->_result_set.size() && index_b < o._result_set.size()) {
    //    recno_a = this->_result_set[index_a]; recno_b = o._result_set[index_b];
    //    if (recno_a == recno_b) {
    //        to_insert = recno_a; // eliminate duplicate values (O(n_1 + n_2))
    //        index_a++;
    //        index_b++;
    //    }
    //    else if (recno_a < recno_b) {
    //        to_insert = recno_a;
    //        index_a++;
    //    }
    //    else { // if (recno_a > recno_b)
    //        to_insert = recno_b;
    //        index_b++;
    //    }
    //    _union.push_back(to_insert);
    //}

    ///// Add remaining recnos 
    //int i = (index_a < this->_result_set.size() ? index_a : index_b);
    //const vector<long>& remaining = (index_a < this->_result_set.size() ? this->_result_set : o._result_set);
    //for (i; i < remaining.size(); i++) {
    //    _union.push_back(remaining.at(i));
    //}

    //if (debug) {
    //    cout << "set_union() : _union = [";
    //    for (int i = 0; i < _union.size(); i++)
    //        cout << _union[i] << " ";
    //    cout << "]" << endl;
    //}
    //this->_result_set = _union;
}

std::vector<long> ResultSet::vector_recnos() {
    return this->_result_set;
}
