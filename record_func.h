#ifndef RECORD_FUNC_H
#define RECORD_FUNC_H
#include <iostream>
#include <fstream>
#include "record.h"
#include <vector>


using namespace std;

bool file_exists(const char filename[]);
void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);
bool f_read_info(const char filename[], vector<string>& v);
bool f_write_info(const char filename[], const vector<string> &v);
long f_size(fstream& f, int max_size);


#endif // RECORD_FUNC_H
