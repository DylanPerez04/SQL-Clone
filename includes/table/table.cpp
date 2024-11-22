#include "table.h"

// custom includes
#include "../shunting_yard/shunting_yard.h"
#include "../binary_files/utilities.h"

int Table::serial = 0;

Table::Table() : _empty(true), _last_record(-1), _record_count(0) {}

Table::Table(const string& _name, const vector<string>& field_names) : Table() {
    serial++;
    this->_name = _name;
    this->_file_name = _name + ".bin";

    fstream f;
    open_fileW(f, _file_name.c_str());
    f.close();

    set_fields(field_names);
}
//Table& Table::operator=(const Table& o) {
//    
//}


Table::Table(const string& _name) : Table() {
    this->_name = _name;
    this->_file_name = _name + ".bin";

    reindex();
}

void Table::delete_table() {
    remove(_file_name.c_str());
    string _field_file_name = _name + "_fields.txt";
    remove(_field_file_name.c_str());

    _name.clear();
    _file_name.clear();
    _indices.clear();
    _field_names.clear();
    _field_map.clear();
    _select_recnos.clear();

    //_keywords.clear();
    //OPERATOR_LOGICAL.clear();
    //OPERATOR_RELATIONAL.clear();

    _record_count = 0;
    _empty = true;
}

void Table::set_fields(vector<string> fld_names) {

    fstream f;
    this->_field_names = fld_names;
    f.open(_name + "_fields.txt", ios::out);
    if (f.fail()) {
        cout << "Table::set_fields() : Failed to open " << _name << "_fields.txt for output!" << endl;
        exit(0);
    }

    for (int i = 0; i < _field_names.size(); i++) {
        _indices.push_back(MMap<string, long>());
        _field_map.insert(_field_names[i], i);
        f << _field_names[i] << " ";
        //f << _field_names[i] << endl;
    }
    f.close();
}

vectorstr Table::get_fields() const {
    return _field_names;
}

bool Table::is_empty() {
    return _empty;
}

//return row number
int Table::insert_into(vectorstr& fields) {
    assert(fields.size() <= _field_names.size());
    int recnum = write_info(fields, _file_name);
    _last_record = recnum;
    for (int i = 0; i < _field_names.size(); i++)
        _indices[i].insert(fields[i], recnum);

    _record_count++;
    _empty = false;

    return recnum;
} 

int Table::field_col_no(string field_name) {
    return _field_map.get(field_name);
}

Table Table::select_all(vectorstr fields) {

    Table t("_select_table_" + to_string(++serial), fields);

    fstream f;
    open_fileRW(f, _file_name.c_str());
    FileRecord r;

    int recnum = 0;
    vectorstr to_insert;
    r.read(f, recnum);
    while (!f.eof()) {
        _select_recnos.push_back(recnum);
        for (string field_name : fields)
            to_insert.push_back(r.get_record()[_field_map.get(field_name)]);
        t.insert_into(to_insert);
        r.read(f, ++recnum);
        to_insert.clear();
    }

    return t;
}

Table Table::select(const vectorstr& fields, const Queue<Token*>& postfix) {

    if (DEBUG) cout << "select(fields, postfix) : " << endl;

    _select_recnos = cond(postfix);
    
    if (DEBUG) {
        vectorlong::iterator it = _select_recnos.begin();
        cout << "select(2) : recnums size = " << _select_recnos.size() << " | recnums from cond() : ";
        while (it != _select_recnos.end())
            cout << *it++ << " ";
        cout << endl;
    }

    return vector_to_table(fields, _select_recnos);
}

