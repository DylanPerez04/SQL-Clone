#ifndef SQL_H
#define SQL_H

#include <algorithm>

#include "../table/table.h"
#include "../parser/parser.h"
#include "../binary_files/utilities.h"

/*
* TODO : Make capital keywords valid
*/

class SQL {
private:
    Table _select_table;
public:
    SQL();

    //~SQL();

    Table command(string query);

    vector<long> select_recnos();

};

#endif