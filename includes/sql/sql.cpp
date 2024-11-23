#include "sql.h"

SQL::SQL() {}

Table& SQL::command(string query) {
    const bool debug = false;
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

    Table to_return;
    if (cmd == "make") {
        fields = ptree.get("col");
        if (debug) cout << "command() : make command called. fields = " << fields << endl;
        to_return = Table(table_name, fields);
    }
    else if (cmd == "insert") {
        if (!file_exists(file_name.c_str())) return _table;
        if (debug) cout << "command() : insert command called." << endl;
        to_return = Table(table_name);
        fields = ptree.get("values");
        if(debug) cout << "command() : cmd == insert | fields = " << fields << endl;
        to_return.insert_into(fields);
    }
    else if (cmd == "select") {
        if (!file_exists(file_name.c_str())) return _table;
        _table = Table(table_name);
        fields = ptree.get("fields");
        if (debug) cout << "command() : select | fields = " << fields << endl;
        if (!ptree.contains("where")) {
            cout << "There is no condition" << endl;
            if (fields.front() == "*")
                to_return = _table.select_all(_table.get_fields());
            else
                to_return = _table.select_all(fields);
        }
        else if(ptree.get("where").front() == "yes") {
            if (fields.front() == "*")
                to_return = _table.select(_table.get_fields(), ptree.get("condition"));
            else
                to_return = _table.select(fields, ptree.get("condition"));
        }

        cout << to_return << endl << endl;
    }
    return to_return;
}

vector<long> SQL::select_recnos()
{
    return _table.select_recnos();
}
