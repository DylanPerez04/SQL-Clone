#ifndef SQL_H
#define SQL_H

#include <algorithm>

#include "../table/table.h"
#include "../parser/parser.h"
#include "../binary_files/utilities.h"

/*
* TODO : Make capital keywords valid
* TODO : Store names of all tables previously selected to delete all at the end of SQL object's lifespan
*/

class SQL {
private:
    vector<string> _tables_accessed;
    Table _select_table;
public:
    SQL();

    ~SQL();

    Table command(string query);

    vector<long> select_recnos();

};

#endif