/*
* @param fields The formal name of the fields each record must contain
* @param condition The infix conditions to be met
* @returns A Table containing record numbers for all records the meet #param condition
*/
Table Table::select(const vectorstr& fields, const vectorstr& condition) {
    if (condition.size() == 3)
        return select(fields, condition[0], condition[1], condition[2]);

    Queue<Token*> infix;

    /// Convert postfix #param condition to infix
    for (vectorstr::const_iterator it = condition.cbegin(); it != condition.cend(); it++) {
        string token = *it;
        if (token == "(") infix.push(new LeftParen());
        else if (token == ")") infix.push(new RightParen());
        else if (Operator::get_operator(token) != UNKNOWN) {
            if (Operator::get_operator(token) < AND) infix.push(new Relational(token));
            else infix.push(new Logical(token));
        }
        else infix.push(new TokenStr(token));
    }
    ShuntingYard sy(infix);
    _select_recnos = cond(sy.postfix());
    return  vector_to_table(fields, _select_recnos);
}

// TODO : If grader doesn't care about _select_recnos' order, have this select method call the better select method
Table Table::select(const vectorstr& fields, const string& field,
    const string op, const string& value) {
    _select_recnos.clear();

    Table t("_select_table_" + to_string(serial), fields);
    size_t field_index = _field_map.get(field);
    OperatorType _op = Operator::get_operator(op);

    fstream f;
    open_fileRW(f, _file_name.c_str());
    FileRecord r;

    vectorstr to_insert;
    MMap<string, long> map = _indices[field_index];
    MMap<string, long>::Iterator it;
    switch (_op) {
        case EQUAL:
            _select_recnos = map.get(value);
            for (int recnum : _select_recnos) {
                for (string to_include : fields) {
                    r.read(f, recnum);
                    to_insert.push_back(r[_field_map.get(to_include)]);
                }
                t.insert_into(to_insert);
                to_insert.clear();
            }
            break;
        case LESS_THAN:
            for (it = map.begin(); (*it).key < value; it++) {
                for (int recnum : (*it).value_list) {
                    _select_recnos.push_back(recnum);
                    for (string to_include : fields) {
                        r.read(f, recnum);
                        to_insert.push_back(r[_field_map.get(to_include)]);
                    }
                    t.insert_into(to_insert);
                    to_insert.clear();
                }
            }
            break;
        case GREATER_THAN:
            for (it = map.upper_bound(value); it != map.end(); it++) {

                for (int recnum : (*it).value_list) {
                    _select_recnos.push_back(recnum);
                    for (string to_include : fields) {
                        r.read(f, recnum);
                        to_insert.push_back(r[_field_map.get(to_include)]);
                    }
                    t.insert_into(to_insert);
                    to_insert.clear();
                }

            }
            break;
        case LESS_EQUAL:
            for (it = map.begin(); (*it).key <= value; it++) {
                for (int recnum : (*it).value_list) {
                    _select_recnos.push_back(recnum);
                    for (string to_include : fields) {
                        r.read(f, recnum);
                        to_insert.push_back(r[_field_map.get(to_include)]);
                    }
                    t.insert_into(to_insert);
                    to_insert.clear();
                }
            }
            break;
        case GREATER_EQUAL:
            for (it = map.lower_bound(value); it != map.end(); it++) {

                for (int recnum : (*it).value_list) {
                    _select_recnos.push_back(recnum);
                    for (string to_include : fields) {
                        r.read(f, recnum);
                        to_insert.push_back(r[_field_map.get(to_include)]);
                    }
                    t.insert_into(to_insert);
                    to_insert.clear();
                }

            }
            break;
        case NOT_EQUAL:
            for (it = map.begin(); it != map.end(); it++) {
                if ((*it).key == value) continue;

                for (int recnum : (*it).value_list) {
                    _select_recnos.push_back(recnum);
                    for (string to_include : fields) {
                        r.read(f, recnum);
                        to_insert.push_back(r[_field_map.get(to_include)]);
                    }
                    t.insert_into(to_insert);
                    to_insert.clear();
                }
            }
            break;
        case AND:
            break;
        case OR:
            break;
        case NOT:
            break;
    }

    f.close();
    return t;
}

//NOT USED!
int Table::get_token_type(const string& s) {
    return -1;
} 

