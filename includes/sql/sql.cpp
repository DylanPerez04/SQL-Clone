#include "sql.h"

SQL::SQL() {}

SQL::~SQL() {
    const bool debug = false;
    if (debug) cout << "~SQL() : _tables_access = " << _tables_accessed << endl;
    for (string _name : _tables_accessed) {
        if (debug) cout << "~SQL() : Deleting file " << _name << "..." << endl;
        Table t(_name);
        t.delete_table();
    }
}

Table SQL::command(string query) {
    const bool debug = false;
    Parser parsed_query(query.c_str());
    MMap<string, string>& ptree = parsed_query.parse_tree();
    if (ptree.empty()) {
        if (debug) cout << "command() : Invalid query!" << endl;
        _select_table = Table();
        return _select_table;
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
        if (!file_exists(file_name.c_str())) return to_return;
        if (debug) cout << "command() : insert command called." << endl;
        to_return = Table(table_name);
        fields = ptree.get("values");
        if(debug) cout << "command() : cmd == insert | fields = " << fields << endl;
        to_return.insert_into(fields);
    }
    else if (cmd == "select") {
        if (!file_exists(file_name.c_str())) return to_return;
        _select_table = Table(table_name);
        fields = ptree.get("fields");
        if (debug) cout << "command() : select | fields = " << fields << endl;
        if (!ptree.contains("where")) {
            cout << "There is no condition" << endl;
            if (fields.front() == "*")
                to_return = _select_table.select_all(_select_table.get_fields());
            else
                to_return = _select_table.select_all(fields);
        }
        else if(ptree.get("where").front() == "yes") {
            if (debug) cout << "command() : ptree[\"condition\"] = " << ptree.get("condition") << endl;
            if (fields.front() == "*")
                to_return = _select_table.select(_select_table.get_fields(), ptree.get("condition"));
            else
                to_return = _select_table.select(fields, ptree.get("condition"));
        }

        cout << to_return << endl << endl;
        _tables_accessed.push_back(to_return.Name());
    }
    return to_return;
}

vector<long> SQL::select_recnos()
{
    return _select_table.select_recnos();
}
