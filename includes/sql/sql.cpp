#include "sql.h"

SQL::SQL() {}

Table SQL::command(string query) {
    const bool debug = true;
    Parser parsed_query(query.c_str());
    MMap<string, string>& ptree = parsed_query.parse_tree();
    if (ptree.empty()) {
        if (debug) cout << "command() : Invalid query!" << endl;
        return _table;
    }

    string cmd = ptree.get("command").front();
    string table_name = ptree.get("table_name").front();
    string file_name = table_name + ".bin";
    vector<string> fields;
    if (cmd == "make") {
        fields = ptree.get("col");
        if (debug) cout << "command() : make command called. fields = " << fields << endl;
        _table = Table(table_name, fields);
    }
    else if (cmd == "insert") {
        if (!file_exists(file_name.c_str())) return _table;
        if (debug) cout << "command() : insert command called." << endl;
        _table = Table(table_name);
        cout << "test1" << endl;
        fields = ptree.get("values");
        cout << "test2" << endl;
        _table.insert_into(fields);
        cout << "test3" << endl;

    }
    else if (cmd == "select") {
        if (!file_exists(file_name.c_str())) return _table;
        _table = Table(table_name);
        fields = ptree.get("fields");
        if (!ptree.contains("where")) {
            if (fields.front() == "*")
                _table.select_all(_table.get_fields());
            else
                _table.select_all(fields);
        }
        else if(ptree.get("where").front() == "yes") {
            if (fields.front() == "*")
                _table.select(_table.get_fields(), ptree.get("condition"));
            else
                _table.select(fields, ptree.get("condition"));
        }
    }

    return _table;
}

vector<long> SQL::select_recnos()
{
    return _table.select_recnos();
}
