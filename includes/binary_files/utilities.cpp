#include "utilities.h"

bool file_exists(const char filename[]) {
    fstream file(filename, ios::in);
    bool exists = !file.fail();
    file.close();
    return exists;
}

void open_fileW(fstream& file, const char filename[]) {
    const bool debug = false;

    file.open(filename, ios::binary | ios::out | ios::trunc);
    if (file.fail()) {
        cout << "open_fileW() : Could not open file " << filename << "!" << endl;
        throw("File failed to open.");
    }
}

/*
* Opens #param file with #param filename.
*
* If a file with the name #param filename already, exists, open_fileRW simply
* opens the file for read and write, and does not overwrite the existing file.
*
* If not, opens the file for write and creates a new file.
*/
void open_fileRW(fstream& file, const char filename[]) {

    if (file_exists(filename)) {
        file.open(filename, fstream::in | fstream::out | fstream::binary | fstream::app);
        if (file.fail()) {
            cout << "open_fileRW() : Could not open file " << filename << " to read!" << endl;
            throw("file RW failed  ");
        }
        return;
    }

    file.open(filename, ios::binary | ios::out | ios::app);
    if (file.fail()) {
        cout << "open_fileRW() : Could not open file " << filename << " to write!" << endl;
        throw("file RW failed  ");
    }
}

int write_info(const vector<string>& data, const string bin) {
    fstream f;
    FileRecord r(data);
    open_fileRW(f, bin.c_str());
    r.write(f);
    f.close();
    return r.recno;
}

void read_info(const string txt, vector<string>& data) {
    assert(data.empty());
    fstream f(txt, fstream::in);
    if (f.fail()) {
        cout << "open_fileRW() : Could not open file " << txt << " to read!" << endl;
        throw("read_info failed  ");
    }
    string fields;
    getline(f, fields);

    size_t current_index = 0;
    while (current_index < fields.size()) {
        string field = fields.substr(current_index, fields.find(' ', current_index) - current_index);
        data.push_back(field);
        current_index += (field.size() + 1);
    }

    f.close();
}