/*
* Initializes _indices with .bin files if they already exist
* Call this from ctor!
*/
void Table::reindex() {
    const bool debug = false;
    // Set fields for Table object via pre-existing _fields.txt file
    fstream f;
    f.open(_name + "_fields.txt", ios::in);
    if (f.fail()) {
        cout << "Table::set_fields() : Failed to open " << _name << "_fields.txt for input!" << endl;
        exit(0);
    }

    /// Reindex _field_map, _field_names; add mmaps to _indices
    string fields;
    getline(f, fields);

    size_t current_index = 0;
    while (current_index < fields.size()) {
        string field = fields.substr(current_index, fields.find(' ', current_index) - current_index);
        if (debug) cout << "reindex() : field = " << field << endl;
        _indices.push_back(MMap<string, long>());
        _field_map.insert(field, _field_names.size());
        _field_names.push_back(field);
        current_index += (field.size() + 1);
    }

    f.close();
 
    open_fileRW(f, _file_name.c_str());
    FileRecord r;
    int recnum = 0;
    r.read(f, recnum);
    while (!f.eof()) {
        if (debug) cout << "reindex() : r.get_record() = " << r.get_record() << endl;
        for (int i = 0; i < _field_names.size(); i++) {
            if (debug) cout << "reindex() : inserting into _indices " << r.get_record()[i] << endl;
            _indices[i].insert(r.get_record()[i], recnum);
        }
        r.read(f, ++recnum);
    }
    _record_count = recnum;
    _empty = _record_count == 0;
} 

vectorlong Table::cond(const Queue<Token*>& post) {
    Stack<Token*> output;

    Queue<Token*>::Iterator it;
    for (it = post.begin(); it != post.end(); it++) {
        if (DEBUG) cout << "cond() : post' (*it)->_token = " << (*it)->token_str() << endl;
        if ((*it)->type() == RELAT || (*it)->type() == LOGIC) {
            assert(output.size() >= 2);

            Operator* op = static_cast<Operator*>(*it);
            Token* rhs = output.pop(), // rhs - value (20)
                 * lhs = output.pop(); // lhs - field (age)

            assert(lhs != nullptr && rhs != nullptr);

            ResultSet* result = op->eval(_indices[_field_map.get(lhs->token_str())], lhs, rhs);

            if (DEBUG) {
                cout << "cond() : _result_set = [";
                vectorlong result_vector = result->vector_recnos();
                vectorlong::iterator it = result_vector.begin();
                while (it != result_vector.end())
                    cout << *it++ << " ";
                cout << "]" << endl;
            }

            output.push(result);
            continue;
        }

        output.push(*it);
    }
    
    assert(output.size() == 1 && output.top()->type() == RESULT);
    return static_cast<ResultSet*>(output.pop())->vector_recnos();
}

Table Table::vector_to_table(const vectorstr& fields, const vectorlong& vector_of_recnos) {

    if (DEBUG) cout << "vector_to_table() : " << endl;

    Table t("_select_table_" + to_string(serial), fields);

    FileRecord r;
    fstream f;
    vectorstr to_insert;
    open_fileRW(f, _file_name.c_str());
    for (long recnum : vector_of_recnos) {
        r.read(f, recnum);
        for (string field_name : fields)
            to_insert.push_back(r.get_record()[_field_map.get(field_name)]);
        t.insert_into(to_insert);
        to_insert.clear();
    }
    f.close();
    return t;
}

ostream& operator<<(ostream& outs, const Table& t) {
    outs << endl << "Table name: " << t._name << ", records: " << t._record_count << endl;

    outs << setw(25) << "record";
    for (int i = 0; i < t._field_names.size(); i++)
        outs << setw(25) << t._field_names[i];
    outs << endl << endl;

    fstream f;
    open_fileRW(f, t._file_name.c_str());
    FileRecord r;
    
    int recnum = 0;
    r.read(f, recnum);
    while (!f.eof()) {
        cout << setw(25) << recnum << setw(25) << r << endl;
        r.read(f, ++recnum);
    }
    f.close();
    outs << endl;
    return outs;
}
