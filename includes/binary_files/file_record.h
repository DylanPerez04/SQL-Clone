/** @file file_record.h
* 
* Defines the FileRecord struct and related members to quickly read and write .bin and .txt files necessary
* for the long-term storage and tracking of SQL tables.
*/

#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <string>
#include <cassert>
#include <cstring>

using namespace std;

// TODO : Add exception handling to all methods and mention exceptions in documentation
// TODO : Make a class

/**
* Implementation of a means to quickly read and write indiviudal SQL table records.
* 
* The FileRecord object handles reading and writing from .bin files and .txt files, which act as SQL's
* sole means of persistent, long-term storage of table records and table field-names, respectively.
* 
* Each .bin file contains the records for a single table within the database, while each .txt file
* contains the field-names for a single table within the database. 
*/
struct FileRecord {

    /// The last record number read or the record number of the table record last written.
    int recno;

    /**
    * The maximum number of field values that a FileRecord object can hold, read, and write as part of
    * a single individual SQL table record. 
    */
    static const int _num_of_fields = 10;

    /** 
    * The maximum number of characters an individual field value can have; the number of
    * bytes read or written to a binary file when reading or writing an individual field
    * values.
    */
    static const int MAX = 100;
    
    /// The field values of a single SQL table record
    char _record[_num_of_fields][MAX + 1] = {};

    /**
    * Initializes an empty FileRecord object.
    */
    FileRecord();

    /**
    * Stores the record field values held within @p data in the FileRecord object
    * 
    * @param[in] data A vector of a table record field values to store
    */
    FileRecord(const vector<string> data);

    //FileRecord(char data[_num_of_fields][MAX]);

    /**
    * Stores a single record field value held within @p data in the FileRecord object.
    * 
    * @param[in] data A record field value to store
    */
    FileRecord(string data);

    /**
    * Retrieves the FileRecord object's record field values, in the order they are stored,
    * as a vector of strings.
    * 
    * @returns A `vector<string>` representaton of _record, where each string is a field value
    */
    vector<string> get_record() const;

    /**
    * Retrieves a record field value at @p index. 
    * 
    * @returns A field value at index @p index within FileRecord::_record
    */
    string operator[](int index) const;

    /**
    * Outputs the FileRecord object's record field values.
    * 
    * @param[in, out] outs An ostream object to insert @p file_record 's record field values for output
    * @param[in] file_record The FileRecord object to be printed out
    * @returns An ostream object reference with @p file_record 's record field values inserted
    */
    friend ostream& operator<<(ostream& outs, const FileRecord& file_record) {
        for (const char* field: file_record._record)
            outs << setw(25) << field;
        
        return outs;
    }

    /**
    * Reads file record @p recno from @p file and stores the record field
    * values within the FileRecord object. Any record field values previously stored
    * within the FileRecord object will be overwritten.
    * 
    * It is assumed that @p file is openned and in binary mode.
    * 
    * @param[in, out] file The `fstream` object to read the SQL table record from
    * @param[in] recno The record number to read from @p file
    * @returns The number of bytes read
    */
    streampos read(fstream& file, const int& recno);

    /**
    * Appends the record field values stored within the FileRecord object into @p
    * file. 
    * 
    * It is assumed that @p file is openned and in binary mode
    * 
    * @param[in, out] file The fstream object to append the table record stored within the FileRecord object
    * @returns The record number of the record written within @p file
    */
    streampos write(fstream& file);
};

#endif