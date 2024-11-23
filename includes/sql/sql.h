#ifndef SQL_H
#define SQL_H

#include <algorithm>

#include "../table/table.h"
#include "../parser/parser.h"
#include "../binary_files/utilities.h"

class SQL {
private:
    Table _table;
public:
    SQL();

    Table& command(string query);

    vector<long> select_recnos();

};

#endif