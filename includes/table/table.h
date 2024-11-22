#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include <iomanip>
#include <string> 
#include <vector>
#include <queue>
#include <fstream>

using namespace std;
// #include "consts:b"
#include "typedefs.h"
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"

//:public token
class Table {
private:
    string _name;
    string _file_name;

    vector <MMap<string, long>> _indices;
    vectorstr _field_names;
    map_sl _field_map;

    static int serial;

    bool _empty;
    long _last_record;
    vector<long> _select_recnos;

    // custom fields
    int _record_count;

    static const bool DEBUG = false;
public:
    Table();

    Table(const string& _name);
    Table(const string& _name, const vector<string>& field_names); //vector<emap 35> indices;
    
    //Table& operator=(const Table& o);

    void delete_table();
    void set_fields(vector<string> fld_names);
    vectorstr get_fields() const;
    bool is_empty();
    int insert_into(vectorstr& fields); //return row number
    int field_col_no(string field_name);
    Table select_all(vectorstr fields);
    Table select(const vectorstr& fields, const Queue<Token*>& postfix);
    Table select(const vectorstr& fields, const vectorstr& condition);

    Table select(const vectorstr& fields, const string& field,
        const string op, const string& value);

    int get_token_type(const string& s); //NOT USED!
    void reindex(); //call this from ctor!
    string Name() { return _name; }
    friend ostream& operator << (ostream& outs, const Table& t);
    vectorlong cond(const Queue<Token*>& post);
    Table vector_to_table(const vectorstr& fields,
        const vectorlong& vector_of_recnos);

    vector<long> select_recnos() { return _select_recnos; }
};

#endif