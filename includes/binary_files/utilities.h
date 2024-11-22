#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "file_record.h"

using namespace std;

bool file_exists(const char filename[]);

void open_fileW(fstream& file, const char filename[]);

/*
* Opens #param file with #param filename. 
* 
* If a file with the name #param filename already, exists, open_fileRW simply
* opens the file for read and write, and does not overwrite the existing file.
* 
* If not, opens the file for write and creates a new file.
*/
void open_fileRW(fstream& file, const char filename[]);

int write_info(const vector<string>& data, const string bin);

void read_info(const string txt, vector<string>& data);

#endif