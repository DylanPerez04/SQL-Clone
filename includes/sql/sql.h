/** @file sql.h
* Defines the SQL object and related methods and fields. 
*/

#ifndef SQL_H
#define SQL_H

#include <algorithm>

#include "../table/table.h"
#include "../parser/parser.h"
#include "../binary_files/utilities.h"

/*
* TODO : Make capital keywords valid
* TODO : Refactor _select_table so that it's a vector of recnos
* TODO : Make CREATE a valid command
*/

/**
* The SQL object acts an SQL-clone, capable of creating a table, inserting records into a table,
* and selecting records from a table.
*/
class SQL {
private:
    /// The name all session select tables
    vector<string> _tables_accessed;

    /// A copy of the previous table selected
    Table _select_table;
public:
    /// Initializes a newly created SQL object
    SQL();

    
    /// Deletes all session binary and .txt files created as a result of session-executed select commands
    ~SQL();

    /**
     * Executes an SQL query passed as a string via @p query @p .
     * 
     * @param[in] query The SQL query to process
     * @returns The resulting table created or accessed as a resort of the query;
     * an empty Table object in the event of an invalid query
     */
    Table command(string query);

    /**
    * Retrieves the previously selected record numbers.
    * @returns The previously selected record numbers from the last SELECT command; 
    * returns an empty `vector<long>` if no select commands have yet been made.
    */
    vector<long> select_recnos();

};

#endif