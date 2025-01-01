/** @file utilities.h
* 
* Defines the methods that file_record.h utilizes to directly read and write from .bin and .txt files used
* to store SQL table records.
*/

#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "file_record.h"

using namespace std;

// TODO : Add exception handling to all methods and mention exceptions in documentation
// TODO : Update open_file methods to close fstreams if open when passed as a parameter

/**
* Checks if a file within the project's out/build/x64-Debug/bin/ folder exists with the name @p filename.
* 
* If a file named @p filename already exists, the file won't be truncated.
* 
* @param[in] filename The name of the file being searched for
* @returns Whether a file with the name @p filename exists
*/
bool file_exists(const char filename[]);

/**
* Opens a file named @p filename for writing using the passed in fstream parameter @p file. 
* 
* The passed in fstream instance, @p file, is closed if open at the time of being passed as an actual parameter.
* 
* Being openned for writing, any file that currently exists with the name @p filename will be truncated (wiped); if no
* file with the name @p filename exists, the method will create one.
* 
* @param[in, out] file An fstream instance to be used to open a file with the named @p filename for writing
* @param[in] filename The name of the file to open for writing
*/
void open_fileW(fstream& file, const char filename[]);

/**
* Opens a file named @p filename for reading and writing using the passed in fstream parameter @p file.
* 
* The passed in fstream instance, @p file, is closed if open at the time of being passed as an actual parameter.
* 
* If a file with the name @p filename already, exists, open_fileRW simply
* opens the file for read and write, and does not overwrite the existing file.
* If not, opens the file for write and creates a new file.
* 
* @param[in, out] file An fstream instance to be used to open a file with the named @p filename for writing
* @param[in] filename The name of the file to open for writing
*/
void open_fileRW(fstream& file, const char filename[]);

/**
* Writes @p data into a file with the filename @p bin via a FileRecord.
* 
* It is assumed that the file name passed in via @p bin ends with .bin.
* 
* @param[in] data The data to be passed into the FileRecord as entries to an SQL field
* @param[in] bin The name of the .bin file to write @p data to
*/
int write_info(const vector<string>& data, const string bin);

/**
* Reads an SQL table's field names from the file with the filename @p txt, and
* inputs the field names into @p data.
* 
* It is assumed that the file name passed in via @p txt ends with .txt.
* 
* @param[in] txt The file name of the .txt file to read the SQL table field names from
* @param[in, out] data The `vector<string>` object to store the SQL table field names in
*/
void read_info(const string txt, vector<string>& data);

#endif