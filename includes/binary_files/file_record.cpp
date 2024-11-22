#include "file_record.h"

// TODO : Make _num_of_fields DYNAMIC based on data passed in (make _record dynamic)

FileRecord::FileRecord() : recno(-1) {}

FileRecord::FileRecord(const vector<string> data) : FileRecord() {
    assert(data.size() <= _num_of_fields);

    for (int i = 0; i < data.size(); i++) {
        strncpy(_record[i], data[i].c_str(), MAX);
        _record[i][MAX] = '\0';
    }

}

//FileRecord::FileRecord(char data[_num_of_fields][MAX]) : FileRecord() {
//    for (int i = 0; i < _num_of_fields; i++) {
//        memcpy(_record[i], data[i], MAX);
//        _record[i][MAX] = '\0';
//    }
//}

FileRecord::FileRecord(string data) : FileRecord() {
    strncpy(_record[0], data.c_str(), MAX);
    _record[0][MAX] = '\0';
}

vector<string> FileRecord::get_record() const {
    vector<string> data;
    for (int i = 0; i < _num_of_fields; i++)
        data.push_back(_record[i]);
    return data;
}

string FileRecord::operator[](int index) const {
    assert(index >= 0 && index < _num_of_fields);
    return _record[index];
}

streampos FileRecord::read(fstream& file, const int& recno) {
    file.seekp(recno * (_num_of_fields * (MAX + 1)));
    file.read(*_record, _num_of_fields * (MAX + 1));

    this->recno = recno;

    return file.gcount();
}

streampos FileRecord::write(fstream& file) {
    file.seekp(0, file.end);
    streampos recnum = file.tellp() / (_num_of_fields * (MAX + 1));

    file.write(*_record, static_cast<std::streamsize>(_num_of_fields * (MAX + 1)));

    this->recno = static_cast<int>(recnum);

    return recnum;
}
