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

struct FileRecord {
    static const int _num_of_fields = 10;
    static const int MAX = 100; // max
    int recno;
    char _record[_num_of_fields][MAX + 1] = {};

    FileRecord();
    FileRecord(const vector<string> data);
    //FileRecord(char data[_num_of_fields][MAX]);
    FileRecord(string data);

    vector<string> get_record() const;

    string operator[](int index) const;

    friend ostream& operator<<(ostream& outs, const FileRecord& file_record) {
        for (const char* field: file_record._record)
            outs << setw(25) << field;
        
        return outs;
    }

    streampos read(fstream& file, const int& recno);

    streampos write(fstream& file);
};

#endif