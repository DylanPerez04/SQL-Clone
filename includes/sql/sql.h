#ifndef SQL_H
#define SQL_H

#include <algorithm>

#include "../table/table.h"
#include "../parser/parser.h"
#include "../binary_files/utilities.h"

/*
* TODO : Reset STokenizer so that it tokenizes as normal; Make Parser's set_string convert long strings ("Sammuel L.") into a single string token
* to be inserted to input_q; Create a new Parser adj_table that checks for parenthesis. 
* 
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